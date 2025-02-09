// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "WeatherData.generated.h"


UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Clear		UMETA(DisplayName = "Clear"),
	Cloudy		UMETA(DisplayName = "Cloudy"),
	Rain		UMETA(DisplayName = "Rain"),
};


USTRUCT(BlueprintType)
struct FWeatherData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EWeatherType WeatherType;

	UPROPERTY(EditAnywhere)
	float CloudCoverage;

	UPROPERTY(EditAnywhere)
	float CloudScale;

	UPROPERTY(EditAnywhere)
	float Intensity;

	UPROPERTY(EditAnywhere)
	FVector4f CloudAlbedoColor;

	UPROPERTY(EditAnywhere)
	FVector4f CloudWindControls;
};