// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "OpenableObject.h"

AOpenableObject::AOpenableObject()
{
	PrimaryActorTick.bCanEverTick = true;
	InteractableObjectStaticMesh->SetSimulatePhysics(false);

	_canBePutToInventory = false;
	_key = 0;
	_locked = false;
	_animate = false;
	_openSpeed = 200.0f;
}

void AOpenableObject::BeginPlay()
{
	Super::BeginPlay();

	initialRotationYaw = InteractableObjectStaticMesh->GetComponentRotation().Yaw;
}

void AOpenableObject::SetIsInteracted(bool _isInteracted)
{
	Super::SetIsInteracted(_isInteracted);

	_animate = true;
	_openClose = !_openClose;
}

void AOpenableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (_animate)
	{
		float openCloseSpeed;
		float objectRotationZ = InteractableObjectStaticMesh->GetComponentRotation().Yaw;

		if (_openDirection)
		{
			openCloseSpeed = _openSpeed * DeltaTime * (_openClose == true ? 1 : -1);

			InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, objectRotationZ + openCloseSpeed, 0.f));
			if (objectRotationZ < initialRotationYaw)
			{
				InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, initialRotationYaw, 0.f));
				_animate = false;
			}
			else if (objectRotationZ > initialRotationYaw + 80.f)
			{
				InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, initialRotationYaw + 80.f, 0.f));
				_animate = false;
			}
		}
		else
		{
			openCloseSpeed = _openSpeed * DeltaTime * (_openClose == true ? -1 : 1);

			InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, objectRotationZ + openCloseSpeed, 0.f));
			if (objectRotationZ > initialRotationYaw)
			{
				InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, initialRotationYaw, 0.f));
				_animate = false;
			}
			else if (objectRotationZ < initialRotationYaw - 80.f)
			{
				InteractableObjectStaticMesh->SetRelativeRotation(FRotator(0.f, initialRotationYaw - 80.f, 0.f));
				_animate = false;
			}
		}

	}
}