// Fill out your copyright notice in the Description page of Project Settings.

#include "Survival.h"
#include "Inventory.h"

Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::AddToInventory(AInteractableObject* item)
{
	_inventoryItems.Add(item);
	//GetWorld()->GetTimerManager().SetTimer(InventoryTimerHandler, this, &Inventory::SetPhysicsDisabled, 0.001f);
}

void Inventory::RemoveFromInventory(AInteractableObject* item)
{
	_inventoryItems.Remove(item);
}

void Inventory::ShowInventory()
{
	size_t itemCount = _inventoryItems.Num();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("%d"), itemCount));
	for (size_t i = 0; i < itemCount; i++)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString(_inventoryItems[i]->_interactableObjectName.ToString()));
	}
}

void Inventory::SetPhysicsDisabled()
{
	//item->InteractableObjectStaticMesh->SetSimulatePhysics(false);
	//GetWorld()->GetTimerManager().ClearTimer(InventoryTimerHandler);
}
