#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/Character.h"
#include "AI.generated.h"

UCLASS()
class ZOMBIESURVIVAL_API AAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UBehaviorTree* GetBehaviorTree() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* Tree;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};