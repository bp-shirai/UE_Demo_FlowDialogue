// Fill out your copyright notice in the Description page of Project Settings.

#include "Sequence/LinearEvents/LinearEvent_ActorBase.h"

AActor* UOmegaActorEventBase::GetEventActor()
{
    if (ActorSource && GetWorld())
    {
        return ActorSource->Private_GetActor(GetWorld());
    }
    return nullptr;
}

TArray<AActor*> UOmegaActorEventBase::GetEventActorList()
{
    TArray<AActor*> out;
    if (ActorSource && GetWorld())
    {
        return ActorSource->Private_GetActorList(GetWorld());
    }
    return out;
}

void UOmegaActorEvent_Teleport::Native_Begin(const FString& Flag)
{
    Super::Native_Begin(Flag);
	const AActor* TargetActor = TargetPoint->Private_GetActor(GetWorld());

    if (GetEventActor() && TargetActor)
    {
        FHitResult DumpHit;
        const FVector NewLocation  = TargetActor->GetActorLocation();
        const FRotator NewRotation = TargetActor->GetActorRotation();
        GetEventActor()->K2_SetActorLocationAndRotation(NewLocation, NewRotation, false, DumpHit, false);
    }
    Finish(" ");
}