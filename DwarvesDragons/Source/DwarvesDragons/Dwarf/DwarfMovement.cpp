// Copyright 2021 Hazen Johnson


#include "DwarfMovement.h"



#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDwarfMovement::UDwarfMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDwarfMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UDwarfMovement::Initialize(APawn* pawn, TSubclassOf<AActor> targetType, float movementRate, bool debug)
{
	this->_pawn = pawn;
	this->_rate = movementRate;
	this->_debug = debug;
	TArray<AActor*> targets;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), targetType, targets);
	for(auto target : targets)
	{
		this->_target = target;
		break;
	}
	if (!_target)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Unable to find target"), *_pawn->GetHumanReadableName());
	}
}


void UDwarfMovement::RecalculatePath(UWorld* world)
{
	if (_pawn)
	{
		auto path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), _pawn->GetActorLocation(), _target);
		this->_path = path->PathPoints;
	}
}


// Called every frame
void UDwarfMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_pawn)
	{
		const FVector movement = _pawn->ConsumeMovementInputVector();
		_pawn->AddActorWorldOffset(movement, true);
		if (_path.Num() > 1)
		{
			const FVector next = _path[1];
			const FVector location = _pawn->GetActorLocation();
			FVector direction = next - location; direction.Normalize();
			_pawn->AddMovementInput(direction, _rate * DeltaTime);
		}
	}

	if (_debug)
	{
		for (int i = 0; i < _path.Num() - 1; ++i)
		{
			const FVector current = _path[i] + FVector(0, 0, 10.f);
			const FVector next = _path[i+1] + FVector(0, 0, 10.f);
			DrawDebugLine(GetWorld(), current, next, FColor(255.0, 0, 0));
		}

		FVector location = _pawn->GetActorLocation() + FVector(0, 0, 10.f);
		FVector velocity = (_pawn->GetPendingMovementInputVector() * 100.f) + location;
		DrawDebugLine(GetWorld(), location, velocity, FColor(0, 255.f, 0));
	}

}

