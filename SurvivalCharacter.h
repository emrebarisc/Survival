// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "SurvivalCharacter.generated.h"

UCLASS(config=Game)
class ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	/*UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* PlayerMesh;*/

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* SM_Tool;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* FP_MuzzleLocation;


public:
	ASurvivalCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector ToolOffset;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint32 bUsingMotionControllers : 1;

	virtual void BeginPlay();
	virtual void Tick(float DeltaTime);

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	FORCEINLINE class USkeletalMeshComponent* GetPlayerMesh() const { return GetMesh(); }
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCameraComponent; }

public:
	UPROPERTY(BlueprintReadWrite, Category = Animation)
	float _PlayerSpeed;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _IsInAir;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _IsCrouching;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _DoingIdle1;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _DoingIdle2;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _IsLeaningDown;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _IsSleeping;

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool _IsHittingTool;

	UPROPERTY(BlueprintReadWrite, Category = "InGame")
	bool _showInventory;


	UPROPERTY(BlueprintReadWrite, Category = "InGame")
	bool _isInspecting;

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	float GetPlayerHealth() { return this->_PlayerHealth; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	float GetPlayerThirst() { return this->_PlayerThirst; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	float GetPlayerHunger() { return this->_PlayerHunger; }

	UFUNCTION(BlueprintCallable, Category = "PlayerStats")
	float GetPlayerEnergy() { return this->_PlayerEnergy; }

	UFUNCTION(BlueprintCallable, Category = "Helpers")
	FText GetHelperText() { return this->_helperText; }

	UFUNCTION(BlueprintCallable, Category = "Helpers")
	FText GetInteractableObjectName() { return this->_interactableObjectName; }

	UFUNCTION(BlueprintCallable, Category = "InGame")
	TArray<class AInteractableObject*> GetInventoryItems();

public:
	void ApplySupply(float energy, float hunger, float thirst, float health);

	class Inventory* GetPlayerInventory();

private:

	void AddControllerPitchInput(float Val) override;
	void AddControllerYawInput(float Val) override;

	void Run();
	void StopRunning();

	void Crouch(void);

	void MakeBoolOpposite(bool &Bool);

	void PerformRaycast();
	void PlayerCanMove();
	
	void Interact();
	void StopRotatingInspectingObject();

	UFUNCTION(BlueprintCallable, Category = "InGame")
	void StopInteraction();

	void ShowInventory();

	bool CheckIfPlayerHasTheKey(unsigned int key);
	void ClearHelperTexts();

	UFUNCTION(BlueprintCallable, Category = "InGame")
	void GetInteractedObject();

	void PlayerStatControl(float DeltaTime);
	void RotateInspectedObject(float DeltaTime);

	////////////////////////////////////////////////

	bool _PlayerCanMove;
	bool _IsRunning;
	bool _isRotatingInspectedObject;
		
	float RunSpeed;
	float BaseSpeed;
	float Damage;

	float RaycastRange;

	float _PlayerHealth;
	float _PlayerThirst;
	float _PlayerHunger;
	float _PlayerEnergy;

	float _PlayerEnergyDecreaseSpeed;
	float _PlayerHungerDecreaseSpeed;
	float _PlayerThirstDecreaseSpeed;
	float _PlayerHealthDecreaseSpeed;

	class Inventory* _playerInventory;

	UStaticMeshComponent* SM_EmptyInspectableObject;

	FText _helperText;
	FText _interactableObjectName;

	FVector MousePositionAtInspectingBeggining;
	FVector InteractableObjectInitialPosition;
	FRotator InteractableObjectInitialRotation;
	
	////////////////////////////////////////////////

	FTimerHandle PlayerMoveTimerHandle;
	FTimerHandle PlayerGeneralTimerHandle;
	class AInteractableObject* InteractableObject;
	class ADamageableObject* DamageableObject;
};

