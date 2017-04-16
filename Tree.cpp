// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "Tree.h"

ATree::ATree() : ADamageableObject(200.f)
{
	TreeStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreeStaticMeshComponent"));
	TreeSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TreeSkeletalMeshComponent"));
}


void ATree::BeginPlay()
{
}

void ATree::FullyDamaged()
{

}