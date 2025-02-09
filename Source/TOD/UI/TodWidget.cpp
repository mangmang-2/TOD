// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TodWidget.h"
#include "TodWidget.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "LevelSequenceActor.h"
#include <Kismet/GameplayStatics.h>
#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence.h"

void UTodWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (TextBlock_Time)
    {
        FDateTime Now = FDateTime::Now();
        FString TimeString = TEXT("Time : ") + Now.ToString(TEXT("%H:%M"));

        TextBlock_Time->SetText(FText::FromString(TimeString));
    }

    if (Slider_Time)
    {
        FDateTime Now = FDateTime::Now();
        int32 CurrentMinute = Now.GetHour() * 60 + Now.GetMinute();
        Slider_Time->SetValue(static_cast<float>(CurrentMinute) / 1440.0f);
    }

    if (Slider_Time)
    {
        Slider_Time->OnValueChanged.AddDynamic(this, &UTodWidget::OnSliderValueChanged);
    }

}

void UTodWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	

}

void UTodWidget::OnSliderValueChanged(float Value)
{
    if (TextBlock_Time)
    {
        int32 TotalMinutes = FMath::RoundToInt(Value * 1440.0f);

        int32 Hours = TotalMinutes / 60;
        int32 Minutes = TotalMinutes % 60;

        FString TimeString = TEXT("Time : ") + FString::Printf(TEXT("%02d:%02d"), Hours, Minutes);
        TextBlock_Time->SetText(FText::FromString(TimeString));
        
        SetSolarTime(TotalMinutes);
    }
}

void UTodWidget::SetSolarTime(int32 TargetFrame)
{
    ALevelSequenceActor* SequenceActor = Cast<ALevelSequenceActor>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ALevelSequenceActor::StaticClass())
    );

    if (SequenceActor && SequenceActor->SequencePlayer)
    {
        int32 TotalFrames = SequenceActor->SequencePlayer->GetFrameDuration();

        FFrameTime FrameTime(TargetFrame);
        FMovieSceneSequencePlaybackParams PlaybackParams(FrameTime, EUpdatePositionMethod::Jump);
        SequenceActor->SequencePlayer->SetPlaybackPosition(PlaybackParams);
    }
}
