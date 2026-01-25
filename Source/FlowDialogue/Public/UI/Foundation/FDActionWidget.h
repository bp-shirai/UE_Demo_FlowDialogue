// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActionWidget.h"
#include "FDActionWidget.generated.h"

class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class FLOWDIALOGUE_API UFDActionWidget : public UCommonActionWidget
{
    GENERATED_BODY()

public:
    //~ Begin UCommonActionWidget interface
    virtual FSlateBrush GetIcon() const override;
    //~ End of UCommonActionWidget interface

private:
    UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputSubsystem() const;
};