// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Survival.h"
#include "InteractableObject.h"
#include "OpenableObject.h"
#include "KeyObject.h"
#include "DamageableObject.h"
#include "SurvivalCharacter.h"
#include "Inventory.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

ASurvivalCharacter::ASurvivalCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(RootComponent);
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	SM_Tool = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SM_Tool"));
	SM_Tool->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	SM_Tool->bCastDynamicShadow = false;
	SM_Tool->CastShadow = false;
	// SM_Tool->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	SM_Tool->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(SM_Tool);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	ToolOffset = FVector(100.0f, 30.0f, 10.0f);

	SM_EmptyInspectableObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EmptyStaticMeshComponent"));
	SM_EmptyInspectableObject->SetupAttachment(FirstPersonCameraComponent);

	_playerInventory = new Inventory();

	_IsInAir = false;
	_IsCrouching = false;
	_DoingIdle1 = false;
	_DoingIdle2 = false;
	_IsLeaningDown = false;
	_IsSleeping = false;
	_IsHittingTool = false;
	_PlayerCanMove = true;
	_showInventory = false;

	_isInspecting = false;

	BaseSpeed = 150;
	RunSpeed = 300;
	
	RaycastRange = 200.f;

	_PlayerHealth = 100.f;
	_PlayerThirst = 100.f;
	_PlayerHunger = 100.f;
	_PlayerEnergy = 100.f;

	_PlayerEnergyDecreaseSpeed = 0.3f;
	_PlayerHungerDecreaseSpeed = 0.6f;
	_PlayerThirstDecreaseSpeed = 0.5f;
	_PlayerHealthDecreaseSpeed = 0.0f;
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	SM_Tool->AttachToComponent(this->GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	FirstPersonCameraComponent->AttachToComponent(GetPlayerMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("HeadCameraSocket"));
	FRotator CameraRotation(0, 0.f, 0.f);
	FVector CameraLocation(0, 0, -1.f);
	FVector CameraScale(0.1f, 0.1f, 0.1f);
	FirstPersonCameraComponent->SetRelativeLocation(CameraLocation);
	FirstPersonCameraComponent->SetRelativeRotation(CameraRotation);
	FirstPersonCameraComponent->SetRelativeScale3D(CameraScale);

	GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;

	SM_EmptyInspectableObject->SetRelativeLocation(FVector(40.f, 0, -5.f));
}

void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->_PlayerSpeed = FMath::Sqrt(FMath::Square(GetVelocity().X) + FMath::Square(GetVelocity().Y));
	this->_IsInAir = GetCharacterMovement()->IsFalling();
	
	PlayerStatControl(DeltaTime);

	/*	
		If not inspecting an object, do raycasting
		Else rotate the inspecting object
	*/

	if (!_isInspecting)
	{
		this->PerformRaycast();
	}
	else
	{
		if (_isRotatingInspectedObject) this->RotateInspectedObject(DeltaTime);
	}
}

