// Copyright 2021 Hazen Johnson


#include "DwarfCarry.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDwarfCarry::UDwarfCarry()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDwarfCarry::BeginPlay()
{
	Super::BeginPlay();

	_actor = GetAttachmentRootActor();
	_throwable = GetWorld()->SpawnActor(objectType);
}


// Called every frame
void UDwarfCarry::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UDwarfCarry::IsCarrying() { return (bool)_throwable; }

bool UDwarfCarry::Throw()
{
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FHitResult hit;
	controller->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, hit);
	if (hit.IsValidBlockingHit())
	{
		const FVector location = _actor->GetActorLocation();
		const float distanceSq = FVector::DistSquared(hit.Location, location);
		const float rangeSq = range * range;
		if (distanceSq <= rangeSq && _throwable)
		{
			throwEvent.Execute(_throwable, hit.Location);
			_throwable = nullptr;
			return true;
		}
	}
	return false;
}



