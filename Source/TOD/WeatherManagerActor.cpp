// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherManagerActor.h"
#include "Components/VolumetricCloudComponent.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Components/DirectionalLightComponent.h"

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
        // UMaterialParameterCollectionInstance 가져오기 (MPC를 변경하려면 꼭 필요)
        MPC_Instance = GetWorld()->GetParameterCollectionInstance(MPC_Global);
    }
    FindDirectionalLight();
    BeginCloudy();
}

// Called every frame
void AWeatherManagerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeatherManagerActor::BeginCloudy()
{
    if (MPC_Instance)
    {
        CloudyTimer.StartValue = -0.2f;
        CloudyTimer.TargetValue = 0.4f;
        CloudyTimer.Duration = 10.0f;
        CloudyTimer.ElapsedTime = 0.0f;
        GetWorldTimerManager().SetTimer(
            CloudyTimer.TimerHandle,
            [this]() { UpdateCloudCoverage(CloudyTimer); },
            0.1f,
            true
        );

        LightTimer.StartValue = 4.0f;
        LightTimer.TargetValue = 1.0f;
        LightTimer.Duration = 10.0f;
        LightTimer.ElapsedTime = 0.0f;

        GetWorldTimerManager().SetTimer(
            LightTimer.TimerHandle,
            [this]() { UpdateIntensity(LightTimer); },
            0.1f,
            true
        );
    }
}

void AWeatherManagerActor::UpdateCloudCoverage(FTickData& TickData)
{
    if (MPC_Instance == nullptr)
    {
        GetWorldTimerManager().ClearTimer(TickData.TimerHandle);
        return;
    }

    TickData.ElapsedTime += 0.1f;
    float Alpha = TickData.ElapsedTime / TickData.Duration; // 0 ~ 1 사이 값
    float NewValue = FMath::Lerp(TickData.StartValue, TickData.TargetValue, Alpha); // 선형 보간

    MPC_Instance->SetScalarParameterValue(TEXT("CloudCoverage"), NewValue);

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
