// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TodWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOD_API UTodWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct();
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

	UFUNCTION()
	void OnSliderValueChanged(float Value);

	void SetSolarTime(int32 TargetFrame);
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> TextBlock_Time;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USlider> Slider_Time;
};
