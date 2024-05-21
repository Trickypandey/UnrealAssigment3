// Fill out your copyright notice in the Description page of Project Settings.


#include "AWallSpline.h"

#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
AAWallSpline::AAWallSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SplineComponent->ClearSplinePoints();
	WallMeshComponents.Empty();


	if (SplineComponent)
	{
		SetRootComponent(SplineComponent);

	}

}

void AAWallSpline::AddSplinePoint(const FVector& Location)
{
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, FString::Printf(TEXT("in CreateWall: %d"), SplineComponent->GetNumberOfSplinePoints()));
	SplineComponent->AddSplinePoint(Location, ESplineCoordinateSpace::World);
	PreviousSplinePoints.Add(Location);
	if (SplineComponent->GetNumberOfSplinePoints() > 1)
	{
		CreateWalls();
	}
}

// Called when the game starts or when spawned
void AAWallSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAWallSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAWallSpline::CreateWalls()
{
	// Clear existing wall mesh components
	for (UStaticMeshComponent* WallMeshComponent : WallMeshComponents)
	{
		if (WallMeshComponent) {
			WallMeshComponent->DestroyComponent();
		}
	}
	WallMeshComponents.Empty();

	// Create walls between each pair of spline points
	int32 NoOfSplinePoints = SplineComponent->GetNumberOfSplinePoints() - 1;
	for (int32 SplineCount = 0; SplineCount < NoOfSplinePoints; ++SplineCount)
	{
		FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
		FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);
		FVector StartTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount, ESplineCoordinateSpace::World);
		FVector EndTangent = SplineComponent->GetTangentAtSplinePoint(SplineCount + 1, ESplineCoordinateSpace::World);
		CreateWall(StartLocation, StartTangent, EndLocation, EndTangent);
	}
}

void AAWallSpline::CreateWall(const FVector& StartPos, const FVector& StartTangent, const FVector& EndPos, const FVector& EndTangent)
{

	USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	WallMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Architecture/Wall_400x200.Wall_400x200'"));
	SplineMesh->SetStaticMesh(WallMesh);
	SplineMesh->SetMobility(EComponentMobility::Movable);
	SplineMesh->RegisterComponentWithWorld(GetWorld());
	SplineMesh->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
	WallMeshComponents.Add(SplineMesh);

	SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
}


