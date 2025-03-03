// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Border.h"
#include "PocketSlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIESURVIVAL_API UPocketSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    UBorder* SlotBorder; // Representará el cuadrado visual

    virtual void NativeConstruct() override
    {
        Super::NativeConstruct();
    }
};
