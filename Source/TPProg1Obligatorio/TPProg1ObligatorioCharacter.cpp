// Copyright Epic Games, Inc. All Rights Reserved.

#include "TPProg1ObligatorioCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "HealthWidget.h"
#include "HealNotificationWidget.h"
#include "InputActionValue.h"
#include "InteractableInterface.h"
#include "TPProg1Obligatorio.h"
#include "Blueprint/UserWidget.h"

ATPProg1ObligatorioCharacter::ATPProg1ObligatorioCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATPProg1ObligatorioCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATPProg1ObligatorioCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ATPProg1ObligatorioCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATPProg1ObligatorioCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATPProg1ObligatorioCharacter::DoInteract);

	}
	else
	{
		UE_LOG(LogTPProg1Obligatorio, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ATPProg1ObligatorioCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ATPProg1ObligatorioCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ATPProg1ObligatorioCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsLocallyControlled())
	{
		if (HealthWidgetClass)
		{
			HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass);
			if (HealthWidget)
				HealthWidget->AddToViewport();
		}

		if (HealthComponent)
		{
			HealthComponent->OnLifeChanged.AddDynamic(
				this, &ATPProg1ObligatorioCharacter::HandleLifeChanged);
			HandleLifeChanged(HealthComponent->Health, HealthComponent->MaxHealth);
		}
	}
}

void ATPProg1ObligatorioCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ATPProg1ObligatorioCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ATPProg1ObligatorioCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ATPProg1ObligatorioCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}


void ATPProg1ObligatorioCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!OtherActor) return;

	UE_LOG(LogTemp, Warning, TEXT("[%s] Overlap BEGIN with %s"),
		*GetName(),
		*OtherActor->GetName());

	if (OtherActor->Implements<UInteractableInterface>())
	{
		CurrentInteractable = OtherActor;

		UE_LOG(LogTemp, Warning, TEXT("[%s] Set CurrentInteractable -> %s"),
			*GetName(),
			*OtherActor->GetName());
	}
}

void ATPProg1ObligatorioCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!OtherActor) return;

	UE_LOG(LogTemp, Warning, TEXT("[%s] Overlap END with %s"),
		*GetName(),
		*OtherActor->GetName());

	if (OtherActor == CurrentInteractable)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] Clearing CurrentInteractable (%s)"),
			*GetName(),
			*OtherActor->GetName());

		CurrentInteractable = nullptr;
	}
}

void ATPProg1ObligatorioCharacter::HandleLifeChanged(float Health, float MaxHealth)
{
	if (HealthWidget)
	{
		HealthWidget->UpdateBar(Health, MaxHealth);
	}
}

void ATPProg1ObligatorioCharacter::DoInteract()
{
	if (!IsLocallyControlled()) return;
	if (!CurrentInteractable) return;
	Server_Interact(CurrentInteractable);
}


void ATPProg1ObligatorioCharacter::Server_Interact_Implementation(AActor* Interactable)
{
	if (!HasAuthority()) return;
	if (Interactable && Interactable->Implements<UInteractableInterface>())
	{
		IInteractableInterface::Execute_Interact(Interactable, this);
	}
}

void ATPProg1ObligatorioCharacter::ShowHealMessage_Implementation(float HealAmount)
{
	Client_ShowHealMessage(HealAmount);
}

void ATPProg1ObligatorioCharacter::Client_ShowHealMessage_Implementation(float HealAmount)
{
	if (!IsLocallyControlled()) return;

	if (HealNotificationClass)
	{
		UHealNotificationWidget* Widget = CreateWidget<UHealNotificationWidget>(GetWorld(), HealNotificationClass);
		if (Widget)
		{
			Widget->AddToViewport();

			FString Msg = FString::Printf(TEXT("Estás mejor que antes +%.0f"), HealAmount);
			Widget->ShowMessage(Msg);

			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(Timer, [Widget]()
			{
				if (Widget)
				{
					Widget->RemoveFromParent();
				}
			}, 2.0f, false);
		}
	}
}
