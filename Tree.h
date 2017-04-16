// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DamageableObject.h"
#include "Tree.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVAL_API ATree : public ADamageableObject
{
	GENERATED_BODY()
	
public:
	ATree();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* TreeSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* TreeStaticMesh;

private:

	virtual void FullyDamaged() override;
	
};
