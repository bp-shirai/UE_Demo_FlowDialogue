// Copyright https://github.com/MothCocoon/FlowGame/graphs/contributors

#include "Gameplay/SimplePlayerController.h"

#include "Gameplay/GameplaySettings.h"
#include "Gameplay/InteractionComponent.h"
#include "Gameplay/SimpleUIManager.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Pawn.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SimplePlayerController)

ASimplePlayerController::ASimplePlayerController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = true;

    UInteractionComponent::OnPlayerEnter.AddUObject(this, &ASimplePlayerController::OnInteractionEnter);
    UInteractionComponent::OnPlayerExit.AddUObject(this, &ASimplePlayerController::OnInteractionExit);
}

void ASimplePlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    SetExplorationContext();

    EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    check(EnhancedInputComponent.IsValid());

    // Exploration context
    {
        EnhancedInputComponent->BindAction(IA_Interaction, ETriggerEvent::Started, this, &ThisClass::OnInteractionUsed);

        EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
        EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
        EnhancedInputComponent->BindAction(IA_Journal, ETriggerEvent::Started, this, &ThisClass::OnOpenJournal);
        EnhancedInputComponent->BindAction(IA_Pause, ETriggerEvent::Started, this, &ThisClass::OnPauseMenu);
    }
}

void ASimplePlayerController::SetExplorationContext() const
{
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        Subsystem->ClearAllMappings();

#if !UE_BUILD_SHIPPING
        Subsystem->AddMappingContext(DebugContext, 0);
#endif

        Subsystem->AddMappingContext(GenericContext, 0);
        Subsystem->AddMappingContext(ExplorationContext, 1);
    }
}

void ASimplePlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    if (PossibleInteractions.Num() > 0)
    {
        const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();
        PossibleInteractions.Sort(
            [CameraLocation](const TWeakObjectPtr<UInteractionComponent>& A, const TWeakObjectPtr<UInteractionComponent>& B)
            {
                const float DistanceA = FVector::DistSquared(CameraLocation, A.Get()->GetComponentLocation());
                const float DistanceB = FVector::DistSquared(CameraLocation, B.Get()->GetComponentLocation());
                return DistanceA < DistanceB;
            });

        if (!ActiveInteraction.IsValid() && ActiveInteraction != PossibleInteractions[0])
        {
            ActivateInteraction(PossibleInteractions[0]);
        }
    }
    else if (ActiveInteraction.IsValid())
    {
        DeactivateInteraction();
    }
}

void ASimplePlayerController::OnInteractionEnter(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
    PossibleInteractions.Add(Interaction);
}

void ASimplePlayerController::OnInteractionExit(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
    if (ActiveInteraction.IsValid() && ActiveInteraction == Interaction)
    {
        DeactivateInteraction();
    }

    PossibleInteractions.Remove(Interaction);
}

void ASimplePlayerController::ActivateInteraction(const TWeakObjectPtr<UInteractionComponent> Interaction)
{
    if (ActiveInteraction.IsValid())
    {
        DeactivateInteraction();
    }

    ActiveInteraction = Interaction;
    GetGameInstance()->GetSubsystem<USimpleUIManager>()->OpenWidget(GetDefault<UGameplaySettings>()->InteractionWidget);
}

void ASimplePlayerController::DeactivateInteraction()
{
    ActiveInteraction = nullptr;
    GetGameInstance()->GetSubsystem<USimpleUIManager>()->CloseWidget(GetDefault<UGameplaySettings>()->InteractionWidget);
}

void ASimplePlayerController::OnInteractionUsed()
{
    if (ActiveInteraction.IsValid())
    {
        ActiveInteraction->OnUsed.Broadcast();
    }
}

void ASimplePlayerController::Move(const FInputActionValue& Value)
{
    if (!IsValid(GetPawn()) || IsMoveInputIgnored()) return;

    const FVector2D InputVector = Value.Get<FVector2D>().GetSafeNormal();
    const FRotator YawRotation(0, GetControlRotation().Yaw, 0);

    const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection   = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    const FVector MovementVector = InputVector.Y * ForwardDirection + InputVector.X * RightDirection;
    GetPawn()->AddMovementInput(MovementVector);
}

void ASimplePlayerController::Look(const FInputActionValue& Value)
{
    if (!IsValid(GetPawn()) || IsLookInputIgnored()) return;

    const FVector2D InputVector = Value.Get<FVector2D>();

    AddYawInput(InputVector.X);
    AddPitchInput(InputVector.Y);
}
