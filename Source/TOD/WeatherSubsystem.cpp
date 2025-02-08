// Fill out your copyright notice in the Description page of Project Settings.


#include "WeatherSubsystem.h"

void UWeatherSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
    InitData();
}


void UWeatherSubsystem::InitData()
{
    UDataTable* DataTablePointer = LoadObject<UDataTable>(nullptr, TEXT("/Game/TOD/Data/DT_WeatherData.DT_WeatherData"));
    if (DataTablePointer != nullptr)
    {
        TArray<FWeatherData*> AllRows;
        DataTablePointer->GetAllRows(nullptr, AllRows);

        for (const auto& Data : AllRows)
        {
            WeatherDataList.Add(*Data);
        }
    }
}

FWeatherData UWeatherSubsystem::GetWeatherData(EWeatherType WeatherType)
{
    for (const auto& WeatherData : WeatherDataList)
    {
        if (WeatherData.WeatherType == WeatherType)
        {
            return WeatherData;
        }
    }
    return FWeatherData();
}

FWeatherData UWeatherSubsystem::GetCurrentWeatherData()
{
    return GetWeatherData(CurrentWeatherType);
}
