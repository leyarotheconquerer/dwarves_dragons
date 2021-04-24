// Copyright 2021 Hazen Johnson


#include "DwarfHealth.h"

// Sets default values for this component's properties
UDwarfHealth::UDwarfHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDwarfHealth::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = TotalHealth;
}


// Called every frame
void UDwarfHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!dead && CurrentHealth <= 0) {
		dead = true;
		if (!Die.ExecuteIfBound())
		{
			UE_LOG(LogTemp, Warning, TEXT("Nothing bound to the die delegate"));
		}
	}
}

void UDwarfHealth::TakeDamage(int damage)
{
	CurrentHealth -= damage;
	if (CurrentHealth < 0) { CurrentHealth = 0; }
}


