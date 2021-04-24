// Copyright 2021 Hazen Johnson


#include "Select.h"


#include "DwarvesDragons/Dwarf/DwarfCarry.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
USelect::USelect()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USelect::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USelect::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USelect::Select()
{
	APlayerController* controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FHitResult hit;
	if (selected)
	{
		unselectEvent.Execute(selected);
		selected = nullptr;
	}
	if (controller->GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn, true, hit))
	{
		AActor* target = hit.GetActor();
		TArray<UDwarfCarry*> carry;
		target->GetComponents(carry);
		UE_LOG(LogTemp, Display, TEXT("%s: Found an actor (%s)"), *GetName(), *target->GetHumanReadableName());
		if (carry.Num() > 0 && carry[0]->IsCarrying())
		{
			selected = target;
			selectEvent.Execute(selected);
			UE_LOG(LogTemp, Display, TEXT("%s: Found a dwarf to select (%s)"), *GetName(), *target->GetHumanReadableName());
		}
	}
}
