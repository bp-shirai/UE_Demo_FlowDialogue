// Fill out your copyright notice in the Description page of Project Settings.

#include "Sequence/LinearEvents/LinearChoice_SimpleChoice.h"

void ULinearChoice_SimpleChoice::GetGeneralDataText_Implementation(const FString& Label, const UObject* Context, FText& Name, FText& Description)
{
    Name = ChoiceText;
}

bool ULinearChoice_SimpleChoice::IsChoiceAvailable_Implementation() const
{
    return true;//GetWorld()->GetGameInstance()->GetSubsystem<UOmegaSaveSubsystem>()->CustomSaveConditionsMet(Conditions);
}