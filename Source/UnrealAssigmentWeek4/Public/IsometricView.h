// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArchMeshActor.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "IsometricView.generated.h"

UCLASS()
class UNREALASSIGMENTWEEK4_API AIsometricView : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AIsometricView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void CameraLeftRotation();
	void CameraRightRotation();
	void CameraZoom(const FInputActionValue& ActionValue);

private:
	// Camera boom positioning the camera above the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	// Rotation angle increment
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float RotationAngleIncrement;


	

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* RotateClockwiseAction;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* RotateAntiClockwiseAction;



	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ZoomAction;


	UPROPERTY()
	class UFloatingPawnMovement* PMovement;


	float RotationSpeed;
	float TargetRotationYawValue;
};
