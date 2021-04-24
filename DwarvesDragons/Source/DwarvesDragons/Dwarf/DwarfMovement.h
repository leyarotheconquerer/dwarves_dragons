// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DwarfMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API UDwarfMovement : public USceneComponent
{
	GENERATED_BODY()

private:
	APawn* _pawn = nullptr;
	AActor* _target = nullptr;
	TArray<FVector> _path;
	float _rate = 1;
	bool _debug = false;
	
public:	
	// Sets default values for this component's properties
	UDwarfMovement();

	UFUNCTION(BlueprintCallable, Category="Initialize")
	void Initialize(APawn* pawn, AActor* target, float movementRate, bool debug = false);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Movement")
	void RecalculatePath(UWorld* world);
};
