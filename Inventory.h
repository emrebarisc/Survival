// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "InteractableObject.h"

class SURVIVAL_API Inventory
{
public:
	Inventory();
	~Inventory();

	void AddToInventory(AInteractableObject* item);
	void RemoveFromInventory(AInteractableObject* item);
	void ShowInventory();
	TArray<AInteractableObject*> GetInventoryItems() { return this->_inventoryItems; }

private:
	TArray<AInteractableObject*> _inventoryItems;
	FTimerHandle InventoryTimerHandler;

	void SetPhysicsDisabled();
};
