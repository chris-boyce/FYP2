//Server Controller - Controls the game start and finish - 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ServerController.generated.h"


USTRUCT(BlueprintType)
struct FBotData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IDNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ReactionTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShotAccuracy;
};
USTRUCT(BlueprintType)
struct FBotSeverData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int IDNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool InGame;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GamesPlayed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillRating;
};

class ASpawnController;
UCLASS()
class FYP5_API AServerController : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerController();
	virtual void Tick(float DeltaTime) override;

	FBotData LocalBotData;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ASpawnController* SpawnController;

	UFUNCTION(BlueprintCallable , Category="Event Reciever")
	void CreateMatch();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> RedLobbyBot;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> BlueLobbyBot;

	//Called By the SpawnController
	UFUNCTION(BlueprintCallable , Category="Event Reciever")
	void EndGame();

	//Live Maps Controlling the Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotData> BotStatsMap;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotSeverData> BotServerStatus;
	
	//BOT STUFF
	//Writes Array from Data Table
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsWriteDataTableToArray();
	//Makes Map from the array
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsWriteArrayToMap();
	//Edits the bot data and stats - Use is limited until bot change stats over time
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsEditDataInMapAndArray(int IDNum, float ReactionTime, float Accuracy);

	//SERVER STUFF
	//Start of Game Loads into a array to be used later to write to
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void ServerWriteDataTableToArray();
	//Used to enter data into a map
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void ServerWriteArrayToMap();
	//Used to edit both array and data map
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void ServerEditDataInMapAndArray(int IDNum, int GamesPlayed, float SkillRating);
	//Edit just the map
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void ServerEditMap(int IDNum, int GamesPlayed, float SkillRating);
	//Runs at end to put all the values from data map to array to the CSV for reimport
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void RefreshArray();

	
	
protected:
	virtual void BeginPlay() override;
	
};
