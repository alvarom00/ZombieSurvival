// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryComponent.h"
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

    UFUNCTION(BlueprintCallable, Category = "Inventory")
    void UpdateProximityUI(const TArray<TSubclassOf<UItem>>& NearbyItems);

	virtual void NativeConstruct() override;

    void SetInventoryComponent(UInventoryComponent* NewInventoryComponent);
    void UpdateInventoryUI();

protected:
    /** Secci�n de proximidad */
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ProximitySection;

    /** Secci�n de manos */
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* HandsSection;

    /** Secci�n de vestimenta */
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ClothingSection;

    /** Secci�n de bolsillos */
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PocketContainerSection;

    /** Widget Blueprint del inventario */
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UInventoryItemWidget> ItemWidgetClass;

    /** Widget Blueprint para los slots */
    UPROPERTY(EditAnywhere, Category = "Inventory")
    TSubclassOf<UInventorySlotWidget> SlotWidgetClass;

    UPROPERTY()
    UInventoryComponent* InventoryComponent;
};