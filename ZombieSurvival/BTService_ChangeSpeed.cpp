// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_ChangeSpeed.h"
#include "AI_Controller.h"
#include "AI.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTService_ChangeSpeed::UBTService_ChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	if (auto const Cont = OwnerComp.GetAIOwner())
	{
		if (auto* const AI = Cast<AAI>(Cont->GetPawn()))
		{
			AI->GetCharacterMovement()->MaxWalkSpeed = Speed;
		}
	}
}