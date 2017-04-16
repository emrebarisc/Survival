// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "DayTimeManager.h"


// Sets default values
ADayTimeManager::ADayTimeManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LastTimeOfTheDay = 0;
	TimeSinceLastIncrement = 0;

	DirectionalLightComponent = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("DayLightComponent"));
	DirectionalLightComponent->SetupAttachment(RootComponent);
	DirectionalLightComponent->Intensity = 2.f;
}

// Called when the game starts or when spawned
void ADayTimeManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADayTimeManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	static float PitchRotation = 0;

	if (DirectionalLightComponent)
	{
		/* TimeOfDay is expressed in minutes, we need to convert this into a pitch rotation */
		const float MinutesInDay = 24 * 60;
		const float PitchOffset = 270; /* The offset to account for time of day 0 should equal midnight */
		PitchRotation += 360 * (DeltaTime / MinutesInDay) * 20;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("%f\n"), PitchRotation));

		FRotator NewSunRotation = FRotator(PitchRotation + PitchOffset, 45.f, 0);
		DirectionalLightComponent->SetRelativeRotation(NewSunRotation);
	}
}

