//Server Controller

#include "ServerController.h"
#include "SpawnController.h"
#include "Kismet/GameplayStatics.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

AServerController::AServerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AServerController::BeginPlay()
{
	Super::BeginPlay();
	//Gets all the SpawnController and makes a Map -- Key = ServerIDNum, Data = Actor SpawnController, Bool IsActive
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnController::StaticClass(), ActorSpawnController);
	for(int i = 0; i < ActorSpawnController.Num(); i++)
	{
		FServerStatus temps;
		temps.SpawnController = ActorSpawnController[i];
		temps.IsActive = false;
		ServerStatus.Add(i, temps);
	}
	//Getting Table and Writing it to Array (File Saving) and Map (Live Data to Use)
	BotStatsWriteDataTableToArray();
	BotStatsWriteArrayToMap();
	ServerWriteDataTableToArray();
	ServerWriteArrayToMap();
	//Matchmaking start Chain - LobbyMaker -> LobbySorting -> Server Finding -> CreateMatch
	LobbyMaker();
}

void AServerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AServerController::ServerSelector()
{
	//Goes Through Server till Empty Found -- TODO : Add No Lobby Free
	for(int i = 0; i < ServerStatus.Num(); i++)
	{
		if(ServerStatus[i].IsActive == false)
		{
			CreateMatch(i);
			break;
		}
	}
}

void AServerController::LobbyMaker()
{
	//Goes Through BotServerData if not in game add to Lobby
	for(int i = 0; i < BotServerStatus.Num(); i++)
	{
		if(BotServerStatus[i].InGame == false)
		{
			//Breaks out when lobby is full and goes to Sorting Teams
			if(FullLobby.Num() >= 10) 
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Run Break"));
				break;
			}
			//Gets the first player to lobby then uses them to find percentile TODO - This may need change when split apart more
			if(FullLobby.Num() == 0)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Added first bot"));
				FullLobby.Add(BotServerStatus[i].IDNum);
				TotalSkillRating = (AverageSkillRating + BotServerStatus[i].SkillRating);
				BotServerStatus[i].InGame = true;
				AverageSkillRating = TotalSkillRating;
				LobbyCounter = 1;
			}
			//Adds Bot to Lobby depending if they are close to server avarage - ATM 10%
			else if(PercentageDifference(BotServerStatus[i].SkillRating, AverageSkillRating) < 10)
			{
				UE_LOG(LogTemp, Warning, TEXT("Has Added New Bot"));
				FullLobby.Add(BotServerStatus[i].IDNum);
				LobbyCounter = LobbyCounter + 1;
				BotServerStatus[i].InGame = true;
				TotalSkillRating = (TotalSkillRating + BotServerStatus[i].SkillRating);
				AverageSkillRating = TotalSkillRating / LobbyCounter;
			}
		}
	}
	if(FullLobby.Num() >= 10) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Run Lobby"));
		SortTeams();
	}
}

void AServerController::SortTeams()
{
	for(int i = 0; i < 5; i++)
	{
		RedLobbyBot.Add(FullLobby[i]);
	}
	for(int i = 0; i < 5; i++)
	{
		BlueLobbyBot.Add(FullLobby[i + 5 ]);
	}
	ServerSelector();
}

float AServerController::PercentageDifference(float FirstNum , float SecondNum) //Returns Difference of 2 numbers
{
	float TopDiv = FirstNum - SecondNum;
	float BottomDiv = (FirstNum + SecondNum) / 2;
	float TotalDiv = TopDiv/BottomDiv;
	float Answer = TotalDiv * 100;
	return Answer;
}

void AServerController::CreateMatch(int ServerToConnect)
{
	//Clear SpawnController and then finds the server passed into it via ServerIDNum
	SpawnController = nullptr;
	SpawnController = Cast<ASpawnController>(ServerStatus[ServerToConnect].SpawnController);
	SpawnController->SpawnControllerIDNum = ServerToConnect;
	//Sets the Server to Active and binds the end to call back event for when game is over
	ServerStatus[ServerToConnect].IsActive = true;
	SpawnController->OnRoundEnd.AddDynamic(this, &AServerController::EndGame);

	//Adds Bot Data for Bot Stat Map to a local Map on the Spawn Controller - Then giving the data to the bot spawned
	for(int i = 0; i < RedLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[RedLobbyBot[i]];
		SpawnController->RedMatchBots.Add(i, LocalBotData );
	}
	for(int i = 0; i < BlueLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[BlueLobbyBot[i]];
		SpawnController->BlueMatchBots.Add(i, LocalBotData );
	}
	//Calls the start of the game and spawns the bots once data is passed
	SpawnController->SpawnAI();

	//Reset The Server Stats for reruns
	FullLobby.Empty();
	RedLobbyBot.Empty();
	BlueLobbyBot.Empty();
	AverageSkillRating = 0;
	TotalSkillRating = 0;
	LobbyCounter = 0;

	if(IsEmptyServers())
	{
		LobbyMaker();
	}
}

bool AServerController::IsEmptyServers()
{
	for (int i = 0; i < ServerStatus.Num(); i++)
	{
		if(ServerStatus[i].IsActive == false)
		{
			return true;
		}
	}
	return false;
}

void AServerController::EndGame(int ServerID , TArray<int>BotID)//Needs to pass in Server ID and All Bot IDs
{
	ServerStatus[ServerID].IsActive = false;
	UE_LOG(LogTemp, Warning, TEXT("Server Says Game Over"));
}



