// Copyright 2021 Hazen Johnson


#include "DwarfCarry.h"


#include "DrawDebugHelpers.h"
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
	_throwable = GetWorld()->SpawnActor(_throwableType);
}


// Called every frame
void UDwarfCarry::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_debug)
	{
		DrawDebugCircle(GetWorld(),
			_actor->GetActorLocation(),
			_range,
			32,
			FColor(255.f, 0, 0),
			false, -1, 0, 0,
			FVector(0, 1.f, 0.f),
			FVector(0, 0.f, 1.f)
		);
	}

	// ...
}

void UDwarfCarry::Initialize(TSubclassOf<AActor> throwableType, FThrowEvent throwEvent, float range, bool debug)
{
	_throwableType = throwableType;
	_throwEvent = throwEvent;
	_range = range;
	_debug = debug;
}


bool UDwarfCarry::IsCarrying() { return (bool)_throwable; }

bool UDwarfCarry::Throw()
{
	if (_throwable)
	{
		auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, hit);
		if (hit.IsValidBlockingHit())
		{
			const FVector location = _actor->GetActorLocation();
			const float distanceSq = FVector::DistSquared(hit.Location, location);
			const float rangeSq = _range * _range;
			if (distanceSq <= rangeSq)
			{
				_throwEvent.Execute(_throwable, hit.Location);
				_throwable = nullptr;
				return true;
			}
		}
	}
	return false;
}



