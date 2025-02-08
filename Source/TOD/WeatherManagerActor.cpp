// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManagerActor.h"
#include "Components/VolumetricCloudComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/DirectionalLightComponent.h"
#include "WeatherSubsystem.h"

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
    //BeginCloudy();
}

// Called every frame
void AWeatherManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AWeatherManagerActor::BeginCloudy()
{
    UWeatherSubsystem* WeatherSubsystem = GetGameInstance()->GetSubsystem<UWeatherSubsystem>();
    if(WeatherSubsystem == nullptr)
        return;

    FWeatherData CurrentWeatherData = WeatherSubsystem->GetCurrentWeatherData();
    FWeatherData WeatherData = WeatherSubsystem->GetWeatherData(EWeatherType::Rain);
    
	CloudCoverageTimer.StartValue = CurrentWeatherData.CloudCoverage;
	CloudCoverageTimer.TargetValue = WeatherData.CloudCoverage;
	CloudCoverageTimer.Duration = 10.0f;
	CloudCoverageTimer.ElapsedTime = 0.0f;
	GetWorldTimerManager().SetTimer(
		CloudCoverageTimer.TimerHandle,
		[this]() { UpdateCloud(TEXT("CloudCoverage"), CloudCoverageTimer); },
		0.1f,
		true
	);

    //CloudScaleTimer.StartValue = CurrentWeatherData.CloudScale;
    //CloudScaleTimer.TargetValue = WeatherData.CloudScale;
    //CloudScaleTimer.Duration = 10.0f;
    //CloudScaleTimer.ElapsedTime = 0.0f;
    //GetWorldTimerManager().SetTimer(
    //    CloudScaleTimer.TimerHandle,
    //    [this]() { UpdateCloud(TEXT("CloudScale"), CloudScaleTimer); },
    //    0.1f,
    //    true
    //);

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
}

void AWeatherManagerActor::UpdateCloud(FName MaterialName, FTickData& TickData)
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
