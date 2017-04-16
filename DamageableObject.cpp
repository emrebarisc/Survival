// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "DamageableObject.h"


ADamageableObject::ADamageableObject()
{
	PrimaryActorTick.bCanEverTick = false;
	DamageableObjectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Damageable Object Mesh"));

}

ADamageableObject::ADamageableObject(float Health) 
	: _Health(Health)
{
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADamageableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADamageableObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void ADamageableObject::Damage(float DamageAmount)
{
	this->_Health -= DamageAmount;

	if (_Health <= 0) FullyDamaged();
}

void ADamageableObject::FullyDamaged()
{

}
