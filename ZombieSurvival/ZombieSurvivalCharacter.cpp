// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieSurvivalCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "CollisionShape.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "MotionWarpingComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AZombieSurvivalCharacter

AZombieSurvivalCharacter::AZombieSurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	AddOwnedComponent(MotionWarpingComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	bIsInventoryOpen = false;


	ProximitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("ProximitySphere"));
	ProximitySphere->SetupAttachment(RootComponent);
	ProximitySphere->SetSphereRadius(300.f); // Define el radio
	ProximitySphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // Solo consulta colisiones
	ProximitySphere->SetCollisionObjectType(ECC_WorldDynamic);
	ProximitySphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProximitySphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);  // Detectar jugadores/NPCs
	ProximitySphere->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	ProximitySphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);

	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &AZombieSurvivalCharacter::OnProximityEnter);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &AZombieSurvivalCharacter::OnProximityExit);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZombieSurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZombieSurvivalCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AZombieSurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {


		Super::SetupPlayerInputComponent(PlayerInputComponent);
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AZombieSurvivalCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AZombieSurvivalCharacter::Look);

		// open inventory
		PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AZombieSurvivalCharacter::ToggleInventory);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AZombieSurvivalCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AZombieSurvivalCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AZombieSurvivalCharacter::SetupStimulusSource()
{
	StimulusSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	if (StimulusSource)
	{
		StimulusSource->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSource->RegisterWithPerceptionSystem();

	}
}

void AZombieSurvivalCharacter::ToggleInventory()
{
	bIsInventoryOpen = !bIsInventoryOpen;

	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidget is not initialized!"));
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null!"));
		return;
	}

	if (bIsInventoryOpen)
	{
		InventoryWidget->AddToViewport();
		PlayerController->bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		PlayerController->SetInputMode(InputMode);

		// FORZAR actualizaci�n de �tems en proximidad al abrir el inventario
		TArray<AActor*> OverlappingActors;
		ProximitySphere->GetOverlappingActors(OverlappingActors, AItemActor::StaticClass());
		InventoryComponent->ProximityItems.Empty();

		for (AActor* Actor : OverlappingActors)
		{
			AItemActor* ItemActor = Cast<AItemActor>(Actor);
			if (ItemActor && ItemActor->ItemData)
			{
				InventoryComponent->ProximityItems.Add(ItemActor);
			}
		}

		InventoryWidget->UpdateProximityUI(InventoryComponent->ProximityItems);
	}
	else
	{
		InventoryWidget->RemoveFromParent();
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

void AZombieSurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	bIsInventoryOpen = false;

	// Verificar si InventoryWidgetClass est� configurado
	if (InventoryWidgetClass)
	{
		APlayerController* PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController)
		{
			// Crear el widget a partir de la clase
			InventoryWidget = CreateWidget<UInventoryWidget>(PlayerController, InventoryWidgetClass);

			// Confirmar si el widget se cre� correctamente
			if (!InventoryWidget)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to create InventoryWidget!"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerController is null in BeginPlay!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryWidgetClass is null! Please assign a widget class in the editor or via code."));
	}
}

void AZombieSurvivalCharacter::OnProximityEnter(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!bIsInventoryOpen) return; // Solo ejecuta si el inventario est� abierto

	AItemActor* ItemActor = Cast<AItemActor>(OtherActor);
	if (ItemActor)
	{
		InventoryComponent->ProximityItems.AddUnique(ItemActor); // Agrega el AItemActor directamente
		InventoryWidget->UpdateProximityUI(InventoryComponent->ProximityItems);
	}
}

void AZombieSurvivalCharacter::OnProximityExit(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AItemActor* ItemActor = Cast<AItemActor>(OtherActor);
	if (ItemActor)
	{
		InventoryComponent->ProximityItems.Remove(ItemActor);
		InventoryWidget->UpdateProximityUI(InventoryComponent->ProximityItems);
	}
}