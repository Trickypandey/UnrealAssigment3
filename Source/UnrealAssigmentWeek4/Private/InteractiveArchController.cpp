// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractiveArchController.h"

#include "ArchMeshActor.h"

AInteractiveArchController::AInteractiveArchController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bIsVissible = false;
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

void AInteractiveArchController::BeginPlay()
{
	Super::BeginPlay();
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	SelectionWidgetInstance = CreateWidget<UOverlayWidget>(this, SelectionWidget);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("in")));
	if (SelectionWidgetInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("created")));
		SelectionWidgetInstance->AddToViewport();
		SelectionWidgetInstance->MeshBox->OnMeshAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SpawnActor);
		SelectionWidgetInstance->MaterialBox->OnMaterialAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SetMaterial);
		SelectionWidgetInstance->TextureBox->OnTextureAssetThumbnailSelected.AddDynamic(this, &AInteractiveArchController::SetTexture);
		HideVisibility();
		
	}
		
}


void AInteractiveArchController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent);

	MappingContext = NewObject<UInputMappingContext>(this);

	OnLeftClick = NewObject<UInputAction>(this);
	OnLeftClick->ValueType = EInputActionValueType::Boolean;

	OnTabClick = NewObject<UInputAction>(this);
	OnTabClick->ValueType = EInputActionValueType::Boolean;

	check(EIC)
		EIC->BindAction(OnLeftClick, ETriggerEvent::Completed, this, &AInteractiveArchController::LeftClickProcessor);
		EIC->BindAction(OnTabClick, ETriggerEvent::Completed, this, &AInteractiveArchController::HideVisibility);

	if (MappingContext) {
		MappingContext->MapKey(OnLeftClick, EKeys::LeftMouseButton);
		MappingContext->MapKey(OnTabClick, EKeys::Tab);

	}

	UEnhancedInputLocalPlayerSubsystem* SubSystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if(SubSystem)
	{
		SubSystem->AddMappingContext(MappingContext, 0);
	}
}
void AInteractiveArchController::LeftClickProcessor()
{
	float MouseX, MouseY;
	if (GetMousePosition(MouseX, MouseY)) {

		FVector WorldLocation, WorldDirection;
		if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection)) {

			auto TraceEnd = WorldLocation + WorldDirection * 10000.0f;
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;
			QueryParams.bTraceComplex = true;
			//QueryParams.AddIgnoredActor(GetPawn());

			if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, TraceEnd, ECC_Visibility, QueryParams)) {
				if (HitResult.GetActor()) {
					LastHitLocation = HitResult.Location;
					if (SelectionWidgetInstance && !SelectionWidgetInstance->IsInViewport()) {
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Clicked")));
						SelectionWidgetInstance->AddToViewport();
					}


					AArchMeshActor* ArchActor = Cast<AArchMeshActor>(HitResult.GetActor());

					if (ArchActor) {
						
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
					if (!bIsVissible)
					{
						SelectionWidgetInstance->MeshBox->SetVisibility(ESlateVisibility::Visible);
						bIsVissible = true;
						
					}

					
					//OnFLoorDetected();
				}
			}
		}
	}

}

void AInteractiveArchController::HideVisibility() {
	SelectionWidgetInstance->MeshBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidgetInstance->MaterialBox->SetVisibility(ESlateVisibility::Hidden);
	SelectionWidgetInstance->TextureBox->SetVisibility(ESlateVisibility::Hidden);
	bIsVissible = false;
}

void AInteractiveArchController::ShowMeshTextureWidget() {

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
		bIsMeshPresent = false;
	}
	else
	{
		StaticMeshActor = GetWorld()->SpawnActor<AArchMeshActor>(AArchMeshActor::StaticClass(), LastHitLocation, FRotator::ZeroRotator, SpawnParams);
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
