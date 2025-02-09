// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "../../Data/WeatherData.h"
#include "WeatherMessage.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeatherMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FGameplayTag Verb;

    UPROPERTY(BlueprintReadWrite)
    EWeatherType WeatherType;
};
