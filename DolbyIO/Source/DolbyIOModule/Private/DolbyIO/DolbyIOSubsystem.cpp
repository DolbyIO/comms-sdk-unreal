// Copyright 2022 Dolby Laboratories

#include "DolbyIOSubsystem.h"

#include "DolbyIO/Authenticator.h"
#include "DolbyIO/SdkAccess.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TimerManager.h"

void UDolbyIOSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	CppSdk = MakeShared<DolbyIO::FSdkAccess>(*this);
	Authenticator = MakeShared<DolbyIO::FAuthenticator>(*this);
	GameInstance = GetGameInstance();

	if (GameInstance)
	{
		GameInstance->GetTimerManager().SetTimer(SpatialUpdateTimerHandle, this,
		                                         &UDolbyIOSubsystem::UpdateViewPointUsingFirstPlayer, 0.03, true);
	}

	OnTokenNeeded.Broadcast();
}

void UDolbyIOSubsystem::SetToken(const FString& Token)
{
	CppSdk->SetToken(Token);
}
void UDolbyIOSubsystem::SetTokenUsingKeyAndSecret(const FString& AppKey, const FString& AppSecret,
                                                  int TokenExpirationTimeInSeconds)
{
	Authenticator->GetToken(AppKey, AppSecret, TokenExpirationTimeInSeconds);
}
void UDolbyIOSubsystem::Connect(const FString& ConferenceName, const FString& UserName, const FString& ExternalID,
                                const FString& AvatarURL)
{
	CppSdk->Connect(ConferenceName, UserName, ExternalID, AvatarURL);
}
void UDolbyIOSubsystem::ConnectToDemoConference()
{
	CppSdk->ConnectToDemoConference();
}
void UDolbyIOSubsystem::Disconnect()
{
	CppSdk->Disconnect();
}
void UDolbyIOSubsystem::MuteInput()
{
	CppSdk->MuteInput();
}
void UDolbyIOSubsystem::UnmuteInput()
{
	CppSdk->UnmuteInput();
}
void UDolbyIOSubsystem::MuteOutput()
{
	CppSdk->MuteOutput();
}
void UDolbyIOSubsystem::UnmuteOutput()
{
	CppSdk->UnmuteOutput();
}
void UDolbyIOSubsystem::GetAudioLevels()
{
	CppSdk->GetAudioLevels();
}
void UDolbyIOSubsystem::SetSpatialEnvironmentScale(float SpatialEnvironmentScale)
{
	CppSdk->SetSpatialEnvironmentScale(SpatialEnvironmentScale);
}

void UDolbyIOSubsystem::UpdateViewPoint(const FVector& Position, const FRotator& Rotation)
{
	if (SpatialUpdateTimerHandle.IsValid())
	{
		GameInstance->GetTimerManager().ClearTimer(SpatialUpdateTimerHandle);
	}

	CppSdk->UpdateViewPoint(Position, Rotation);
}
void UDolbyIOSubsystem::UpdateViewPointUsingFirstPlayer()
{
	if (GameInstance)
	{
		if (const auto World = GameInstance->GetWorld())
		{
			if (const auto FirstPlayerController = World->GetFirstPlayerController())
			{
				FVector Position;
				FRotator Rotation;
				FirstPlayerController->GetActorEyesViewPoint(Position, Rotation);
				CppSdk->UpdateViewPoint(Position, Rotation);
			}
		}
	}
}
