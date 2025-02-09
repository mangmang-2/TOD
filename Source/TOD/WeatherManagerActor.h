// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Data/WeatherData.h"
#include "WeatherManagerActor.generated.h"

USTRUCT()
struct FTickData
{
	GENERATED_BODY()

	FTimerHandle TimerHandle;
	float StartValue;
	FVector4f StartVector;
	float TargetValue;
	FVector4f TargetVector;
	float Duration;
	float ElapsedTime;
};

UCLASS()
class TOD_API AWeatherManagerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeatherManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, Category = "MPC")
	TObjectPtr<class UMaterialParameterCollection> MPC_Global;

	class UMaterialParameterCollectionInstance* MPC_Instance;
	
	FTickData CloudCoverageTimer;
	FTickData CloudScaleTimer;
	FTickData CloudAlbedoColorTimer;
	FTickData CloudWindControsTimer;
	FTickData LightTimer;
public:

	void BeginWeatherChange(EWeatherType WeatherType);
	
	UFUNCTION()
	void UpdateCloudScalar(FName MaterialName, FTickData& TickData);

		UFUNCTION()
	void UpdateCloudVector(FName MaterialName, FTickData& TickData);

	void FindDirectionalLight();

	UFUNCTION()
	void UpdateIntensity(FTickData& TickData);

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UDirectionalLightComponent> DirectionalLightComponent;


	void ResponseMessage(struct FGameplayTag Channel, const struct FWeatherMessage& Payload);
};
