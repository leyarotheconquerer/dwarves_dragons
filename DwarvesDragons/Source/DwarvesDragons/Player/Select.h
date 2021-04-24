// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Select.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_OneParam(FSelectEvent, AActor*, actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API USelect : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USelect();

	UPROPERTY(BlueprintReadOnly, Category="Select")
	AActor* selected;

	UPROPERTY(BlueprintReadWrite, Category="Select")
	FSelectEvent selectEvent;
	
	UPROPERTY(BlueprintReadWrite, Category="Select")
	FSelectEvent unselectEvent;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Select")
	void Select();
};
