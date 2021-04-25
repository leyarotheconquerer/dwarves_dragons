// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DragonAttack.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FAttackEvent, FVector, target);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API UDragonAttack : public USceneComponent
{
	GENERATED_BODY()

private:
	AActor* _actor = nullptr;
	FAttackEvent _attackEvent;
	FTimerHandle _attackTimer;
	AActor* _target = nullptr;
	TSet<AActor*> _enemies = {};
	TSubclassOf<AActor> _attack;
	float _rate;
	float _range;
	bool _debug = false;

public:	
	// Sets default values for this component's properties
	UDragonAttack();

	UFUNCTION(BlueprintCallable, Category="Initialize")
	void Initialize(AActor* actor, TSubclassOf<AActor> attack, FAttackEvent attackEvent, float range, float rate = 5.f, bool debug = false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Attack")
	bool Detect(AActor* other);

	UFUNCTION(BlueprintCallable, Category="Attack")
	void Attack();

	UFUNCTION(BlueprintCallable, Category="Attack")
	bool IsAlert();

	void StartAttack();
};
