// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
#include "Components/ScrollBox.h"
#include "InventoryWidget.generated.h"

class UInventorySlotWidget;
class UInventoryItemWidget;
class UUniformGridPanel;
class UVerticalBox;

UCLASS()
class ZOMBIESURVIVAL_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    virtual void NativeConstruct() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateProximityUI(const TArray<TSubclassOf<UItem>>& NearbyItems);

    UFUNCTION(BlueprintCallable)
    void ClearProximityUI();

    void SetInventoryComponent(UInventoryComponent* NewInventoryComponent);
    void UpdateInventoryUI();

protected:
    /** Sección de proximidad */
    UPROPERTY(meta = (BindWidget))
    UScrollBox* ProximitySection;

    /** Sección de manos */
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* HandsSection;

    /** Sección de vestimenta */
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ClothingSection;

    /** Sección de bolsillos */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PocketContainerSection;

    /** Widget Blueprint del inventario */
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

    /** Widget Blueprint para los slots */
    UPROPERTY(EditDefaultsOnly, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

    UPROPERTY()
    UInventoryComponent* InventoryComponent;
};