// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"

#include "DwarfCarry.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FThrowEvent, AActor*, actor, FVector, target);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API UDwarfCarry : public USceneComponent
{
	GENERATED_BODY()

private:
	AActor* _actor = nullptr;
	AActor* _throwable = nullptr;
	
	TSubclassOf<AActor> _throwableType;
	FThrowEvent _throwEvent;
	float _range = 100;
	bool _debug = false;

public:	
	// Sets default values for this component's properties
	UDwarfCarry();

	UFUNCTION(BlueprintCallable, Category="Initialize")
	void Initialize(TSubclassOf<AActor> throwableType, FThrowEvent throwEvent, float range = 100, bool debug = false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category="Carry")
	bool IsCarrying();

	UFUNCTION(BlueprintCallable, Category="Carry")
	bool Throw();
};
