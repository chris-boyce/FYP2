// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Bot.generated.h"

UCLASS()
class FYP5_API ABot : public ACharacter
{
	GENERATED_BODY()

public:
	ABot();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable , Category="Scoring")
	void KilledEnemy();

	UFUNCTION(BlueprintCallable , Category="Scoring")
	void Died();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Kills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Deaths;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IsDead;

	UFUNCTION(BlueprintImplementableEvent , Category="Team")
	void SetBlueTeam();

	UFUNCTION(BlueprintImplementableEvent , Category="Team")
	void SetRedTeam();

	UFUNCTION(BlueprintImplementableEvent , Category="Rounds")
	void RespawnBot();
	
	
	
};
