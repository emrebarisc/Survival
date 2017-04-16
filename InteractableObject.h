// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "GameFramework/Actor.h"
#include "InteractableObject.generated.h"

UCLASS()
class SURVIVAL_API AInteractableObject : public AActor
{
	GENERATED_BODY()

public:	
	AInteractableObject();
	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
	virtual void Use();

	UFUNCTION(BlueprintCallable, Category = "Interactable Object")
	virtual void Drop();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* InteractableObjectStaticMesh;
	
	bool GetIsInteracted();

	virtual void SetIsInteracted(bool newIsInteracted);

	UPROPERTY(BlueprintReadWrite, Category = Animation)
	bool IsInteracted;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable Object")
	FText _helpText;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable Object")
	FText _interactableObjectName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable Object")
	bool _canBePutToInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable Object")
	bool _canBeInspected;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interactable Object")
	UTexture2D* _thumbnail;

private:
	FTimerHandle InteractableObjectTimerHandler;

	void SetPhysicsEnabled();
};
