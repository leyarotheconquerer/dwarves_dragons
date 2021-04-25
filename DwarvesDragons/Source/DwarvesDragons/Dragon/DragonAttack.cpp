// Copyright 2021 Hazen Johnson


#include "DragonAttack.h"

#include "DwarvesDragons/Dwarf/DwarfHealth.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDragonAttack::UDragonAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDragonAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDragonAttack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_target)
	{
		FVector look = _target->GetActorLocation() - GetComponentLocation();
		_actor->SetActorRotation(look.Rotation());
	}
	// ...
}

void UDragonAttack::Initialize(AActor* actor, TSubclassOf<AActor> attack, FAttackEvent attackEvent, float range, float rate, bool debug)
{
	_actor = actor;
	_attack = attack;
	_attackEvent = attackEvent;
	_rate = rate;
	_range = range;
	_debug = debug;
}

bool UDragonAttack::Detect(AActor* other)
{
	if (other->FindComponentByClass(UDwarfHealth::StaticClass()))
	{
		_enemies.Add(other);
		if (!_target) { _target = other; }
		GetWorld()->GetTimerManager().SetTimer(_attackTimer, this, &UDragonAttack::StartAttack, _rate, false);
		return true;
	}
	return false;
}

void UDragonAttack::StartAttack()
{
	TSet<AActor*> remove;
	for(AActor* enemy : _enemies)
	{
		if (FVector::DistSquared(_actor->GetActorLocation(), enemy->GetActorLocation()) >= _range * _range)
		{
			remove.Add(enemy);
			if (_target == enemy) { _target = nullptr; }
		}
		else
		{
			if (_debug) { UE_LOG(LogTemp, Display, TEXT("%s: %s in range"), *GetName(), *enemy->GetHumanReadableName()); }
		}
	}
	for (AActor* enemy : remove) { _enemies.Remove(enemy); }
	if (_enemies.Num() > 0)
	{
		if (!_target) { _target = _enemies.Array()[0]; }
		if (_debug) { UE_LOG(LogTemp, Display, TEXT("%s: Starting attack against %d enemies"), *GetName(), _enemies.Num()); }
		_attackEvent.Execute(_target->GetActorLocation());
	}
}

void UDragonAttack::Attack()
{
	FVector location = GetComponentLocation();
	FRotator rotation = GetComponentRotation();
	GetWorld()->SpawnActor(_attack, &location, &rotation);
	GetWorld()->GetTimerManager().SetTimer(_attackTimer, this, &UDragonAttack::StartAttack, _rate, false);
}

