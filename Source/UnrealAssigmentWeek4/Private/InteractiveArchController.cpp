// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

#include "ArchMeshActor.h"
#include "IsometricView.h"
#include "OrthographicView.h"
#include "PerspectiveView.h"

AInteractiveArchController::AInteractiveArchController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bIsVisible = false;
	Pawns.Add(CreateDefaultSubobject<APerspectiveView>(TEXT("PerspectivePawn"))->GetClass());
	Pawns.Add(CreateDefaultSubobject<AIsometricView>(TEXT("IsometricPawn"))->GetClass());
	Pawns.Add(CreateDefaultSubobject<AOrthographicView>(TEXT("OrthographicPawn"))->GetClass());

}

void AInteractiveArchController::SetMaterial(const FMaterialData& MeshData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Material")));
	StaticMeshActor->GetStaticMeshComponent()->SetMaterial(0, MeshData.Type);
}

void AInteractiveArchController::SetTexture(const FTextureData& MeshData)
{
	UMaterialInstanceDynamic* DynamicMaterial = StaticMeshActor->GetStaticMeshComponent()->CreateAndSetMaterialInstanceDynamic(0);
	if(DynamicMaterial)
	{
		// Set the texture parameter of the material
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Texture")));
		DynamicMaterial->SetTextureParameterValue(TEXT("TextureParameterName"), MeshData.Type);
	}

}



void AInteractiveArchController::BindWidgetEvents()
{
	if (SelectionWidgetInstance)
	{
		SelectionWidgetInstance->MeshBox->OnMeshAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SpawnActor);
		SelectionWidgetInstance->MaterialBox->OnMaterialAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SetMaterial);
		SelectionWidgetInstance->TextureBox->OnTextureAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SetTexture);
	}
}

void AInteractiveArchController::SetupInputBindings()
{
	if (SelectionWidget)
	{
		SelectionWidgetInstance = CreateWidget<UOverlayWidget>(this, SelectionWidget);
		if (SelectionWidgetInstance)
		{
			SelectionWidgetInstance->AddToViewport();
			// Bind widget events
			BindWidgetEvents();
			HideVisibility();
		}
	}
}

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();
	const FVector Location = FVector::ZeroVector; 
	const FRotator SpawnRotation = FRotator::ZeroRotator; 

	
	

	SetupInputBindings();
	
}


void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();
	SetupEnhancedInputBindings();
	
}
void AInteractiveArchController::LeftClickProcessor()
{
	 ;
	if (float MouseX, MouseY; GetMousePosition(MouseX, MouseY)) {

		
		if (FVector WorldLocation, WorldDirection; DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {

			auto TraceEnd = WorldLocation + WorldDirection * 10000.0f;
			
			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;


			if (FHitResult HitResult; GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams)) {
				if (HitResult.GetActor()) {
					LastHitLocation = HitResult.Location;
					if (SelectionWidgetInstance && !SelectionWidgetInstance->IsInViewport()) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Clicked")));
						SelectionWidgetInstance->AddToViewport();
					}

					if (AArchMeshActor* ArchActor = Cast<AArchMeshActor>(HitResult.GetActor())) {
						
						bIsMeshPresent = true;
						StaticMeshActor = ArchActor;
						LastHitLocation = StaticMeshActor->GetActorLocation();
						GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Actor");
						SelectionWidgetInstance->MeshBox->SetVisibility(ESlateVisibility::Visible);
						SelectionWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
						SelectionWidgetInstance->TextureBox->SetVisibility(ESlateVisibility::Visible);
					}
					else
					{
						bIsMeshPresent = false;
					}
					if (!bIsVisible)
					{
						SelectionWidgetInstance->MeshBox->SetVisibility(ESlateVisibility::Visible);
						bIsVisible = true;
						
					}

					
					//OnFLoorDetected();
				}
			}
		}
	}

}

void AInteractiveArchController::SwitchPawn()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Clicked %d"), PawnIndex));
	if (Pawns.Num() == 0)
	{
		return;
	}

	if (PawnIndex >= Pawns.Num())
	{
		PawnIndex = 0;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	TSubclassOf<APawn> PawnClass = Pawns[PawnIndex];
	if (!PawnClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector PreviousPawnLocation =FVector::ZeroVector;
	if (CurrentPawn)
	{
		PreviousPawnLocation = CurrentPawn->GetActorLocation();
		CurrentPawn->Destroy();
	}
	// Spawn the new pawn at the location of the previous one
	APawn* NewPawn = World->SpawnActor<APawn>(PawnClass, PreviousPawnLocation  + FVector(0,0,100), FRotator::ZeroRotator, SpawnParams);
	if (!NewPawn)
	{
		return;
	}

	// Destroy the previous pawn if it exists

	// Store reference to the new pawn
	CurrentPawn = NewPawn;
	// Possess the new pawn
	Possess(CurrentPawn);

	// Add the mapping context if a subsystem exists
	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		SubSystem->AddMappingContext(MappingContext, 0);
	}

	// Increment pawn index for the next switch
	PawnIndex++;
}


void AInteractiveArchController::HideVisibility() {
	SelectionWidgetInstance->MeshBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidgetInstance->TextureBox->SetVisibility(ESlateVisibility::Hidden);
	bIsVisible = false;
}

void AInteractiveArchController::ShowMeshTextureWidget() const
{

	SelectionWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Visible);
	SelectionWidgetInstance->TextureBox->SetVisibility(ESlateVisibility::Visible);
}

void AInteractiveArchController::SpawnActor(const FMeshData& MeshData)
{

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Spawn")));

	
	if (LastHitLocation.IsZero())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("LastHitLocation is invalid!"));
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	if (bIsMeshPresent)
	{
		StaticMeshActor->Destroy();
		StaticMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), LastHitLocation, FRotator::ZeroRotator, SpawnParams);
		GetPawn()->SetActorLocation(LastHitLocation);
		bIsMeshPresent = false;
	}
	else
	{
		StaticMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), LastHitLocation, FRotator::ZeroRotator, SpawnParams);
		GetPawn()->SetActorLocation(LastHitLocation);
	}

	if (StaticMeshActor)
	{
		
		StaticMeshActor->SetMobility(EComponentMobility::Movable);
		StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(MeshData.Type);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to spawn actor!"));
	}

}

void AInteractiveArchController::SetupEnhancedInputBindings()
{
	UEnhancedInputComponent* Eic = Cast<UEnhancedInputComponent>(InputComponent);

	MappingContext = NewObject<UInputMappingContext>(this);

	OnLeftClick = NewObject<UInputAction>(this);
	OnLeftClick->ValueType = EInputActionValueType::Boolean;

	OnTabClick = NewObject<UInputAction>(this);
	OnTabClick->ValueType = EInputActionValueType::Boolean;

	OnSwitchPawn = NewObject<UInputAction>(this);
	OnSwitchPawn->ValueType = EInputActionValueType::Boolean;

	check(Eic)
		Eic->BindAction(OnLeftClick, ETriggerEvent::Completed, this, &AInteractiveArchController::LeftClickProcessor);
		Eic->BindAction(OnTabClick, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisibility);
		Eic->BindAction(OnSwitchPawn, ETriggerEvent::Completed, this, &AInteractiveArchController::SwitchPawn);

	if (MappingContext) {
		MappingContext->MapKey(OnLeftClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(OnTabClick, EKeys::Tab);
		MappingContext->MapKey(OnSwitchPawn, EKeys::P);

	}

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		SubSystem->AddMappingContext(MappingContext, 0);
	}
}




