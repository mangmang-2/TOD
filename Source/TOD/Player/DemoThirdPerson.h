// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Data/WeatherData.h"
#include "DemoThirdPerson.generated.h"

UCLASS()
class TOD_API ADemoThirdPerson : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADemoThirdPerson();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ResponseMessage(struct FGameplayTag Channel, const struct FPlayerMessage& Payload);


	UFUNCTION(BlueprintImplementableEvent)
	void ActivateRain(bool bOn);
};
