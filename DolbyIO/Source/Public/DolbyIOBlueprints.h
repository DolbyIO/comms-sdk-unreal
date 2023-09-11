// Copyright 2023 Dolby Laboratories

#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "DolbyIO.h"

#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"

#include "DolbyIOBlueprints.generated.h"

#define DLB_DEFINE_CONSTRUCTOR(ClassName)              \
	{                                                  \
		ClassName* Self = NewObject<ClassName>();      \
		Self->WorldContextObject = WorldContextObject; \
		return Self;                                   \
	}

inline UDolbyIOSubsystem* GetDolbyIOSubsystem(const UObject* WorldContextObject)
{
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);
	return GameInstance ? GameInstance->GetSubsystem<UDolbyIOSubsystem>() : nullptr;
}

#define DLB_GET_SUBSYSTEM                                                          \
	UDolbyIOSubsystem* DolbyIOSubsystem = GetDolbyIOSubsystem(WorldContextObject); \
	if (!DolbyIOSubsystem)                                                         \
	{                                                                              \
		return;                                                                    \
	}

#define DLB_DEFINE_ACTIVATE_METHOD(MethodName, SuccessEvent, ...)                                     \
	void Activate() override                                                                          \
	{                                                                                                 \
		DLB_GET_SUBSYSTEM;                                                                            \
		DolbyIOSubsystem->SuccessEvent.AddDynamic(this, &UDolbyIO##MethodName::SuccessEvent##Impl);   \
		DolbyIOSubsystem->On##MethodName##Error.AddDynamic(this, &UDolbyIO##MethodName::OnErrorImpl); \
		DolbyIOSubsystem->MethodName(__VA_ARGS__);                                                    \
	}

#define DLB_DEFINE_ACTIVATE_METHOD_Two_Events(MethodName, SuccessEvent1, SuccessEvent2, ...)          \
	void Activate() override                                                                          \
	{                                                                                                 \
		DLB_GET_SUBSYSTEM;                                                                            \
		DolbyIOSubsystem->SuccessEvent1.AddDynamic(this, &UDolbyIO##MethodName::SuccessEvent1##Impl); \
		DolbyIOSubsystem->SuccessEvent2.AddDynamic(this, &UDolbyIO##MethodName::SuccessEvent2##Impl); \
		DolbyIOSubsystem->On##MethodName##Error.AddDynamic(this, &UDolbyIO##MethodName::OnErrorImpl); \
		DolbyIOSubsystem->MethodName(__VA_ARGS__);                                                    \
	}

#define DLB_DEFINE_IMPL_METHOD(MethodName, SuccessEvent, ...)                                            \
	{                                                                                                    \
		SuccessEvent.Broadcast(__VA_ARGS__);                                                             \
		DLB_GET_SUBSYSTEM;                                                                               \
		DolbyIOSubsystem->SuccessEvent.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent##Impl);   \
		DolbyIOSubsystem->On##MethodName##Error.RemoveDynamic(this, &UDolbyIO##MethodName::OnErrorImpl); \
	}

#define DLB_DEFINE_IMPL_METHOD_Two_Events(MethodName, ActualSuccessEvent, SuccessEvent1, SuccessEvent2, ...) \
	{                                                                                                        \
		ActualSuccessEvent.Broadcast(__VA_ARGS__);                                                           \
		DLB_GET_SUBSYSTEM;                                                                                   \
		DolbyIOSubsystem->SuccessEvent1.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent1##Impl);     \
		DolbyIOSubsystem->SuccessEvent2.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent2##Impl);     \
		DolbyIOSubsystem->On##MethodName##Error.RemoveDynamic(this, &UDolbyIO##MethodName::OnErrorImpl);     \
	}

#define DLB_DEFINE_ERROR_METHOD(MethodName, SuccessEvent, ...)                                           \
	{                                                                                                    \
		OnError.Broadcast(__VA_ARGS__);                                                                  \
		DLB_GET_SUBSYSTEM;                                                                               \
		DolbyIOSubsystem->SuccessEvent.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent##Impl);   \
		DolbyIOSubsystem->On##MethodName##Error.RemoveDynamic(this, &UDolbyIO##MethodName::OnErrorImpl); \
	}

#define DLB_DEFINE_ERROR_METHOD_Two_Events(MethodName, SuccessEvent1, SuccessEvent2, ...)                \
	{                                                                                                    \
		OnError.Broadcast(__VA_ARGS__);                                                                  \
		DLB_GET_SUBSYSTEM;                                                                               \
		DolbyIOSubsystem->SuccessEvent1.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent1##Impl); \
		DolbyIOSubsystem->SuccessEvent2.RemoveDynamic(this, &UDolbyIO##MethodName::SuccessEvent2##Impl); \
		DolbyIOSubsystem->On##MethodName##Error.RemoveDynamic(this, &UDolbyIO##MethodName::OnErrorImpl); \
	}

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDolbyIOSetTokenOutputPin, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOSetToken : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Initializes or refreshes the client access token. Initializes the plugin unless already initialized.
	 *
	 * Successful initialization triggers the On Initialized event.
	 *
	 * For quick testing, you can manually obtain a token from the Dolby.io dashboard (https://dashboard.dolby.io)
	 * and paste it directly into the node or use the Get Dolby.io Token function.
	 *
	 * @param Token - The client access token.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Set Token"))
	static UDolbyIOSetToken* DolbyIOSetToken(const UObject* WorldContextObject, const FString& Token)
	{
		UDolbyIOSetToken* Self = NewObject<UDolbyIOSetToken>();
		Self->WorldContextObject = WorldContextObject;
		Self->Token = Token;
		return Self;
	}

	UPROPERTY(BlueprintAssignable)
	FDolbyIOSetTokenOutputPin OnInitialized;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOSetTokenOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(SetToken, OnInitialized, Token);

	UFUNCTION()
	void OnInitializedImpl()
	{
		DLB_DEFINE_IMPL_METHOD(SetToken, OnInitialized, "");
	}

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(SetToken, OnInitialized, ErrorMsg);
	}

	const UObject* WorldContextObject;
	FString Token;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDolbyIOConnectOutputPin, const FString&, LocalParticipantID,
                                               const FString&, ConferenceID, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOConnect : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Connects to a conference.
	 *
	 * Triggers On Connected if successful.
	 *
	 * @param ConferenceName - The conference name. Must not be empty.
	 * @param UserName - The name of the participant.
	 * @param ExternalID - The external unique identifier that the customer's application can add to the participant
	 * while opening a session. If a participant uses the same external ID in conferences, the participant's ID also
	 * remains the same across all sessions.
	 * @param AvatarURL - The URL of the participant's avatar.
	 * @param ConnectionMode - Defines whether to connect as an active user or a listener.
	 * @param SpatialAudioStyle - The spatial audio style of the conference.
	 * @param MaxVideoStreams - Sets the maximum number of video streams that may be transmitted to the user.
	 * @param VideoForwardingStrategy - Defines how the plugin should select conference participants whose videos
	 * will be transmitted to the local participant.
	 * @param VideoCodec - The preferred video codec.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Connect"))
	static UDolbyIOConnect* DolbyIOConnect(
	    const UObject* WorldContextObject, const FString& ConferenceName = "unreal", const FString& UserName = "",
	    const FString& ExternalID = "", const FString& AvatarURL = "",
	    EDolbyIOConnectionMode ConnectionMode = EDolbyIOConnectionMode::Active,
	    EDolbyIOSpatialAudioStyle SpatialAudioStyle = EDolbyIOSpatialAudioStyle::Shared, int MaxVideoStreams = 25,
	    EDolbyIOVideoForwardingStrategy VideoForwardingStrategy = EDolbyIOVideoForwardingStrategy::LastSpeaker,
	    EDolbyIOVideoCodec VideoCodec = EDolbyIOVideoCodec::H264)
	{
		UDolbyIOConnect* Self = NewObject<UDolbyIOConnect>();
		Self->WorldContextObject = WorldContextObject;
		Self->ConferenceName = ConferenceName;
		Self->UserName = UserName;
		Self->ExternalID = ExternalID;
		Self->AvatarURL = AvatarURL;
		Self->ConnectionMode = ConnectionMode;
		Self->SpatialAudioStyle = SpatialAudioStyle;
		Self->MaxVideoStreams = MaxVideoStreams;
		Self->VideoForwardingStrategy = VideoForwardingStrategy;
		Self->VideoCodec = VideoCodec;
		return Self;
	}

	UPROPERTY(BlueprintAssignable)
	FDolbyIOConnectOutputPin OnConnected;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOConnectOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(Connect, OnConnected, ConferenceName, UserName, ExternalID, AvatarURL, ConnectionMode,
	                           SpatialAudioStyle, MaxVideoStreams, VideoForwardingStrategy, VideoCodec);

	UFUNCTION()
	void OnConnectedImpl(const FString& LocalParticipantID, const FString& ConferenceID)
	{
		DLB_DEFINE_IMPL_METHOD(Connect, OnConnected, LocalParticipantID, ConferenceID, "");
	}

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(Connect, OnConnected, {}, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
	FString ConferenceName;
	FString UserName;
	FString ExternalID;
	FString AvatarURL;
	EDolbyIOConnectionMode ConnectionMode;
	EDolbyIOSpatialAudioStyle SpatialAudioStyle;
	int MaxVideoStreams;
	EDolbyIOVideoForwardingStrategy VideoForwardingStrategy;
	EDolbyIOVideoCodec VideoCodec;
};

UCLASS()
class DOLBYIO_API UDolbyIODemoConference : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Connects to a demo conference.
	 *
	 * The demo automatically brings in 3 invisible bots into the conference as a quick way to validate the
	 * connection to the service with audio functionality. The bots are placed at point {0, 0, 0}.
	 *
	 * Triggers On Connected if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Demo Conference"))
	static UDolbyIODemoConference* DolbyIODemoConference(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIODemoConference);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOConnectOutputPin OnConnected;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOConnectOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(DemoConference, OnConnected);

	UFUNCTION()
	void OnConnectedImpl(const FString& LocalParticipantID, const FString& ConferenceID)
	    DLB_DEFINE_IMPL_METHOD(DemoConference, OnConnected, LocalParticipantID, ConferenceID, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(DemoConference, OnConnected, {}, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDolbyIODisconnectOutputPin, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIODisconnect : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Disconnects from the current conference.
	 *
	 * Triggers On Disconnected if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Disconnect"))
	static UDolbyIODisconnect* DolbyIODisconnect(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIODisconnect);

	UPROPERTY(BlueprintAssignable)
	FDolbyIODisconnectOutputPin OnDisconnected;

	UPROPERTY(BlueprintAssignable)
	FDolbyIODisconnectOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(Disconnect, OnDisconnected);

	UFUNCTION()
	void OnDisconnectedImpl() DLB_DEFINE_IMPL_METHOD(Disconnect, OnDisconnected, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(Disconnect, OnDisconnected, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOEnableVideoOutputPin, const FString&, VideoTrackID, const FString&,
                                             ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOEnableVideo : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Enables video streaming from the given video device or the default device if no device is given.
	 *
	 * Triggers On Video Enabled if successful.
	 *
	 * @param VideoDevice - The video device to use.
	 * @param bBlurBackground - Indicates whether the background should be blurred. This parameter is ignored on
	 * platforms other than Windows and macOS.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Enable Video", AutoCreateRefTerm = "VideoDevice"))
	static UDolbyIOEnableVideo* DolbyIOEnableVideo(const UObject* WorldContextObject,
	                                               const FDolbyIOVideoDevice& VideoDevice, bool bBlurBackground = false)
	{
		UDolbyIOEnableVideo* Self = NewObject<UDolbyIOEnableVideo>();
		Self->WorldContextObject = WorldContextObject;
		Self->VideoDevice = VideoDevice;
		Self->bBlurBackground = bBlurBackground;
		return Self;
	}

	UPROPERTY(BlueprintAssignable)
	FDolbyIOEnableVideoOutputPin OnLocalCameraTrackAdded;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOEnableVideoOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(EnableVideo, OnLocalCameraTrackAdded, VideoDevice, bBlurBackground);

	UFUNCTION()
	void OnLocalCameraTrackAddedImpl(const FString& VideoTrackID)
	    DLB_DEFINE_IMPL_METHOD(EnableVideo, OnLocalCameraTrackAdded, VideoTrackID, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(EnableVideo, OnLocalCameraTrackAdded, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
	FDolbyIOVideoDevice VideoDevice;
	bool bBlurBackground;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIODisableVideoOutputPin, const FString&, VideoTrackID,
                                             const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIODisableVideo : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Disables video streaming.
	 *
	 * Triggers On Video Disabled if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Disable Video"))
	static UDolbyIODisableVideo* DolbyIODisableVideo(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIODisableVideo);

	UPROPERTY(BlueprintAssignable)
	FDolbyIODisableVideoOutputPin OnLocalCameraTrackRemoved;

	UPROPERTY(BlueprintAssignable)
	FDolbyIODisableVideoOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(DisableVideo, OnLocalCameraTrackRemoved)

	UFUNCTION()
	void OnLocalCameraTrackRemovedImpl(const FString& VideoTrackID)
	    DLB_DEFINE_IMPL_METHOD(DisableVideo, OnLocalCameraTrackRemoved, VideoTrackID, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(DisableVideo, OnLocalCameraTrackRemoved, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetScreenshareSourcesOutputPin,
                                             const TArray<FDolbyIOScreenshareSource>&, Sources, const FString&,
                                             ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetScreenshareSources : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets a list of all possible screen sharing sources. These can be entire screens or specific application
	 * windows.
	 *
	 * Triggers On Screenshare Sources Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Screenshare Sources"))
	static UDolbyIOGetScreenshareSources* DolbyIOGetScreenshareSources(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetScreenshareSources);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetScreenshareSourcesOutputPin OnScreenshareSourcesReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetScreenshareSourcesOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(GetScreenshareSources, OnScreenshareSourcesReceived);

	UFUNCTION()
	void OnScreenshareSourcesReceivedImpl(const TArray<FDolbyIOScreenshareSource>& Sources)
	    DLB_DEFINE_IMPL_METHOD(GetScreenshareSources, OnScreenshareSourcesReceived, Sources, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(GetScreenshareSources, OnScreenshareSourcesReceived, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOStartScreenshareOutputPin, const FString&, VideoTrackID,
                                             const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOStartScreenshare : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Starts screen sharing using a given source.
	 *
	 * Users should make use of the parameters to optimize for the content they are sharing. For example, for
	 * sharing dynamic content like a YouTube video, the ideal settings are MaxResolution=DownscaleTo1080p,
	 * EncoderHint=Fluid, DownscaleQuality=High.
	 *
	 * Triggers On Screenshare Started if successful.
	 *
	 * @param Source - The source to use.
	 * @param EncoderHint - Provides a hint to the plugin as to what type of content is being captured by the screen
	 * share.
	 * @param MaxResolution - The maximum resolution for the capture screen content to be shared as.
	 * @param DownscaleQuality - The quality for the downscaling algorithm to be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Start Screenshare"))
	static UDolbyIOStartScreenshare* DolbyIOStartScreenshare(
	    const UObject* WorldContextObject, const FDolbyIOScreenshareSource& Source,
	    EDolbyIOScreenshareEncoderHint EncoderHint = EDolbyIOScreenshareEncoderHint::Detailed,
	    EDolbyIOScreenshareMaxResolution MaxResolution = EDolbyIOScreenshareMaxResolution::ActualCaptured,
	    EDolbyIOScreenshareDownscaleQuality DownscaleQuality = EDolbyIOScreenshareDownscaleQuality::Low)
	{
		UDolbyIOStartScreenshare* Self = NewObject<UDolbyIOStartScreenshare>();
		Self->WorldContextObject = WorldContextObject;
		Self->Source = Source;
		Self->EncoderHint = EncoderHint;
		Self->MaxResolution = MaxResolution;
		Self->DownscaleQuality = DownscaleQuality;
		return Self;
	}

	UPROPERTY(BlueprintAssignable)
	FDolbyIOStartScreenshareOutputPin OnLocalScreenshareTrackAdded;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOStartScreenshareOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(StartScreenshare, OnLocalScreenshareTrackAdded, Source, EncoderHint, MaxResolution,
	                           DownscaleQuality);

	UFUNCTION()
	void OnLocalScreenshareTrackAddedImpl(const FString& VideoTrackID)
	    DLB_DEFINE_IMPL_METHOD(StartScreenshare, OnLocalScreenshareTrackAdded, VideoTrackID, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(StartScreenshare, OnLocalScreenshareTrackAdded, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
	FDolbyIOScreenshareSource Source;
	EDolbyIOScreenshareEncoderHint EncoderHint;
	EDolbyIOScreenshareMaxResolution MaxResolution;
	EDolbyIOScreenshareDownscaleQuality DownscaleQuality;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOStopScreenshareOutputPin, const FString&, VideoTrackID,
                                             const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOStopScreenshare : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Stops screen sharing.
	 *
	 * Triggers On Screenshare Stopped if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Stop Screenshare"))
	static UDolbyIOStopScreenshare* DolbyIOStopScreenshare(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOStopScreenshare);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOStopScreenshareOutputPin OnLocalScreenshareTrackRemoved;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOStopScreenshareOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(StopScreenshare, OnLocalScreenshareTrackRemoved);

	UFUNCTION()
	void OnLocalScreenshareTrackRemovedImpl(const FString& VideoTrackID)
	    DLB_DEFINE_IMPL_METHOD(StopScreenshare, OnLocalScreenshareTrackRemoved, VideoTrackID, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(StopScreenshare, OnLocalScreenshareTrackRemoved, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetAudioInputDevicesOutputPin, const TArray<FDolbyIOAudioDevice>&,
                                             Devices, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetAudioInputDevices : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets a list of all available audio input devices.
	 *
	 * Triggers On Audio Input Devices Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Audio Input Devices"))
	static UDolbyIOGetAudioInputDevices* DolbyIOGetAudioInputDevices(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetAudioInputDevices);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetAudioInputDevicesOutputPin OnAudioInputDevicesReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetAudioInputDevicesOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(GetAudioInputDevices, OnAudioInputDevicesReceived);

	UFUNCTION()
	void OnAudioInputDevicesReceivedImpl(const TArray<FDolbyIOAudioDevice>& Devices)
	    DLB_DEFINE_IMPL_METHOD(GetAudioInputDevices, OnAudioInputDevicesReceived, Devices, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(GetAudioInputDevices, OnAudioInputDevicesReceived, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetAudioOutputDevicesOutputPin, const TArray<FDolbyIOAudioDevice>&,
                                             Devices, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetAudioOutputDevices : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets a list of all available audio output devices.
	 *
	 * Triggers On Audio Output Devices Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Audio Output Devices"))
	static UDolbyIOGetAudioOutputDevices* DolbyIOGetAudioOutputDevices(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetAudioOutputDevices);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetAudioOutputDevicesOutputPin OnAudioOutputDevicesReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetAudioOutputDevicesOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(GetAudioOutputDevices, OnAudioOutputDevicesReceived);

	UFUNCTION()
	void OnAudioOutputDevicesReceivedImpl(const TArray<FDolbyIOAudioDevice>& Devices)
	    DLB_DEFINE_IMPL_METHOD(GetAudioOutputDevices, OnAudioOutputDevicesReceived, Devices, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(GetAudioOutputDevices, OnAudioOutputDevicesReceived, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetCurrentAudioInputDeviceOutputPin, const FDolbyIOAudioDevice&,
                                             Device, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetCurrentAudioInputDevice : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets the current audio input device.
	 *
	 * Triggers On Current Audio Input Device Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Current Audio Input Device"))
	static UDolbyIOGetCurrentAudioInputDevice* DolbyIOGetCurrentAudioInputDevice(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetCurrentAudioInputDevice);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioInputDeviceOutputPin OnCurrentAudioInputDeviceReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioInputDeviceOutputPin OnCurrentAudioInputDeviceReceivedNone;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioInputDeviceOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD_Two_Events(GetCurrentAudioInputDevice, OnCurrentAudioInputDeviceReceived,
	                                      OnCurrentAudioInputDeviceReceivedNone);

	UFUNCTION()
	void OnCurrentAudioInputDeviceReceivedImpl(const FDolbyIOAudioDevice& Device)
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentAudioInputDevice, OnCurrentAudioInputDeviceReceived,
	                                      OnCurrentAudioInputDeviceReceived, OnCurrentAudioInputDeviceReceivedNone,
	                                      Device, "");

	UFUNCTION()
	void OnCurrentAudioInputDeviceReceivedNoneImpl()
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentAudioInputDevice, OnCurrentAudioInputDeviceReceivedNone,
	                                      OnCurrentAudioInputDeviceReceived, OnCurrentAudioInputDeviceReceivedNone, {},
	                                      "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD_Two_Events(GetCurrentAudioInputDevice, OnCurrentAudioInputDeviceReceived,
		                                   OnCurrentAudioInputDeviceReceivedNone, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetCurrentAudioOutputDeviceOutputPin, const FDolbyIOAudioDevice&,
                                             Device, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetCurrentAudioOutputDevice : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets the current audio output device.
	 *
	 * Triggers On Current Audio Output Device Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Current Audio Output Device"))
	static UDolbyIOGetCurrentAudioOutputDevice* DolbyIOGetCurrentAudioOutputDevice(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetCurrentAudioOutputDevice);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioOutputDeviceOutputPin OnCurrentAudioOutputDeviceReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioOutputDeviceOutputPin OnCurrentAudioOutputDeviceReceivedNone;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentAudioOutputDeviceOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD_Two_Events(GetCurrentAudioOutputDevice, OnCurrentAudioOutputDeviceReceived,
	                                      OnCurrentAudioOutputDeviceReceivedNone);

	UFUNCTION()
	void OnCurrentAudioOutputDeviceReceivedImpl(const FDolbyIOAudioDevice& Device)
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentAudioOutputDevice, OnCurrentAudioOutputDeviceReceived,
	                                      OnCurrentAudioOutputDeviceReceived, OnCurrentAudioOutputDeviceReceivedNone,
	                                      Device, "");

	UFUNCTION()
	void OnCurrentAudioOutputDeviceReceivedNoneImpl()
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentAudioOutputDevice, OnCurrentAudioOutputDeviceReceivedNone,
	                                      OnCurrentAudioOutputDeviceReceived, OnCurrentAudioOutputDeviceReceivedNone,
	                                      {}, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD_Two_Events(GetCurrentAudioOutputDevice, OnCurrentAudioOutputDeviceReceived,
		                                   OnCurrentAudioOutputDeviceReceivedNone, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetVideoDevicesOutputPin, const TArray<FDolbyIOVideoDevice>&,
                                             Devices, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetVideoDevices : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets a list of all available video devices.
	 *
	 * Triggers On Video Devices Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Video Devices"))
	static UDolbyIOGetVideoDevices* DolbyIOGetVideoDevices(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetVideoDevices);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetVideoDevicesOutputPin OnVideoDevicesReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetVideoDevicesOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD(GetVideoDevices, OnVideoDevicesReceived);

	UFUNCTION()
	void OnVideoDevicesReceivedImpl(const TArray<FDolbyIOVideoDevice>& Devices)
	    DLB_DEFINE_IMPL_METHOD(GetVideoDevices, OnVideoDevicesReceived, Devices, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD(GetVideoDevices, OnVideoDevicesReceived, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetCurrentVideoDeviceOutputPin, const FDolbyIOVideoDevice&, Device,
                                             const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetCurrentVideoDevice : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets the current video device.
	 *
	 * Triggers On Current Video Device Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Current Video Device"))
	static UDolbyIOGetCurrentVideoDevice* DolbyIOGetVideoDevice(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetCurrentVideoDevice);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentVideoDeviceOutputPin OnCurrentVideoDeviceReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentVideoDeviceOutputPin OnCurrentVideoDeviceReceivedNone;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentVideoDeviceOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD_Two_Events(GetCurrentVideoDevice, OnCurrentVideoDeviceReceived,
	                                      OnCurrentVideoDeviceReceivedNone);

	UFUNCTION()
	void OnCurrentVideoDeviceReceivedImpl(const FDolbyIOVideoDevice& Device)
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentVideoDevice, OnCurrentVideoDeviceReceived,
	                                      OnCurrentVideoDeviceReceived, OnCurrentVideoDeviceReceivedNone, Device, "");

	UFUNCTION()
	void OnCurrentVideoDeviceReceivedNoneImpl()
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentVideoDevice, OnCurrentVideoDeviceReceivedNone,
	                                      OnCurrentVideoDeviceReceived, OnCurrentVideoDeviceReceivedNone, {}, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD_Two_Events(GetCurrentVideoDevice, OnCurrentVideoDeviceReceived,
		                                   OnCurrentVideoDeviceReceivedNone, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDolbyIOGetCurrentScreenshareSourceOutputPin,
                                             const FDolbyIOScreenshareSource&, Source, const FString&, ErrorMsg);

UCLASS()
class DOLBYIO_API UDolbyIOGetCurrentScreenshareSource : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:
	/** Gets the current screenshare source.
	 *
	 * Triggers On Current Screenshare Source Received if successful.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (BlueprintInternalUseOnly = "true", WorldContext = "WorldContextObject",
	                  DisplayName = "Dolby.io Get Current Screenshare Source"))
	static UDolbyIOGetCurrentScreenshareSource* DolbyIOGetScreenshareSource(const UObject* WorldContextObject)
	    DLB_DEFINE_CONSTRUCTOR(UDolbyIOGetCurrentScreenshareSource);

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentScreenshareSourceOutputPin OnCurrentScreenshareSourceReceived;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentScreenshareSourceOutputPin OnCurrentScreenshareSourceReceivedNone;

	UPROPERTY(BlueprintAssignable)
	FDolbyIOGetCurrentScreenshareSourceOutputPin OnError;

private:
	DLB_DEFINE_ACTIVATE_METHOD_Two_Events(GetCurrentScreenshareSource, OnCurrentScreenshareSourceReceived,
	                                      OnCurrentScreenshareSourceReceivedNone);

	UFUNCTION()
	void OnCurrentScreenshareSourceReceivedImpl(const FDolbyIOScreenshareSource& Source)
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentScreenshareSource, OnCurrentScreenshareSourceReceived,
	                                      OnCurrentScreenshareSourceReceived, OnCurrentScreenshareSourceReceivedNone,
	                                      Source, "");

	UFUNCTION()
	void OnCurrentScreenshareSourceReceivedNoneImpl()
	    DLB_DEFINE_IMPL_METHOD_Two_Events(GetCurrentScreenshareSource, OnCurrentScreenshareSourceReceivedNone,
	                                      OnCurrentScreenshareSourceReceived, OnCurrentScreenshareSourceReceivedNone,
	                                      {}, "");

	UFUNCTION()
	void OnErrorImpl(const FString& ErrorMsg)
	{
		DLB_DEFINE_ERROR_METHOD_Two_Events(GetCurrentScreenshareSource, OnCurrentScreenshareSourceReceived,
		                                   OnCurrentScreenshareSourceReceivedNone, {}, ErrorMsg);
	}

	const UObject* WorldContextObject;
};

// --------------------------------------------------------------------------------------------------------------------

UCLASS()
class UDolbyIOBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

#define DLB_EXECUTE_SUBSYSTEM_METHOD(MethodName, ...) \
	DLB_GET_SUBSYSTEM;                                \
	DolbyIOSubsystem->MethodName(__VA_ARGS__);

#define DLB_EXECUTE_RETURNING_SUBSYSTEM_METHOD(MethodName, ...)                    \
	UDolbyIOSubsystem* DolbyIOSubsystem = GetDolbyIOSubsystem(WorldContextObject); \
	if (!DolbyIOSubsystem)                                                         \
	{                                                                              \
		return {};                                                                 \
	}                                                                              \
	return DolbyIOSubsystem->MethodName(__VA_ARGS__);

public:
	/** Sets the spatial environment scale.
	 *
	 * The larger the scale, the longer the distance at which the spatial audio
	 * attenuates. To get the best experience, the scale should be set separately for each level. The default value
	 * of "1.0" means that audio will fall completely silent at a distance of 10000 units (10000 cm/100 m).
	 *
	 * @param SpatialEnvironmentScale - The scale as a floating point number.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Spatial Environment Scale"))
	static void SetSpatialEnvironmentScale(const UObject* WorldContextObject, float SpatialEnvironmentScale = 1.0f)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetSpatialEnvironmentScale, SpatialEnvironmentScale);
	}

	/** Mutes audio input. */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Mute Input"))
	static void MuteInput(const UObject* WorldContextObject)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(MuteInput);
	}

	/** Unmutes audio input. */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Unmute Input"))
	static void UnmuteInput(const UObject* WorldContextObject)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(UnmuteInput);
	}

	/** Mutes audio output. */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Mute Output"))
	static void MuteOutput(const UObject* WorldContextObject)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(MuteOutput);
	}

	/** Unmutes audio output. */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Unmute Output"))
	static void UnmuteOutput(const UObject* WorldContextObject)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(UnmuteOutput);
	}

	/** Mutes a given participant for the local user.
	 *
	 * @param ParticipantID - The ID of the remote participant to mute.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Mute Participant"))
	static void MuteParticipant(const UObject* WorldContextObject, const FString& ParticipantID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(MuteParticipant, ParticipantID);
	}

	/** Updates information about the local participant.
	 *
	 * @param UserName - The name of the participant.
	 * @param AvatarURL - The URL of the participant's avatar.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Update User Metadata"))
	static void UpdateUserMetadata(const UObject* WorldContextObject, const FString& UserName, const FString& AvatarURL)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(UpdateUserMetadata, UserName, AvatarURL);
	}

	/** Unmutes a given participant for the local user.
	 *
	 * @param ParticipantID - The ID of the remote participant to unmute.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Unmute Participant"))
	static void UnmuteParticipant(const UObject* WorldContextObject, const FString& ParticipantID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(UnmuteParticipant, ParticipantID);
	}

	/** Gets a list of all remote participants.
	 *
	 * @return An array of current Dolby.io Participant Info's.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Get Participants"))
	static TArray<FDolbyIOParticipantInfo> GetParticipants(const UObject* WorldContextObject)
	{
		DLB_EXECUTE_RETURNING_SUBSYSTEM_METHOD(GetParticipants);
	}

	/** Binds a dynamic material instance to hold the frames of the given video track. The plugin will update the
	 * material's texture parameter named "DolbyIO Frame" with the necessary data, therefore the material should
	 * have such a parameter to be usable. Automatically unbinds the material from all other tracks, but it is
	 * possible to bind multiple materials to the same track. Has no effect if the track does not exist at the
	 * moment the function is called, therefore it should usually be called as a response to the "On Video Track
	 * Added" event.
	 *
	 * @param Material - The dynamic material instance to bind.
	 * @param VideoTrackID - The ID of the video track.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Bind Material"))
	static void BindMaterial(const UObject* WorldContextObject, UMaterialInstanceDynamic* Material,
	                         const FString& VideoTrackID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(BindMaterial, Material, VideoTrackID);
	}

	/** Unbinds a dynamic material instance to no longer hold the video frames of the given video track. The plugin
	 * will no longer update the material's texture parameter named "DolbyIO Frame" with the necessary data.
	 *
	 * @param Material - The dynamic material instance to unbind.
	 * @param VideoTrackID - The ID of the video track.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Unbind Material"))
	static void UnbindMaterial(const UObject* WorldContextObject, UMaterialInstanceDynamic* Material,
	                           const FString& VideoTrackID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(UnbindMaterial, Material, VideoTrackID);
	}

	/** Gets the texture to which video from a given track is being rendered.
	 *
	 * @param VideoTrackID - The ID of the video track.
	 * @return The texture holding the video track's frame or NULL if no such texture exists.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Get Texture"))
	static class UTexture2D* GetTexture(const UObject* WorldContextObject, const FString& VideoTrackID)
	{
		DLB_EXECUTE_RETURNING_SUBSYSTEM_METHOD(GetTexture, VideoTrackID);
	}

	/** Changes the screen sharing parameters if already sharing screen.
	 *
	 * @param EncoderHint - Provides a hint to the plugin as to what type of content is being captured by the screen
	 * share.
	 * @param MaxResolution - The maximum resolution for the capture screen content to be shared as.
	 * @param DownscaleQuality - The quality for the downscaling algorithm to be used.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Change Screenshare Parameters"))
	static void ChangeScreenshareParameters(
	    const UObject* WorldContextObject,
	    EDolbyIOScreenshareEncoderHint EncoderHint = EDolbyIOScreenshareEncoderHint::Detailed,
	    EDolbyIOScreenshareMaxResolution MaxResolution = EDolbyIOScreenshareMaxResolution::ActualCaptured,
	    EDolbyIOScreenshareDownscaleQuality DownscaleQuality = EDolbyIOScreenshareDownscaleQuality::Low)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(ChangeScreenshareParameters, EncoderHint, MaxResolution, DownscaleQuality);
	}

	/** Updates the location of the listener for spatial audio purposes.
	 *
	 * Calling this function even once disables the default behavior, which is to automatically use the location of
	 * the first player controller.
	 *
	 * @param Location - The location of the listener.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Local Player Location"))
	static void SetLocalPlayerLocation(const UObject* WorldContextObject, const FVector& Location)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetLocalPlayerLocation, Location);
	}

	/** Updates the rotation of the listener for spatial audio purposes.
	 *
	 * Calling this function even once disables the default behavior, which is to automatically use the rotation of
	 * the first player controller.
	 *
	 * @param Rotation - The rotation of the listener.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Local Player Rotation"))
	static void SetLocalPlayerRotation(const UObject* WorldContextObject, const FRotator& Rotation)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetLocalPlayerRotation, Rotation);
	}

	/** Updates the location of the given remote participant for spatial audio purposes.
	 *
	 * This is only applicable when the spatial audio style of the conference is set to "Individual".
	 *
	 * Calling this function with the local participant ID has no effect. Use Set Local Player Location instead.
	 *
	 * @param ParticipantID - The ID of the remote participant.
	 * @param Location - The location of the remote participant.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Remote Player Location"))
	static void SetRemotePlayerLocation(const UObject* WorldContextObject, const FString& ParticipantID,
	                                    const FVector& Location)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetRemotePlayerLocation, ParticipantID, Location);
	}

	/** Sets what to log in the Dolby.io C++ SDK.
	 *
	 * This function should be called before the first call to Set Token if the user needs logs about the plugin's
	 * operation. Calling this function more than once has no effect.
	 *
	 * @param SdkLogLevel - Log level for SDK logs.
	 * @param MediaLogLevel - Log level for Media Engine logs.
	 * @param DvcLogLevel - Log level for DVC logs.
	 * @param bLogToConsole - Sets whether the logs should be displayed in the Output Log.
	 * @param bLogToFile - If this is true, the logs will be written to a file in the default project log directory
	 * (e.g. Saved/Logs on Windows).
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Log Settings"))
	static void SetLogSettings(const UObject* WorldContextObject, EDolbyIOLogLevel SdkLogLevel = EDolbyIOLogLevel::Info,
	                           EDolbyIOLogLevel MediaLogLevel = EDolbyIOLogLevel::Info,
	                           EDolbyIOLogLevel DvcLogLevel = EDolbyIOLogLevel::Info, bool bLogToConsole = false,
	                           bool bLogToFile = true)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetLogSettings, SdkLogLevel, MediaLogLevel, DvcLogLevel, bLogToConsole,
		                             bLogToFile);
	}

	/** Sets the audio input device.
	 *
	 * @param NativeID - The ID of the device to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Audio Input Device"))
	static void SetAudioInputDevice(const UObject* WorldContextObject, const FString& NativeID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetAudioInputDevice, NativeID);
	}

	/** Sets the audio output device.
	 *
	 * @param NativeID - The ID of the device to use.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Audio Output Device"))
	static void SetAudioOutputDevice(const UObject* WorldContextObject, const FString& NativeID)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetAudioOutputDevice, NativeID);
	}

	/** Sets the audio capture mode.
	 *
	 * @param NoiseReduction - The noise reduction mode.
	 * @param VoiceFont - The voice font.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Set Audio Capture Mode"))
	static void SetAudioCaptureMode(const UObject* WorldContextObject, EDolbyIONoiseReduction NoiseReduction,
	                                EDolbyIOVoiceFont VoiceFont)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SetAudioCaptureMode, NoiseReduction, VoiceFont);
	}

	/** Sends a message to selected participants in the current conference. The message size is limited to 16KB.
	 *
	 * @param Message - The message to send.
	 * @param ParticipantIDs - The participants to whom the message should be sent. If an empty array is provided, the
	 * message will be broadcast to all participants.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Send Message"))
	static void SendMessage(const UObject* WorldContextObject, const FString& Message,
	                        const TArray<FString>& ParticipantIDs)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SendMessage, Message, ParticipantIDs);
	}

	/** Sends a message to all participants in the current conference. The message size is limited to 16KB.
	 *
	 * This function calls Send Message with an empty array of selected participants.
	 *
	 * @param Message - The message to send.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dolby.io Comms",
	          Meta = (WorldContext = "WorldContextObject", DisplayName = "Dolby.io Broadcast Message"))
	static void BroadcastMessage(const UObject* WorldContextObject, const FString& Message)
	{
		DLB_EXECUTE_SUBSYSTEM_METHOD(SendMessage, Message, {});
	}
};

#undef DLB_EXECUTE_RETURNING_SUBSYSTEM_METHOD
#undef DLB_EXECUTE_SUBSYSTEM_METHOD
#undef DLB_DEFINE_IMPL_METHOD
#undef DLB_DEFINE_ACTIVATE_METHOD
#undef DLB_GET_SUBSYSTEM
#undef DLB_DEFINE_CONSTRUCTOR
