// Fill out your copyright notice in the Description page of Project Settings.


#include "SMeshSelectionScrollBox.h"
#include "SlateOptMacros.h"
#include "Brushes/SlateColorBrush.h"


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SMeshSelectionScrollBox::Construct(const FArguments& InArgs)
{
	MeshDataAsset = InArgs._InMeshDataAsset;
	ThumbnailSize = InArgs._InThumbnailSize;
	AssetType = InArgs._InAssetType;

	ScrollBox = SNew(SScrollBox);
	TSharedPtr<STextBlock> TextBox = SNew(STextBlock)
		.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 24))
		.Justification(ETextJustify::Center)
		.ColorAndOpacity(FColor::Black);

	ChildSlot
		[
			ScrollBox.ToSharedRef()
		];

	

	RefreshContent();
}
void SMeshSelectionScrollBox::RefreshContent() const
{

	ScrollBox->ClearChildren();
	if (MeshDataAsset.IsValid()) {

		//if (AssetT == AssetType::MeshType) {
		//DisplayMeshes(MeshDataAsset->MeshDataArray);
		//}
		//if (AssetT == AssetType::MaterialType) {
		//DisplayMaterials(MeshDataAsset->MaterialDataArray);
		//}
		//if (AssetT == AssetType::TextureType) {
		//DisplayTextures(MeshDataAsset->TextureDataArray);
		//}
		switch (AssetType)
		{
			case EAssetType::MeshType:
			{
				DisplayMeshes(MeshDataAsset->MeshDataArray);
				break;
			}
			case EAssetType::MaterialType:
			{
				DisplayMaterials(MeshDataAsset->MaterialDataArray);
				break;
			}
			case EAssetType::TextureType:
			{
				DisplayTextures(MeshDataAsset->TextureDataArray);
				break;
			}
			default:
			{
				DisplayMeshes(MeshDataAsset->MeshDataArray);
				break;
			}
			
		}
	}
}

void SMeshSelectionScrollBox::DisplayTextures(TArray<FTextureData> DataArray) const
{
	ScrollBox->ClearChildren();
	ScrollBox->SetOrientation(Orient_Vertical);
	for (const FTextureData& Data : DataArray) {
		if (Data.Type) {
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Type)) {
				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->SetResourceObject(Data.Type);
				SlateBrush->ImageSize = FVector2D(200); // Adjust the image si  ze as needed
				TSharedPtr<STextBlock> TextBlock = SNew(STextBlock)
					.ColorAndOpacity(FColor::FromHex("#0d3b66"))
					.Font(FSlateFontInfo(FPaths::EngineContentDir() / TEXT("Slate/Fonts/Roboto-Regular.ttf"), 20))
					.Text(Data.Name);

				TSharedPtr<SImage> ThumbnailImage = SNew(SImage)
					.Image(SlateBrush)
					.Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& Geometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnTextureThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				FSlateColorBrush* BorderBrush = new FSlateColorBrush(FLinearColor(0.904661f, 0.651406f, 0.111932f, 1.0f));

				TSharedPtr<SBorder> ImageBorder = SNew(SBorder)
					.BorderImage(BorderBrush)
					.Padding(10)
					[
						ThumbnailImage.ToSharedRef()
					];

				TSharedPtr<SVerticalBox> VerticalBox = SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.AutoHeight()
					[
						ImageBorder.ToSharedRef()
					]
					+ SVerticalBox::Slot()
					.HAlign(EHorizontalAlignment::HAlign_Center)
					.AutoHeight()
					.Padding(10)
					[
						TextBlock.ToSharedRef()
					];

				ScrollBox->AddSlot()
					[
						VerticalBox.ToSharedRef()
					];
			}
		}
	}
}


void SMeshSelectionScrollBox::DisplayMaterials(TArray<FMaterialData> DataArray) const
{
	ScrollBox->ClearChildren();
	ScrollBox->SetOrientation(Orient_Vertical);
	for (const FMaterialData& Data : DataArray) {
		UE_LOG(LogTemp, Warning, TEXT("in mESS: %d  %s"), DataArray.Num() ,*Data.Name.ToString());
		if (Data.Type) {
			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Icon)) {
				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->SetResourceObject(Data.Icon);
				SlateBrush->ImageSize = FVector2D(200);;
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& MouseGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMaterialThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<STextBlock> TextBox = SNew(STextBlock).Text(Data.Name);	
				ScrollBox->AddSlot()
					[
						
						SNew(SVerticalBox)
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								ThumbnailImage.ToSharedRef()
							]
							+ SVerticalBox::Slot()
							.AutoHeight()
							[
								TextBox.ToSharedRef()
							]
					];
			}
		}
	}
}

void SMeshSelectionScrollBox::DisplayMeshes(TArray<FMeshData> DataArray) const
{
	ScrollBox->ClearChildren();
	ScrollBox->SetOrientation(Orient_Horizontal);
	for (const FMeshData& Data : DataArray) {
		if (Data.Type) {
			/* UE_LOG(LogTemp, Warning, TEXT("in mESS: %d  %s"), DataArray.Num() ,*Data.Name.ToString()); */

			if (UTexture2D* ThumbnailTexture = Cast<UTexture2D>(Data.Icon)) {
				FSlateBrush* SlateBrush = new FSlateBrush();
				SlateBrush->SetResourceObject(Data.Icon);
				SlateBrush->ImageSize = FVector2D(200);
				TSharedPtr<SImage> ThumbnailImage = SNew(SImage).Image(SlateBrush).Cursor(EMouseCursor::Hand)
					.OnMouseButtonDown_Lambda([this, Data](const FGeometry& MouseGeometry, const FPointerEvent& MouseEvent) {
					if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton) {
						OnMeshThumbnailSelected.ExecuteIfBound(Data);
						return FReply::Handled();
					}
					return FReply::Unhandled();
						});

				TSharedPtr<STextBlock> TextBox = SNew(STextBlock).Text(Data.Name);

				ScrollBox->AddSlot()
					[
						SNew(SHorizontalBox)
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								ThumbnailImage.ToSharedRef()
							]
							+ SHorizontalBox::Slot()
							.AutoWidth()
							[
								TextBox.ToSharedRef()
							]
					];
			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
