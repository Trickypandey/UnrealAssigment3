// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Layout/SScrollBox.h"
#include "MeshAssetManager.h"
#include "SlateOptMacros.h"
#include "MeshAssetManager.h"
#include "Styling/SlateBrush.h"
#include "Engine/Texture2D.h"
#include "Widgets/Input/SEditableTextBox.h"

/**
 * 
 */
DECLARE_DELEGATE_OneParam(FAssetTextureThumbnailSelected, const FTextureData&);
DECLARE_DELEGATE_OneParam(FAssetMaterialThumbnailSelected, const FMaterialData&);
DECLARE_DELEGATE_OneParam(FAssetMeshThumbnailSelected, const FMeshData&);
class UNREALASSIGMENTWEEK4_API SMeshSelectionScrollBox : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMeshSelectionScrollBox)
		{}
		SLATE_ARGUMENT(TWeakObjectPtr<UMeshAssetManager>, InMeshDataAsset)
		SLATE_ARGUMENT(float, InThumbnailSize)
		SLATE_ARGUMENT(EAssetType, InAssetType)
	SLATE_END_ARGS()


private:
	TSharedPtr<SScrollBox> ScrollBox;
	TSharedPtr<SScrollBox> MaterialVerticalScrollBox;
	TSharedPtr<SScrollBox> TextureVerticalScrollBox;
	TSharedPtr<SScrollBox> MeshHorizontalScrollBox;


	//TSharedPtr<STextBlock> TextBox;

	TWeakObjectPtr<UMeshAssetManager> MeshDataAsset;

public:
	

	void Construct(const FArguments& InArgs);

	float ThumbnailSize;

	void RefreshContent() const;

	void DisplayMaterials(TArray<FMaterialData> DataArray) const;

	void DisplayMeshes(TArray<FMeshData> DataArray) const;
	void DisplayTextures(TArray<FTextureData> DataArray) const;

	
	FAssetTextureThumbnailSelected OnTextureThumbnailSelected;
	FAssetMaterialThumbnailSelected OnMaterialThumbnailSelected;
	FAssetMeshThumbnailSelected OnMeshThumbnailSelected;

	EAssetType AssetType;
};
