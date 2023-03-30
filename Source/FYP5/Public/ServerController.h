//Server Controller - Controls the game start and finish - 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "ServerController.generated.h"
//I Know this is basically Stupid
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
USTRUCT(BlueprintType)
struct FServerStatus
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* SpawnController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsActive;
	
};
//End of Stupid

class ASpawnController;
UCLASS()
class FYP5_API AServerController : public AActor
{
	GENERATED_BODY()
	
public:	
	AServerController();
	virtual void Tick(float DeltaTime) override;

	//MatchMaking -- Finds the players 10 percent from lobby Average -- Can Change
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	void LobbyMaker();
	//Sorts the collected players into Teams
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	void SortTeams();
	//Finds a Empty Server for the player
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	void ServerSelector();
	//Starts the match Needs Server ID
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	void CreateMatch(int ServerToConnect);
	//Starts the match Needs Server ID
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	bool IsEmptyServers();
	//Match Making Variables
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float AverageSkillRating;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float TotalSkillRating;
	
	UFUNCTION(BlueprintCallable , Category="MatchMaking")
	void OrderMap();

	
	
	//Temp Variables To Pass Data Around Inside This Script
	FBotData LocalBotData;
	TArray<AActor*> ActorSpawnController;
	ASpawnController* SpawnController;
	int ServerToLoad;
	float AverageRedElo;
	float AverageBlueElo;
	
	//Used to check the diff of players in MM
	UFUNCTION(BlueprintCallable, Category="Math")
	float PercentageDifference(float FirstNum, float SecNum);
	
	//Lobby Set Up Take Bot Id Nums
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> RedLobbyBot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> BlueLobbyBot;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<int> FullLobby;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int LobbyCounter;

	//Called By the SpawnController - when ended
	UFUNCTION(BlueprintCallable , Category="Event Reciever")
	void EndGame(int ServerID , TArray<int>BotID);

	UFUNCTION(BlueprintCallable , Category="Elo Changing")
	void ChangeElo(int BotID, float EloChange);
	
	//Server Map (Spawn Controller, If Server is Active)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FServerStatus> ServerStatus;
	//Live Map Controlling the bot Data (Bot ID, Reaction Time, Accuracy)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotData> BotStatsMap;
	//Live Map of Bot data (Bot ID, Ingame Status, Games Player, Skill Rating)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotSeverData> BotServerStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<int, FBotSeverData> OrderMapBotServerData;
	
	//BOT STUFF -- Blueprint
	//Writes Array from Data Table
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsWriteDataTableToArray();
	//Makes Map from the array
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsWriteArrayToMap();
	//Edits the bot data and stats - Use is limited until bot change stats over time
	UFUNCTION(BlueprintImplementableEvent , Category="FileWriting")
	void BotStatsEditDataInMapAndArray(int IDNum, float ReactionTime, float Accuracy);

	//SERVER STUFF -- Blueprint
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
