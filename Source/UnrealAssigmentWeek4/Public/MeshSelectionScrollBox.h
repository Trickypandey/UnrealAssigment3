// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "MeshAssetManager.h"
#include "SMeshSelectionScrollBox.h"
#include "Widgets/SWidget.h"
#include "MeshSelectionScrollBox.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMeshAssetThumbnailSelectedEvent, const FMeshData&, MeshData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMaterialAssetThumbnailSelectedEvent, const FMaterialData&, MeshData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTextureAssetThumbnailSelectedEvent, const FTextureData&, MeshData);

UCLASS()
class UNREALASSIGMENTWEEK4_API UMeshSelectionScrollBox : public UWidget
{
public:
	GENERATED_BODY()
protected:

	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual const FText GetPaletteCategory() override;

	UFUNCTION()
	void HandleAssetMeshThumbnailSelected(const FMeshData& MeshData) const;

	UFUNCTION()
	void HandleAssetMaterialThumbnailSelected(const FMaterialData& MeshData) const;

	UFUNCTION()
	void HandleAssetTextureThumbnailSelected(const FTextureData& MeshData) const;
private:
	TSharedPtr<SMeshSelectionScrollBox> ScrollableAssetBox;

public:
	//virtual TSharedRef<SWidget> RebuildWidget() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	UMeshAssetManager* MeshDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeshData")
	float ThumbnailSize = 1;

	UPROPERTY(BlueprintAssignable, Category = "MeshData")
	FMeshAssetThumbnailSelectedEvent OnMeshAssetThumbnailSelected;

	UPROPERTY(BlueprintAssignable, Category = "MeshData")
	FMaterialAssetThumbnailSelectedEvent OnMaterialAssetThumbnailSelected;

	UPROPERTY(BlueprintAssignable, Category = "MeshData")
	FTextureAssetThumbnailSelectedEvent OnTextureAssetThumbnailSelected;

	virtual void SynchronizeProperties() override;

	UPROPERTY(EditAnywhere)
	EAssetType InAssetType;
};
