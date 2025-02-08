// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/WeatherData.h"
#include "WeatherSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TOD_API UWeatherSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection);
	void InitData();

	FWeatherData GetWeatherData(EWeatherType WeatherType);
	FWeatherData GetCurrentWeatherData();
protected:
	TArray<FWeatherData> WeatherDataList;
	EWeatherType CurrentWeatherType;
};
