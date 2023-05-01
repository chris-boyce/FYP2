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
	FirstTime = 0;
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
	
	//SaveBotStatsToNewFileAtEndLevel();
	ServerWriteDataTableToArrayTS();
	ServerWriteArrayToMapTS();
	OrderMap();
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
	for (auto& Elem : OrderMapBotServerData)
	{
		if(FullLobby.Num() >= 10) 
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Run Break"));
			break;
		}
		if(!Elem.Value.InGame)
		{
			FullLobby.Add(Elem.Value.IDNum);
			Elem.Value.InGame = true;
		}
	}
	
	if(FullLobby.Num() >= 10) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Run Lobby!!!"));
		SortTeams();
	}
}

void AServerController::SortTeams()
{
	if (FullLobby.Num() > 0)
	{
		int32 LastIndex = FullLobby.Num() - 1;
		for (int32 i = 0; i <= LastIndex; ++i)
		{
			int32 Index = FMath::RandRange(i, LastIndex);
			if (i != Index)
			{
				FullLobby.Swap(i, Index);
			}
		}
	}

	
	for(int i = 0; i < 5; i++)
	{
		RedLobbyBot.Add(FullLobby[i]);
		AverageRedElo += BotServerStatusTS[FullLobby[i]].Mu;
	}
	for(int i = 0; i < 5; i++)
	{
		BlueLobbyBot.Add(FullLobby[i + 5 ]);
		AverageBlueElo += BotServerStatusTS[FullLobby[i + 5]].Mu;
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
	if(FirstTime < 10)
	{
		UE_LOG(LogTemp, Warning, TEXT("Bound Event"));
		SpawnController->OnPertentileChange.AddDynamic(this, &AServerController::PercetileSkill);
		FirstTime++;
	}
	
	
	//Sets the Server to Active and binds the end to call back event for when game is over
	ServerStatus[ServerToConnect].IsActive = true;
	SpawnController->OnRoundEnd.AddDynamic(this, &AServerController::EndGame);
	SpawnController->OnEloChange.AddDynamic(this, &AServerController::ChangeElo);
	SpawnController->OnTrueskill.AddDynamic(this, &AServerController::ChangeTrueSkill);
	


	//Adds Bot Data for Bot Stat Map to a local Map on the Spawn Controller - Then giving the data to the bot spawned
	for(int i = 0; i < RedLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[RedLobbyBot[i]];
		SpawnController->RedMatchBots.Add(i, LocalBotData );
		SpawnController->ConnectedRedTeamBotServerData.Add(i, BotServerStatusTS[RedLobbyBot[i]]);
	}
	for(int i = 0; i < BlueLobbyBot.Num(); i++)
	{
		LocalBotData = BotStatsMap[BlueLobbyBot[i]];
		SpawnController->BlueMatchBots.Add(i, LocalBotData );
		SpawnController->ConnectedBlueTeamBotServerData.Add(i, BotServerStatusTS[BlueLobbyBot[i]]);
	}
	SpawnController->AverageBlueElo = AverageBlueElo / 5;
	SpawnController->AverageRedElo = AverageRedElo / 5;
	
	//Calls the start of the game and spawns the bots once data is passed
	SpawnController->SpawnAI();
	
	//Reset The Server Stats for reruns
	AverageRedElo = 0;
	AverageBlueElo = 0;
	FullLobby.Empty();
	RedLobbyBot.Empty();
	BlueLobbyBot.Empty();
	AverageSkillRating = 0;
	TotalSkillRating = 0;

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


void AServerController::OrderMap()
{
	
	OrderMapBotServerData = BotServerStatus;
	if(FirstRun == true)
	{
		OrderMapBotServerData.ValueSort([] (FBotSeverData a, FBotSeverData b)
		{
		return a.SkillRating < b.SkillRating;
		});
	}
	
	/*
	OrderMapBotServerDataTS = BotServerStatusTS;
	if(FirstRun == true)
	{
		OrderMapBotServerDataTS.ValueSort([] (FBotSeverDataTrueSkill a, FBotSeverDataTrueSkill b)
		{
			return a.Mu < b.Mu;
		});
		
	}
	*/
	FirstRun = true;
	
	
}

void AServerController::EndGame(int ServerID , TArray<int>BotID)//Needs to pass in Server ID and All Bot IDs
{
	ServerStatus[ServerID].IsActive = false;
	for (int i = 0; i < BotID.Num(); i++)
	{
		BotServerStatusTS[BotID[i]].InGame = false;
		BotServerStatusTS[BotID[i]].GamesPlayed += 1;
	}
	RefreshArray();
	int count = 0;
	for (int i = 0; i < ServerStatus.Num(); i++)
	{
		if(ServerStatus[i].IsActive == false)
		{
			count += 1;
		}
	}
	if(count >= ServerStatus.Num()) //Once all servers are empty
	{
		//Deletes Left Over Bots
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABot::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Deleted Left Over Bot"));
			Actor->Destroy();
		}
		
		LevelSave++;
		SaveBotStatsToNewFileAtEndLevel();
		OrderMap();
		LobbyMaker();
		UE_LOG(LogTemp, Warning, TEXT("RESTARTING THE SERVER FINDING"));
	}
	
}


void AServerController::ChangeElo(int BotID, float EloChange)
{
	//BotServerStatus[BotID].SkillRating += EloChange;
	UE_LOG(LogTemp, Warning, TEXT("Elo Changed Called"));
}

void AServerController::PercetileSkill(int BotID, float Change)
{
	BotServerStatus[BotID].SkillRating += Change;
	UE_LOG(LogTemp, Warning, TEXT("Percentile Changed Called"));
	if (BotServerStatus[BotID].SkillRating < 0.0f)
	{
		BotServerStatus[BotID].SkillRating = 0;
	}
	
}

void AServerController::ChangeTrueSkill(int BotID, FRating Data)
{
	ServerEditDataInMapAndArrayTS(BotID, false, Data.TeamMu, Data.TeamSigma);
	UE_LOG(LogTemp, Warning, TEXT("TrueSkill Changed Called"));
}



