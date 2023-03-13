// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bot.h"
#include "GameFramework/Actor.h"
#include "SpawnController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRoundEnd);
UCLASS()
class FYP5_API ASpawnController : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnController();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable , Category="Spawn System")
	void SpawnAI();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int TeamSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int RoundNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn System")
	TSubclassOf<AActor>AIToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPointBlue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> SpawnPointRed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABot*> BotListBlue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABot*> BotListRed;
	
	bool BlueTeamWin;
	bool RedTeamWin;

	int DeadCountBlue;
	int DeadCountRed;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scoring")
	FVector2D Score;

	FVector OffSet;

	UFUNCTION(BlueprintCallable , Category="Rounds")
	void CheckEnemysLeft(bool isBlue);

	UFUNCTION(BlueprintCallable , Category="Rounds")
	void ResetRound();
	
	UPROPERTY(BlueprintAssignable, Category = "EventCaller")
	FOnRoundEnd OnRoundEnd;
	
	UFUNCTION(BlueprintCallable , Category="Rounds")
	void EndGame();

};
