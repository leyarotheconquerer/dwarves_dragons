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

}


// Called every frame
void UDwarfCarry::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_debug)
	{
		DrawDebugCylinder(GetWorld(),
			_actor->GetActorLocation(),
			_actor->GetActorLocation() + FVector(0, 0, 10.f),
			_range,
			32,
			FColor(255.f, 0, 0)
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
	_actor = GetAttachmentRootActor();
	_throwable = GetWorld()->SpawnActor(_throwableType);
	if (_throwable)
	{
		_throwable->AttachToComponent(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	}
}


bool UDwarfCarry::IsCarrying() { return (bool)_throwable; }

bool UDwarfCarry::Throw()
{
	if (_throwable)
	{
		UE_LOG(LogTemp, Display, TEXT("%s: Trying to throw"), *GetName());
		auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		FHitResult hit;
		controller->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, false, hit);
		if (hit.IsValidBlockingHit())
		{
			UE_LOG(LogTemp, Display, TEXT("%s: Was valid hit"), *GetName());
			const FVector location = _actor->GetActorLocation();
			const float distanceSq = FVector::DistSquared(hit.Location, location);
			const float rangeSq = _range * _range;
			if (distanceSq <= rangeSq)
			{
				UE_LOG(LogTemp, Display, TEXT("%s: Was in range"), *GetName());
				_throwable->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
				_throwEvent.Execute(_throwable, hit.Location);
				_throwable = nullptr;
				return true;
			}
		}
	}
	return false;
}



