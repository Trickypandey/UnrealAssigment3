// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchMeshActor.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "OverlayWidget.h"
#include "InteractiveArchController.generated.h"

/**
 * 
 */
UCLASS()
class UNREALASSIGMENTWEEK4_API AInteractiveArchController : public APlayerController
{
	GENERATED_BODY()
public:
	AInteractiveArchController();
	UFUNCTION()
	void SetMaterial(const FMaterialData& MeshData);
	UFUNCTION()
	void SetTexture(const FTextureData& MeshData);
	void BindWidgetEvents();
	void SetupInputBindings();
	virtual void BeginPlay() override;

	void LeftClickProcessor();
	void SwitchPawn();
	void HideVisibility();
	void ShowMeshTextureWidget() const;
	

	UFUNCTION()
	void SpawnActor(const FMeshData& MeshData);
protected:
	void SetupEnhancedInputBindings();
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnLeftClick;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnTabClick;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnSwitchPawn;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputMappingContext* MappingContext;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	UOverlayWidget* SelectionWidgetInstance;

	/*UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;*/

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidget> SelectionWidget;

	bool bIsVisible;
	bool bIsMeshPresent=false;
	int32 PawnIndex = 0;

	TArray<TSubclassOf<APawn>> Pawns;
public:
	UPROPERTY(EditAnywhere)
	AArchMeshActor* StaticMeshActor;

	UPROPERTY(EditAnywhere)
	APawn* CurrentPawn;
};
