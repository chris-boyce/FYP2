// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bot.h"
#include "GameFramework/Actor.h"
#include "ServerController.h"
#include "SpawnController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoundEnd, int, ServerID, TArray<int>, BotIDs);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEloChange, int, BotID, float, AmountChange);
UCLASS()
class FYP5_API ASpawnController : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnController();

protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotData> RedMatchBots;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotData> BlueMatchBots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnControllerIDNum;
	
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
	AActor* SpawnPointBlue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SpawnPointRed;

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

	//UFUNCTION(BlueprintCallable , Category="Elo")
	//void EloCalc();
	
	UFUNCTION(BlueprintCallable , Category="Rounds")
	void EndGame();

	UFUNCTION(BlueprintCallable , Category="Rounds")
	void RestartGame();

	UPROPERTY(BlueprintAssignable, Category = "EventCaller")
	FOnRoundEnd OnRoundEnd;

	UPROPERTY(BlueprintAssignable, Category = "EventCaller")
	FOnEloChange OnEloChange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AverageBlueElo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AverageRedElo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NewBlueRating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NewRedRating;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RedEloChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlueEloChange;
	


};
