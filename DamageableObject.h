// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "DamageableObject.generated.h"

UCLASS()
class SURVIVAL_API ADamageableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageableObject();

	ADamageableObject(float Health);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	float GetHealth() { return _Health; }
	void Damage(float);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	UStaticMeshComponent* DamageableObjectMesh;
	
private:
	float _Health;


	virtual void FullyDamaged();
	
};
