// Copyright 2021 Hazen Johnson

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SplineComponent.h"

#include "Thrown.generated.h"

UDELEGATE(BlueprintAuthorityOnly)
DECLARE_DYNAMIC_DELEGATE_TwoParams(FLandEvent, FVector, location, FRotator, rotation);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DWARVESDRAGONS_API UThrown : public UActorComponent
{
	GENERATED_BODY()

private:
	UStaticMeshComponent* _mesh = nullptr;
	USplineComponent* _spline = nullptr;
	FLandEvent _landEvent;
	float _rate = 1.f;
	float _height = 30.f;
	bool _moving = false;
	float _progress = 0.f;
	bool _debug = false;

public:	
	// Sets default values for this component's properties
	UThrown();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Initialize")
	void Initialize(UStaticMeshComponent* mesh, USplineComponent* spline, FLandEvent landEvent, float height = 30.f, float rate = 1.f, bool debug = false);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category="Thrown")
	void SetupSpline(FVector target);
};
