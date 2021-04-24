// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DwarfHealth.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE(FDieEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API UDwarfHealth : public UActorComponent
{
	GENERATED_BODY()

private:
	bool dead = false;

public:	
	// Sets default values for this component's properties
	UDwarfHealth();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Health")
	int TotalHealth = 100;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Health")
	int CurrentHealth;

	UPROPERTY(BlueprintReadWrite, Category="Health")
	FDieEvent Die;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Health")
	void TakeDamage(int damage);
};
