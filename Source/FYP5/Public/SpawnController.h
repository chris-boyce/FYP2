// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Bot.h"
#include "GameFramework/Actor.h"
#include "SpawnController.generated.h"

UCLASS()
class FYP5_API ASpawnController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawn System")
	ABot* CurrentAI;

	bool BlueTeamWinRound;
	bool RedTeamWinRound;

	int DeadCountBlue;
	int DeadCountRed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Scoring")
	FVector2D Score;

	FVector OffSet;

	UFUNCTION(BlueprintCallable , Category="Rounds")
	void CheckEnemysLeft(bool isBlue);

	UFUNCTION(BlueprintCallable , Category="Rounds")
	void ResetRound();

	
	

};
