// Fill out your copyright notice in the Description page of Project Settings.


#include "PerspectiveView.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/FloatingPawnMovement.h"

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
APerspectiveView::APerspectiveView(): PawnMappingContext(nullptr), MoveAction(nullptr), CameraComponent(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	RootComponent = CameraComponent;
	PMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	RotateScale = 500.f;
}

// Called when the game starts or when spawned
void APerspectiveView::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APerspectiveView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APerspectiveView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;
	KeyMap(PawnMappingContext, MoveAction, EKeys::W);
	KeyMap(PawnMappingContext, MoveAction, EKeys::S, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::A, true, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::D, false, true);
	KeyMap(PawnMappingContext, MoveAction, EKeys::Q, true, true, EInputAxisSwizzle::ZYX);
	KeyMap(PawnMappingContext, MoveAction, EKeys::E, false, true, EInputAxisSwizzle::ZYX);

	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Axis2D;

	KeyMap(PawnMappingContext, RotateAction, EKeys::MouseY);
	KeyMap(PawnMappingContext, RotateAction, EKeys::MouseX, false, true);

	

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	APlayerController* FPC = Cast<APlayerController>(Controller);

	check(EIC && FPC);

	EIC->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APerspectiveView::Move);
	EIC->BindAction(RotateAction, ETriggerEvent::Triggered, this, &APerspectiveView::Rotate);
	ULocalPlayer* LocalPlayer = FPC->GetLocalPlayer();
	check(LocalPlayer);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(PawnMappingContext, 0);

}


void APerspectiveView::Move(const FInputActionValue& ActionValue)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("In moveS	"));
	FVector Input = ActionValue.Get<FVector>();
	AddMovementInput(GetActorRotation().RotateVector(Input));
}


void APerspectiveView::Rotate(const FInputActionValue& ActionValue)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("In Rotation"));
	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
	Input += GetActorRotation();
	Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
	Input.Roll = 0;
	SetActorRotation(Input);
}