#include "IsometricView.h"

#include "InteractiveArchController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"



// Sets default values
AIsometricView::AIsometricView()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("IsometricSpringArm"));
	SpringArm->TargetArmLength = 2500;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 30.0f;
	SpringArm->TargetOffset.Z = 300;
	SpringArm->AddRelativeRotation(FRotator(-45, 45, 0));
	RootComponent = SpringArm;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("IsometricCamera"));
	FollowCamera->SetupAttachment(SpringArm);

	PMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;

	RotationSpeed = 2.0f;
	TargetRotationYawValue = 45.0f;
}

// Called when the game starts or when spawned
void AIsometricView::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIsometricView::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = SpringArm->GetRelativeRotation();
	FRotator TargetRotation = FRotator(CurrentRotation.Pitch, TargetRotationYawValue, CurrentRotation.Roll);
	SpringArm->SetRelativeRotation(FMath::Lerp(CurrentRotation, TargetRotation, DeltaTime * RotationSpeed));
}

// Called to bind functionality to input
void AIsometricView::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	AInteractiveArchController* PlayerController = Cast<AInteractiveArchController>(GetController());

	auto* IsometricCameraMapping = NewObject<UInputMappingContext>(this);

	RotateClockwiseAction = NewObject<UInputAction>(this);
	RotateClockwiseAction->ValueType = EInputActionValueType::Boolean;

	RotateAntiClockwiseAction = NewObject<UInputAction>(this);
	RotateAntiClockwiseAction->ValueType = EInputActionValueType::Boolean;

	ZoomAction = NewObject<UInputAction>(this);
	ZoomAction->ValueType = EInputActionValueType::Boolean;

	

	UInputModifierNegate* Negation = NewObject<UInputModifierNegate>(this);

	IsometricCameraMapping->MapKey(RotateClockwiseAction, EKeys::Q);

	IsometricCameraMapping->MapKey(RotateAntiClockwiseAction, EKeys::E);

	IsometricCameraMapping->MapKey(ZoomAction, EKeys::MouseWheelAxis);

	


	if (EIC) {
		EIC->BindAction(RotateClockwiseAction, ETriggerEvent::Completed, this, &AIsometricView::CameraLeftRotation);
		EIC->BindAction(RotateAntiClockwiseAction, ETriggerEvent::Completed, this, &AIsometricView::CameraRightRotation);
		EIC->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AIsometricView::CameraZoom);


	}

	if (PlayerController) {
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		check(LocalPlayer);
		UEnhancedInputLocalPlayerSubsystem* SubSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		check(SubSystem);
		//SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(IsometricCameraMapping, 0);
	}
}

void AIsometricView::CameraLeftRotation()
{
	TargetRotationYawValue += 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricView::CameraRightRotation()
{
	TargetRotationYawValue -= 45;
	TargetRotationYawValue = FMath::Wrap(TargetRotationYawValue, -180.0f, 180.0f);
}

void AIsometricView::CameraZoom(const FInputActionValue& ActionValue)
{
	float value = ActionValue.Get<float>();

	SpringArm->TargetArmLength = FMath::Clamp(SpringArm->TargetArmLength, 500, 3000);
	SpringArm->TargetArmLength -= value * 20;
}