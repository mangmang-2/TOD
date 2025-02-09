// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManagerActor.h"
#include "Components/VolumetricCloudComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/DirectionalLightComponent.h"
#include "WeatherSubsystem.h"
#include "NativeGameplayTags.h"
#include "MessageSystem/GameplayMessageSubsystem.h"
#include "MessageSystem/MessageData/WeatherMessage.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Weather_Message, "Message.Weather.Actor");
UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Player_Message);


// Sets default values
AWeatherManagerActor::AWeatherManagerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeatherManagerActor::BeginPlay()
{
	Super::BeginPlay();

    if (MPC_Global)
    {
        MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_Global);
    }

    FindDirectionalLight();

    UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
    MessageSubsystem.RegisterListener(TAG_Weather_Message, this, &ThisClass::ResponseMessage);

    BeginWeatherChange(EWeatherType::Clear);
}

// Called every frame
void AWeatherManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeatherManagerActor::BeginWeatherChange(EWeatherType WeatherType)
{
    UWeatherSubsystem* WeatherSubsystem = GetGameInstance()->GetSubsystem<UWeatherSubsystem>();
    if(WeatherSubsystem == nullptr)
        return;

    FWeatherData CurrentWeatherData = WeatherSubsystem->GetCurrentWeatherData();
    WeatherSubsystem->SetCurrentWeatherData(WeatherType);

    FWeatherData WeatherData = WeatherSubsystem->GetWeatherData(WeatherType);
    
    GetWorldTimerManager().ClearTimer(CloudCoverageTimer.TimerHandle);
	CloudCoverageTimer.StartValue = CurrentWeatherData.CloudCoverage;
	CloudCoverageTimer.TargetValue = WeatherData.CloudCoverage;
	CloudCoverageTimer.Duration = 10.0f;
	CloudCoverageTimer.ElapsedTime = 0.0f;
	GetWorldTimerManager().SetTimer(
		CloudCoverageTimer.TimerHandle,
		[this]() { UpdateCloudScalar(TEXT("CloudCoverage"), CloudCoverageTimer); },
		0.1f,
		true
	);

    GetWorldTimerManager().ClearTimer(CloudScaleTimer.TimerHandle);
    CloudScaleTimer.StartValue = CurrentWeatherData.CloudScale;
    CloudScaleTimer.TargetValue = WeatherData.CloudScale;
    CloudScaleTimer.Duration = 10.0f;
    CloudScaleTimer.ElapsedTime = 0.0f;
    GetWorldTimerManager().SetTimer(
        CloudScaleTimer.TimerHandle,
        [this]() { UpdateCloudScalar(TEXT("CloudScale"), CloudScaleTimer); },
        0.1f,
        true
    );

    GetWorldTimerManager().ClearTimer(CloudAlbedoColorTimer.TimerHandle);
    CloudAlbedoColorTimer.StartVector = CurrentWeatherData.CloudAlbedoColor;
    CloudAlbedoColorTimer.TargetVector = WeatherData.CloudAlbedoColor;
    CloudAlbedoColorTimer.Duration = 10.0f;
    CloudAlbedoColorTimer.ElapsedTime = 0.0f;
    GetWorldTimerManager().SetTimer(
        CloudAlbedoColorTimer.TimerHandle,
        [this]() { UpdateCloudScalar(TEXT("CloudAlbedoColor"), CloudAlbedoColorTimer); },
        0.1f,
        true
    );

    GetWorldTimerManager().ClearTimer(CloudWindControsTimer.TimerHandle);
    CloudWindControsTimer.StartVector = CurrentWeatherData.CloudWindControls;
    CloudWindControsTimer.TargetVector = WeatherData.CloudWindControls;
    CloudWindControsTimer.Duration = 10.0f;
    CloudWindControsTimer.ElapsedTime = 0.0f;
    GetWorldTimerManager().SetTimer(
        CloudWindControsTimer.TimerHandle,
        [this]() { UpdateCloudScalar(TEXT("CloudWindContros"), CloudWindControsTimer); },
        10.0f,
        false
    );


    GetWorldTimerManager().ClearTimer(LightTimer.TimerHandle);
	LightTimer.StartValue = CurrentWeatherData.Intensity;
	LightTimer.TargetValue = WeatherData.Intensity;
	LightTimer.Duration = 10.0f;
	LightTimer.ElapsedTime = 0.0f;

	GetWorldTimerManager().SetTimer(
		LightTimer.TimerHandle,
		[this]() { UpdateIntensity(LightTimer); },
		0.1f,
		true
	);

    UpdateRain(EWeatherType::Rain == WeatherType);;
}

void AWeatherManagerActor::UpdateCloudScalar(FName MaterialName, FTickData& TickData)
{
    if (MPC_Instance == nullptr)
    {
        GetWorldTimerManager().ClearTimer(TickData.TimerHandle);
        return;
    }

    TickData.ElapsedTime += 0.1f;
    float Alpha = TickData.ElapsedTime / TickData.Duration; // 0 ~ 1 사이 값
    float NewValue = FMath::Lerp(TickData.StartValue, TickData.TargetValue, Alpha); // 선형 보간

    MPC_Instance->SetScalarParameterValue(MaterialName, NewValue);

    if (TickData.ElapsedTime >= TickData.Duration)
    {
        GetWorldTimerManager().ClearTimer(TickData.TimerHandle); // 타이머 종료
    }
}

void AWeatherManagerActor::UpdateCloudVector(FName MaterialName, FTickData& TickData)
{
}

void AWeatherManagerActor::FindDirectionalLight()
{
    for (TObjectIterator<UDirectionalLightComponent> It; It; ++It)
    {
        if (It->GetWorld() == GetWorld()) // 현재 레벨의 컴포넌트인지 확인
        {
            DirectionalLightComponent = *It;
            return;
        }
    }
}

void AWeatherManagerActor::UpdateIntensity(FTickData& TickData)
{
    if(DirectionalLightComponent == nullptr)
        return;
    
    TickData.ElapsedTime += 0.1f;
    float Alpha = TickData.ElapsedTime / TickData.Duration; // 0 ~ 1 사이 값
    float NewValue = FMath::Lerp(TickData.StartValue, TickData.TargetValue, Alpha); // 선형 보간

    DirectionalLightComponent->SetIntensity(NewValue);

    if (TickData.ElapsedTime >= TickData.Duration)
    {
        GetWorldTimerManager().ClearTimer(TickData.TimerHandle); // 타이머 종료
    }
}

void AWeatherManagerActor::UpdateRain(bool bRain)
{
    if (bRain == false)
    {
        FPlayerMessage Message;
        Message.Verb = TAG_Player_Message;
        Message.bRain = bRain;

        UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
        MessageSystem.BroadcastMessage(Message.Verb, Message);
        return;
    }

    FTimerHandle PlayerMessageTimerHandle;
    GetWorldTimerManager().SetTimer(
        PlayerMessageTimerHandle,
        [this, bRain]()
        {
            FPlayerMessage Message;
            Message.Verb = TAG_Player_Message;
            Message.bRain = bRain;

            UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(GetWorld());
            MessageSystem.BroadcastMessage(Message.Verb, Message);
        },
        10.0f,
        false
    );
}

void AWeatherManagerActor::ResponseMessage(FGameplayTag Channel, const FWeatherMessage& Payload)
{
    BeginWeatherChange(Payload.WeatherType);
}
