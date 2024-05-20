// Fill out your copyright notice in the Description page of Project Settings.


#include "OrthographicView.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

static void KeyMap(UInputMappingContext* InputMappingContext, const UInputAction* InputAction, const FKey& Key, const bool bNegate = false
	, const bool bSwizzle = false, const EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{

	auto& Mapping = InputMappingContext->MapKey(InputAction, Key);
	auto* Outer = InputMappingContext->GetOuter();
	if (bNegate)
	{
		auto* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}
	if (bSwizzle)
	{
		auto* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

// Sets default values
AOrthographicView::AOrthographicView()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = -211.599960;
	SpringArm->TargetOffset = FVector(-520, 0, 540);
	SpringArm->SetupAttachment(SpringArm);
	RootComponent = SpringArm;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetRelativeRotation(FRotator(270, 0, 0));
	Camera->SetupAttachment(SpringArm);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
}


void AOrthographicView::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOrthographicView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOrthographicView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	APlayerController* Player = Cast<APlayerController>(GetController());

	MappingContext = NewObject<UInputMappingContext>();
	MoveAction = NewObject<UInputAction>();
	MoveAction->ValueType = EInputActionValueType::Axis2D;

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);
	KeyMap(MappingContext, MoveAction, EKeys::W);
	KeyMap(MappingContext, MoveAction, EKeys::S, true);
	KeyMap(MappingContext, MoveAction, EKeys::D, false, true);
	KeyMap(MappingContext, MoveAction, EKeys::A, true, true);


	//For Zooming

	ScrollAction = NewObject<UInputAction>();
	ScrollAction->ValueType = EInputActionValueType::Axis1D;
	KeyMap(MappingContext, ScrollAction, EKeys::MouseWheelAxis);

	if (EIC) {
		EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AOrthographicView::Move);
		EIC->BindAction(ScrollAction, ETriggerEvent::Triggered, this, &AOrthographicView::Zoom); 

	}
		
	//Binding Actions
	if (Player) {
		ULocalPlayer* LocalPlayer = Player->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(MappingContext, 0);
	}
}

void AOrthographicView::Move(const FInputActionValue& ActionValue)
{
	FVector InputAxis = ActionValue.Get<FVector>();
	AddMovementInput(InputAxis, 2.0F);
}

void AOrthographicView::Zoom(const FInputActionValue& ActionValue) 
{
	float Value = ActionValue.Get<float>();
	SpringArm->TargetArmLength -= Value * 9.2;
	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, -500.f, 0.f);

	// Change the spring arm's target offset Z based on input value
	if (SpringArm->TargetArmLength >= -460.f)
	{
		SpringArm->TargetOffset.Z -= Value * 20.f;
	}
	SpringArm->TargetOffset.Z = FMath::Clamp(SpringArm->TargetOffset.Z, 0.f, 1500.f);
}

