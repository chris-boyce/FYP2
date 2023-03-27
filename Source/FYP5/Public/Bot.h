// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Bot.generated.h"

class ASpawnController;
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
	void RespawnBlueBot(FVector SpawnPos);

	UFUNCTION(BlueprintImplementableEvent , Category="Rounds")
	void RespawnRedBot(FVector SpawnPos);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASpawnController* ReportToController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReactionTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Accuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReverseAccuracy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WinString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SkillRating;

	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void WriteBotDataToFile();

	
	
	
};
