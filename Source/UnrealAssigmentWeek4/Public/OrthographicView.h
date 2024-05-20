// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "OrthographicView.generated.h"

UCLASS()
class UNREALASSIGMENTWEEK4_API AOrthographicView : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AOrthographicView();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly)
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* Camera;

	UPROPERTY()
	UInputMappingContext* MappingContext;

	UPROPERTY()
	UInputAction* MoveAction;

	UPROPERTY()
	UInputAction* ScrollAction;
	UPROPERTY(EditDefaultsOnly)
	UFloatingPawnMovement* FloatingMovement;

	UFUNCTION()
	void Move(const FInputActionValue& ActionValue);
	void Zoom(const FInputActionValue& ActionValue);

};
