// Fill out your copyright notice in the Description page of Project Settings.


#include "DemoThirdPerson.h"
#include "NativeGameplayTags.h"
#include "../MessageSystem/GameplayMessageSubsystem.h"
#include "../MessageSystem/MessageData/WeatherMessage.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_Player_Message, "Message.Player");

// Sets default values
ADemoThirdPerson::ADemoThirdPerson()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADemoThirdPerson::BeginPlay()
{
	Super::BeginPlay();

	UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
	MessageSubsystem.RegisterListener(TAG_Player_Message, this, &ThisClass::ResponseMessage);
}

// Called every frame
void ADemoThirdPerson::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADemoThirdPerson::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADemoThirdPerson::ResponseMessage(FGameplayTag Channel, const FPlayerMessage& Payload)
{
	ActivateRain(Payload.bRain);
}