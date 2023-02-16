// Copyright 2023 Dolby Laboratories

#include "DolbyIOVideoSink.h"

#if PLATFORM_MAC
#include <dolbyio/comms/media_engine/video_frame_macos.h>
#endif
#include <dolbyio/comms/media_engine/video_utils.h>

#include "Async/Async.h"
#include "Engine/Texture2D.h"
#include "Launch/Resources/Version.h"

namespace DolbyIO
{
	void FVideoSink::AddStream(const FString& ParticipantID, const FString& StreamID)
	{
		FScopeLock Lock{&FrameLock};
		StreamFrames.Emplace(StreamID, FFrame{});
		ParticipantStreams.Emplace(ParticipantID, StreamID);
	}

	void FVideoSink::RemoveStream(const FString& ParticipantID, const FString& StreamID)
	{
		FScopeLock Lock{&FrameLock};
		StreamFrames.Remove(StreamID);
		ParticipantStreams.Remove(ParticipantID);
	}

	UTexture2D* FVideoSink::GetTexture(const FString& ParticipantID)
	{
		FScopeLock Lock{&FrameLock};
		if (FString* StreamID = ParticipantStreams.Find(ParticipantID))
		{
			if (FFrame* Frame = StreamFrames.Find(*StreamID))
			{
				return Frame->Texture;
			}
		}
		return nullptr;
	}

	constexpr int Stride = 4;

	void FVideoSink::FFrame::RecreateIfNeeded(int Width, int Height)
	{
		if (Texture)
		{
			if (Texture->GetSizeX() == Width && Texture->GetSizeY())
			{
				return;
			}

			Texture->RemoveFromRoot();
		}

		Region.Width = Width;
		Region.Height = Height;
		Buffer.SetNumUninitialized(Width * Height * Stride);

		Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);
		Texture->AddToRoot();
		Texture->UpdateResource();
	}

	void FVideoSink::FFrame::Convert(dolbyio::comms::video_frame& VideoFrame)
	{
#if PLATFORM_WINDOWS
		if (dolbyio::comms::video_frame_i420* FrameI420 = VideoFrame.get_i420_frame())
		{
			dolbyio::comms::video_utils::format_converter::i420_to_argb(
			    FrameI420->get_y(), FrameI420->stride_y(), FrameI420->get_u(), FrameI420->stride_u(),
			    FrameI420->get_v(), FrameI420->stride_v(), Buffer.GetData(), VideoFrame.width() * Stride,
			    VideoFrame.width(), VideoFrame.height());
		}
#elif PLATFORM_MAC
		class FLockedCVPixelBuffer
		{
		public:
			FLockedCVPixelBuffer(CVPixelBufferRef PixelBuffer) : PixelBuffer(PixelBuffer)
			{
				CVPixelBufferLockBaseAddress(PixelBuffer, kCVPixelBufferLock_ReadOnly);
			}
			~FLockedCVPixelBuffer()
			{
				CVPixelBufferUnlockBaseAddress(PixelBuffer, kCVPixelBufferLock_ReadOnly);
			}

			operator CVPixelBufferRef() const
			{
				return PixelBuffer;
			}

		private:
			CVPixelBufferRef PixelBuffer;
		};

		if (dolbyio::comms::video_frame_macos* FrameMac = VideoFrame.get_native_frame())
		{
			FLockedCVPixelBuffer PixelBuffer{FrameMac->get_buffer()};
			dolbyio::comms::video_utils::format_converter::nv12_to_argb(
			    static_cast<uint8*>(CVPixelBufferGetBaseAddressOfPlane(PixelBuffer, 0)),
			    CVPixelBufferGetBytesPerRowOfPlane(PixelBuffer, 0),
			    static_cast<uint8*>(CVPixelBufferGetBaseAddressOfPlane(PixelBuffer, 1)),
			    CVPixelBufferGetBytesPerRowOfPlane(PixelBuffer, 1), Buffer.GetData(), VideoFrame.width() * Stride,
			    VideoFrame.width(), VideoFrame.height());
		}
#endif
	}

	void FVideoSink::FFrame::Update()
	{
		Texture->UpdateTextureRegions(0, 1, &Region, Region.Width * Stride, Stride, Buffer.GetData());
	}

	void FVideoSink::handle_frame(const std::string& StreamID, const std::string& TrackID,
	                              std::unique_ptr<dolbyio::comms::video_frame> VideoFrame)
	{
		AsyncTask(ENamedThreads::GameThread,
		          [=, VideoFrame = MoveTemp(VideoFrame)]
		          {
			          FScopeLock Lock{&FrameLock};
			          if (FFrame* Frame = StreamFrames.Find(StreamID.c_str()))
			          {
				          Frame->RecreateIfNeeded(VideoFrame->width(), VideoFrame->height());
				          Frame->Convert(*VideoFrame);
				          Frame->Update();
			          }
		          });
	}
}
