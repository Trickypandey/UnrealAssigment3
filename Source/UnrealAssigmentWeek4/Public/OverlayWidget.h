// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MeshAssetManager.h"
#include "MeshSelectionScrollBox.h"
#include "Components/CanvasPanel.h"
#include "OverlayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALASSIGMENTWEEK4_API UOverlayWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	/*void InitializeWidget(UMeshAssetManager* MeshDataAsset);*/
	//UFUNCTION(BlueprintAssignableEvent)
	/*UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FMeshAssetThumbnailSelectedEvent MeshThumbnailDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FMaterialAssetThumbnailSelectedEvent MaterialThumbnailDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Overlay Widget")
	FTextureAssetThumbnailSelectedEvent TextureThumbnailDelegate;*/



	//UFUNCTION(BlueprintCallable, Category = "Overlay Widget")
	//void ReceiveMeshData(FMeshData MeshData);

	//UFUNCTION(BlueprintImplementableEvent)
	//void OnMeshDataInitialization(const UMeshAssetManager* MeshDataAsset);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UCanvasPanel* CanvasPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* MeshBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* TextureBox;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UMeshSelectionScrollBox* MaterialBox;
};
