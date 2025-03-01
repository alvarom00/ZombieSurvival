// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

UItem::UItem()
{
    ItemName = FName("Default Item");
    ItemSize = 1;
    ItemWeight = 1.0f;
}

void UItem::Use()
{
    UE_LOG(LogTemp, Warning, TEXT("Using %s"), *ItemName.ToString());
}