// Fill out your copyright notice in the Description page of Project Settings.

#include "Sequence/Choice/OmegaLinearChoice.h"

#include "Engine/GameInstance.h"
#include "Sequence/OmegaLinearEventSubsystem.h"

UOmegaLinearChoice::UOmegaLinearChoice(const FObjectInitializer& ObjectInitializer)
{
    if (const UObject* Owner = GetOuter())
    {
        WorldPrivate = Owner->GetWorld();
    }
}

UWorld* UOmegaLinearChoice::GetWorld() const
{
    if (GetGameInstance())
    {
        return GetGameInstance()->GetWorld();
    }
    if (WorldPrivate)
    {
        return WorldPrivate;
    }
    return nullptr;
}

UGameInstance* UOmegaLinearChoice::GetGameInstance() const
{
    return GameInstanceRef;
}

void UOmegaLinearChoice::OnChoiceSelected_Implementation() const
{
}

bool UOmegaLinearChoice::IsChoiceAvailable_Implementation() const
{
    return true;
}