inline void ASurvivalCharacter::PlayerStatControl(float DeltaTime)
{
	if (_PlayerEnergy > 0) _PlayerEnergy -= DeltaTime * _PlayerEnergyDecreaseSpeed;
	else _PlayerEnergy = 0;

	if (_PlayerHunger > 0) _PlayerHunger -= DeltaTime * _PlayerHungerDecreaseSpeed;
	else _PlayerHunger = 0;

	if (_PlayerThirst > 0) _PlayerThirst -= DeltaTime * _PlayerThirstDecreaseSpeed;
	else _PlayerThirst = 0;

	_PlayerHealthDecreaseSpeed = 0;

	if (_PlayerEnergy <= 50.f) _PlayerHealthDecreaseSpeed += (50.f - _PlayerEnergy) / 2000;
	else if (_PlayerHealth < 100)  _PlayerHealthDecreaseSpeed -= (50.f - _PlayerEnergy) / 2000;

	if (_PlayerHunger <= 50.f) _PlayerHealthDecreaseSpeed += (50.f - _PlayerHunger) / 2000;
	else if (_PlayerHealth < 100)  _PlayerHealthDecreaseSpeed -= (50.f - _PlayerHunger) / 2000;

	if (_PlayerThirst <= 50.f) _PlayerHealthDecreaseSpeed += (50.f - _PlayerThirst) / 2000;
	else if(_PlayerHealth < 100) _PlayerHealthDecreaseSpeed -= (50.f - _PlayerThirst) / 2000;

	if (_PlayerHealthDecreaseSpeed != 0 && _PlayerHealth >= 0 && _PlayerHealth <= 100.f)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Black, FString::Printf(TEXT("%f"), _PlayerHealthDecreaseSpeed));
		_PlayerHealth -= _PlayerHealthDecreaseSpeed;
		if (_PlayerHealth < 0) _PlayerHealth = 0;
	}
}

inline void ASurvivalCharacter::RotateInspectedObject(float DeltaTime)
{
	FVector MousePosition;
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePosition.X, MousePosition.Y);

	float x, y;

	x = DeltaTime * ((MousePositionAtInspectingBeggining.X - MousePosition.X) / 2);
	y = DeltaTime * ((-(MousePositionAtInspectingBeggining.Y - MousePosition.Y)) / 2);

	FRotator InspectRotation = FRotator(0, x, y);
	FRotator sum = InteractableObject->InteractableObjectStaticMesh->GetComponentRotation() + InspectRotation;

	if (InteractableObject) InteractableObject->InteractableObjectStaticMesh->AddLocalRotation(InspectRotation);
}

void ASurvivalCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	if(!_IsCrouching) PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	if(!_IsCrouching) PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ASurvivalCharacter::Run);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ASurvivalCharacter::StopRunning);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASurvivalCharacter::Crouch);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalCharacter::Interact);
	PlayerInputComponent->BindAction("Interact", IE_Released, this, &ASurvivalCharacter::StopRotatingInspectingObject);

	PlayerInputComponent->BindAction("StopInteraction", IE_Pressed, this, &ASurvivalCharacter::StopInteraction);

	PlayerInputComponent->BindAction("GetInteractedObject", IE_Pressed, this, &ASurvivalCharacter::GetInteractedObject);

	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &ASurvivalCharacter::ShowInventory);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &ASurvivalCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ASurvivalCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalCharacter::LookUpAtRate);
}

void ASurvivalCharacter::AddControllerPitchInput(float Val)
{
	if(!_isInspecting)
		Super::AddControllerPitchInput(Val);
}

void ASurvivalCharacter::AddControllerYawInput(float Val)
{
	if (!_isInspecting)
		Super::AddControllerYawInput(Val);
}

void ASurvivalCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && _PlayerCanMove)
	{
		_PlayerSpeed = GetCharacterMovement()->MaxWalkSpeed;

		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

	}
}

void ASurvivalCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) && _PlayerCanMove)
	{
		_PlayerSpeed = GetCharacterMovement()->MaxWalkSpeed;
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

TArray<class AInteractableObject*> ASurvivalCharacter::GetInventoryItems()
{
	return _playerInventory->GetInventoryItems();
}

Inventory* ASurvivalCharacter::GetPlayerInventory()
{
	return _playerInventory;
}

void ASurvivalCharacter::ApplySupply(float energy, float hunger, float thirst, float health)
{
	if (this->_PlayerEnergy + energy <= 100.f) this->_PlayerEnergy += energy;
	else this->_PlayerEnergy = 100.f;

	if (this->_PlayerHunger + hunger <= 100.f) this->_PlayerHunger += hunger;
	else this->_PlayerHunger = 100.f;

	if (this->_PlayerThirst + thirst <= 100.f) this->_PlayerThirst += thirst;
	else this->_PlayerThirst = 100.f;

	if (this->_PlayerHealth + health <= 100.f) this->_PlayerHealth += health;
	else this->_PlayerHealth = 100.f;
}

void ASurvivalCharacter::Run()
{
	if (_PlayerCanMove && _PlayerSpeed > 0)
	{
		_PlayerEnergyDecreaseSpeed += 1.0;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + RunSpeed;
		this->_PlayerSpeed = FMath::Sqrt(FMath::Square(GetVelocity().X) + FMath::Square(GetVelocity().Y));
	}
}

void ASurvivalCharacter::StopRunning()
{
	if (_PlayerCanMove && _PlayerSpeed > BaseSpeed)
	{
		_PlayerEnergyDecreaseSpeed -= 1.0;
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed;
		this->_PlayerSpeed = FMath::Sqrt(FMath::Square(GetVelocity().X) + FMath::Square(GetVelocity().Y));
	}
}

void ASurvivalCharacter::Crouch(void)
{
	if (!_IsInAir && !_PlayerCanMove && _IsCrouching)
	{
		MakeBoolOpposite(_IsCrouching);
		GetWorld()->GetTimerManager().SetTimer(PlayerMoveTimerHandle, this, &ASurvivalCharacter::PlayerCanMove, 1.5f);
	}
	else if(!_IsInAir && !_IsCrouching)
	{
		MakeBoolOpposite(_IsCrouching);
		MakeBoolOpposite(_PlayerCanMove);
	}
}

void ASurvivalCharacter::PlayerCanMove()
{
	MakeBoolOpposite(_PlayerCanMove);
	GetWorld()->GetTimerManager().ClearTimer(PlayerMoveTimerHandle);
}

void ASurvivalCharacter::Interact()
{
	/*
		If not inspecting, put the object in inspecting position, set inspecting conditions
	*/
	if(!_isInspecting)
	{
		if (InteractableObject)
		{
			if (InteractableObject->_canBeInspected)
			{
				InteractableObject->SetIsInteracted(!InteractableObject->GetIsInteracted());
				InteractableObject->InteractableObjectStaticMesh->SetSimulatePhysics(false);

				FVector StartPoint = this->GetFirstPersonCamera()->GetComponentLocation();
				FVector CameraForwardVector = this->GetFirstPersonCamera()->GetForwardVector();

				InteractableObjectInitialPosition = InteractableObject->GetActorLocation();
				InteractableObjectInitialRotation = InteractableObject->GetActorRotation();

				_isInspecting = true;
				_PlayerCanMove = false;

				FString HelperText = FString("Hint: Right click to put it back. ");
				HelperText.Append(_helperText.ToString());
				_helperText = FText::FromString(HelperText);

				InteractableObject->SetActorLocation(SM_EmptyInspectableObject->GetComponentLocation());
			}
			else
			{
				AOpenableObject* OpenableObject = Cast<AOpenableObject>(InteractableObject);
				if (OpenableObject)
				{
					if (OpenableObject->_locked)
					{
						if (CheckIfPlayerHasTheKey(OpenableObject->_key))
						{
							InteractableObject->SetIsInteracted(!InteractableObject->GetIsInteracted());
							OpenableObject->_locked = false;
						}
						else
						{
							FString HelperText = FString("You don't have the key for this door.");
							_helperText = FText::FromString(HelperText);
							_interactableObjectName = FText::GetEmpty();
							GetWorld()->GetTimerManager().SetTimer(PlayerGeneralTimerHandle, this, &ASurvivalCharacter::ClearHelperTexts, 3.f, false);
						}
					}
					else InteractableObject->SetIsInteracted(!InteractableObject->GetIsInteracted());
				}
			}
		}
		else if (DamageableObject)
		{
			_IsHittingTool = true;
			_PlayerCanMove = false;
		}
	}
	/*
		If inspecting, set MousePositionAtInspectingBeggining and call RotateInspectingObject function
	*/
	else
	{
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetMousePosition(MousePositionAtInspectingBeggining.X, MousePositionAtInspectingBeggining.Y);
		_isRotatingInspectedObject = true;
	}
}

void ASurvivalCharacter::StopRotatingInspectingObject()
{
	if (_isInspecting)
	{
		_isRotatingInspectedObject = false;
	}
}

void ASurvivalCharacter::StopInteraction()
{
	if (_isInspecting && InteractableObject)
	{
		_isInspecting = false;
		_PlayerCanMove = true;
		InteractableObject->SetActorLocation(InteractableObjectInitialPosition);
		InteractableObject->SetActorRotation(InteractableObjectInitialRotation);
		_helperText = FText::GetEmpty();
	}
}

void ASurvivalCharacter::GetInteractedObject()
{
	if (InteractableObject)
	{
		if (InteractableObject->_canBePutToInventory)
		{
			InteractableObject->SetActorHiddenInGame(true);
			InteractableObject->SetActorEnableCollision(false);
			InteractableObject->InteractableObjectStaticMesh->SetSimulatePhysics(false);

			_playerInventory->AddToInventory(InteractableObject);
			StopInteraction();
		}
	}
}

void ASurvivalCharacter::ShowInventory()
{
	_showInventory = !_showInventory;
	_playerInventory->ShowInventory();
}

bool ASurvivalCharacter::CheckIfPlayerHasTheKey(unsigned int key)
{
	size_t InventorySize = GetInventoryItems().Num();

	for (size_t i = 0; i < InventorySize; i++)
	{
		AKeyObject* KeyObject = Cast<AKeyObject>(GetInventoryItems()[i]);

		if (KeyObject)
		{
			if (KeyObject->GetKey() == key) return true;
		}
	}
	return false;
}

void ASurvivalCharacter::ClearHelperTexts()
{
	_helperText = FText::GetEmpty();
	_interactableObjectName = FText::GetEmpty();
	GetWorld()->GetTimerManager().ClearTimer(PlayerGeneralTimerHandle);
}

void ASurvivalCharacter::MakeBoolOpposite(bool & Bool)
{
	Bool = !Bool;
}

void ASurvivalCharacter::PerformRaycast()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartPoint = FirstPersonCameraComponent->GetComponentLocation();
	FVector CameraForwardVector = FirstPersonCameraComponent->GetForwardVector();
	FVector EndPoint = StartPoint + (CameraForwardVector * RaycastRange);

	FCollisionQueryParams* CollisionParams = new FCollisionQueryParams();
	CollisionParams->AddIgnoredActor(this);
	CollisionParams->AddIgnoredComponent(this->GetMesh());

	InteractableObject = NULL;
	DamageableObject = NULL;

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartPoint, EndPoint, ECC_Visibility, *CollisionParams))
	{

		TArray<UStaticMeshComponent*> Components;

		AInteractableObject* HitInteractableObject = Cast<AInteractableObject>(HitResult->GetActor());
		ADamageableObject* HitDamageableObject = Cast<ADamageableObject>(HitResult->GetActor());
		
		if (HitInteractableObject)
		{
			InteractableObject = HitInteractableObject;
			HitInteractableObject->GetComponents(Components);

			if (_helperText.IsEmpty()) _helperText = InteractableObject->_helpText;
			_interactableObjectName = InteractableObject->_interactableObjectName;
		}
		else if (HitDamageableObject)
		{
			DamageableObject = HitDamageableObject;
		}

		if (!HitInteractableObject && !HitDamageableObject)
		{
			_helperText = FText::GetEmpty();
			_interactableObjectName = FText::GetEmpty();
		}
	}
	else
	{
		InteractableObject = nullptr;
		DamageableObject = nullptr;
		_helperText = FText::GetEmpty();
		_interactableObjectName = FText::GetEmpty();
	}
}