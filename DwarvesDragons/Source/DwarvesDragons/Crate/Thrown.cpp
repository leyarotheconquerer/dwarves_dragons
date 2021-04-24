// Copyright 2021 Hazen Johnson


#include "Thrown.h"

#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UThrown::UThrown()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UThrown::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UThrown::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (_moving && _spline && _mesh)
	{
		_progress += DeltaTime * _rate;
		_progress = _progress <= 1.f ? _progress : 1.f;
		_mesh->SetWorldLocation(
			_spline->GetLocationAtDistanceAlongSpline(
				_progress * _spline->GetSplineLength(),
				ESplineCoordinateSpace::World
			),
			false
		);
		if (_progress >= 1.f)
		{
			_landEvent.Execute(_mesh->GetComponentLocation(), _mesh->GetComponentRotation());
			_moving = false;
		}
	}
	// ...
}

void UThrown::Initialize(UStaticMeshComponent* mesh, USplineComponent* spline, FLandEvent landEvent, float height, float rate, bool debug)
{
	_mesh = mesh;
	_spline = spline;
	_height = height;
	_rate = rate;
	_debug = debug;
	_landEvent = landEvent;
}

void UThrown::SetupSpline(FVector target)
{
	FVector location = _mesh->GetComponentLocation();
	FVector apex = (target - location) / 2 + location + FVector(0, 0, _height);
	TArray<FVector> points = { location, apex, target };
	_spline->SetSplinePoints(points, ESplineCoordinateSpace::World);
	if (_debug)
	{
		for(auto point : points)
		{
			DrawDebugSphere(GetWorld(), point, 10, 12, FColor(100.f, 100.f, 255.f), false, 10.f);
		}
		_spline->SetDrawDebug(true);
	}
	_moving = true;
	_progress = 0.f;
}


