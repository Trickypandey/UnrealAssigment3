// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshSelectionScrollBox.h"

#define LOCTEXT_NAMESPACE "ScrollableAssetBoxWidget"


TSharedRef<SWidget> UMeshSelectionScrollBox::RebuildWidget()
{
	ScrollableAssetBox = SNew(SMeshSelectionScrollBox).InMeshDataAsset(MeshDataAsset).InThumbnailSize(ThumbnailSize).InAssetType(InAssetType);
	ScrollableAssetBox->OnMeshThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleAssetMeshThumbnailSelected);
	ScrollableAssetBox->OnMaterialThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleAssetMaterialThumbnailSelected);
	ScrollableAssetBox->OnTextureThumbnailSelected.BindUObject(this, &UMeshSelectionScrollBox::HandleAssetTextureThumbnailSelected);
	return ScrollableAssetBox.ToSharedRef();
	
}

const FText UMeshSelectionScrollBox::GetPaletteCategory()
{
	return LOCTEXT("PaletteCategory", "Panel");
}

void UMeshSelectionScrollBox::HandleAssetMeshThumbnailSelected(const FMeshData& MeshData) const
{
	OnMeshAssetThumbnailSelected.Broadcast(MeshData);
}

void UMeshSelectionScrollBox::HandleAssetMaterialThumbnailSelected(	const FMaterialData& MaterialData) const
{
	OnMaterialAssetThumbnailSelected.Broadcast(MaterialData);
}

void UMeshSelectionScrollBox::HandleAssetTextureThumbnailSelected(const FTextureData& TextureData) const
{
	OnTextureAssetThumbnailSelected.Broadcast(TextureData);
}

void UMeshSelectionScrollBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	if (ScrollableAssetBox) {
		ScrollableAssetBox->ThumbnailSize = ThumbnailSize;
		ScrollableAssetBox->RefreshContent();
	}
}


#undef LOCTEXT_NAMESPACE