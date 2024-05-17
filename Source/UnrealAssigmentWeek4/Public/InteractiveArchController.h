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
	virtual void BeginPlay() override;

	void LeftClickProcessor();
	void HideVisibility();
	void ShowMeshTextureWidget();
	void HideVisiblity();

	UFUNCTION()
	void SpawnActor(const FMeshData& MeshData);
protected:
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnLeftClick;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputAction* OnTabClick;

	UPROPERTY(EditDefaultsOnly, Category = "Controller")
	UInputMappingContext* MappingContext;

	UPROPERTY()
	FVector LastHitLocation;

	UPROPERTY()
	UOverlayWidget* SelectionWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidget> SelectionWidget;

	bool bIsVissible;
public:
	UPROPERTY(EditAnywhere)
	AArchMeshActor* StaticMeshActor;
};
