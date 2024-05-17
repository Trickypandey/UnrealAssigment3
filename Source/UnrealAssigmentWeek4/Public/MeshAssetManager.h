// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MeshAssetManager.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EAssetType : uint8 {
	MeshType,
	MaterialType,
	TextureType
};

USTRUCT(BlueprintType)
struct FMeshData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMesh* Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* Icon;
};


USTRUCT(BlueprintType)
struct FMaterialData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UMaterialInterface* Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture2D* Icon;

};

USTRUCT(BlueprintType)
struct FTextureData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UTexture* Type;
};

UCLASS()
class UNREALASSIGMENTWEEK4_API UMeshAssetManager : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<FMeshData> MeshDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material")
	TArray<FMaterialData> MaterialDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Texture")
	TArray<FTextureData> TextureDataArray;
	
};
