// Copyright 2021 Hazen Johnson


#include "DwarfMovement.h"



#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
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

void UDwarfMovement::Initialize(ACharacter* character, TSubclassOf<AActor> targetType, float movementRate, bool debug)
{
	this->_character = character;
	this->rate = movementRate;
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
		UE_LOG(LogTemp, Error, TEXT("%s: Unable to find target"), *_character->GetHumanReadableName());
	}
}


void UDwarfMovement::RecalculatePath(UWorld* world)
{
	if (_character)
	{
		auto path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), _character->GetActorLocation(), _target);
		_path = path->PathPoints;
		_index = 1;
	}
}


// Called every frame
void UDwarfMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_character)
	{
		// const FVector movement = _character->ConsumeMovementInputVector();
		// FHitResult hit;
		// _character->AddActorWorldOffset(movement, true, &hit, ETeleportType::TeleportPhysics);
		if (_path.Num() > _index)
		{
			const FVector next = _path[_index];
			const FVector location = _character->GetActorLocation();
			if (FVector::DistSquared(location, next) < 8000.0f)
			{
				_index++;
			}
			FVector direction = next - location; direction.Normalize(); direction.Z = 0.f;
			_character->AddMovementInput(direction, rate * DeltaTime);
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

		FVector location = _character->GetActorLocation() + FVector(0, 0, 10.f);
		FVector velocity = (_character->GetPendingMovementInputVector() * 100.f) + location;
		DrawDebugLine(GetWorld(), location, velocity, FColor(0, 255.f, 0));
	}

}

