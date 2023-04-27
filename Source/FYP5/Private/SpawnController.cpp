// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnController.h"
#include "Bot.h"
#include "Math/Vector.h"

ASpawnController::ASpawnController()
{
 	//Primary Setup on Controller
	PrimaryActorTick.bCanEverTick = true;
	TeamSize = 5;
	BotListBlue.SetNum(TeamSize);
	BotListRed.SetNum(TeamSize);
	
	BotListRedKD.SetNum(TeamSize);
	BotListBlueKD.SetNum(TeamSize);

	IncreasePercentage.SetNum(TeamSize);
	DecreasePercentage.SetNum(TeamSize);
	
}

void ASpawnController::BeginPlay()
{
	Super::BeginPlay();
	RoundNumber = 0;
}

void ASpawnController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void ASpawnController::SpawnAI()
{

	//Spawns Bots and Keeps them in List of Each Team
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 500;
		
		BotListBlue[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointBlue->GetActorLocation() + OffSet , SpawnPointBlue->GetActorRotation());
		if(BotListBlue[i] != nullptr)
		{
			BotListBlue[i]->SetBlueTeam();
			BotListBlue[i]->Accuracy = BlueMatchBots[i].ShotAccuracy;
			BotListBlue[i]->ReverseAccuracy = -BlueMatchBots[i].ShotAccuracy;
			BotListBlue[i]->ReactionTime = BlueMatchBots[i].ReactionTime;
			BotListBlue[i]->ReportToController = this;
			BotListBlue[i]->Name = FString::FromInt(BlueMatchBots[i].IDNum);
		}
	}

	
	for ( int i = 0; i < TeamSize; i++ )
	{
		OffSet.X = i * 500;
		BotListRed[i] = GetWorld()->SpawnActor<ABot>(AIToSpawn, SpawnPointRed->GetActorLocation() + OffSet , SpawnPointRed->GetActorRotation());
		if(BotListRed[i] != nullptr)
		{
			BotListRed[i]->SetRedTeam();
			BotListRed[i]->Accuracy = RedMatchBots[i].ShotAccuracy;
			BotListRed[i]->ReverseAccuracy = -RedMatchBots[i].ShotAccuracy;
			BotListRed[i]->ReactionTime = RedMatchBots[i].ReactionTime;
			BotListRed[i]->ReportToController = this;
			BotListRed[i]->Name = FString::FromInt(RedMatchBots[i].IDNum);
		}
	}
	
	
}
void ASpawnController::CheckEnemysLeft(bool isBlue)
{
	//Check run after Death to see if round is over
	if(isBlue == true)
	{
		DeadCountBlue = DeadCountBlue + 1;
		if(DeadCountBlue == TeamSize)
		{
			Score.X = Score.X + 1;
			ResetRound();
		}
	}
	else
	{
		DeadCountRed = DeadCountRed + 1;
		if(DeadCountRed == TeamSize)
		{
			Score.Y = Score.Y + 1;
			ResetRound();
		}
	}
}

void ASpawnController::ResetRound()
{
	//Reset Of Round Stats
	UE_LOG(LogTemp, Warning, TEXT("Round Reset"));
	DeadCountBlue = 0;
	DeadCountRed = 0;
	RoundNumber = RoundNumber + 1;
	if(RoundNumber == 6)
	{
		FVector temp = SpawnPointBlue->GetActorLocation();
		SpawnPointBlue->SetActorLocation(SpawnPointRed->GetActorLocation());
		SpawnPointRed->SetActorLocation(temp);
	}
	if(Score.X == 7)
	{
		RedTeamWin = true;
	}
	if(Score.Y == 7)
	{
		BlueTeamWin = true;
	}
	if(RedTeamWin == true || BlueTeamWin == true)
	{
		EndGame();
	}
	else
	{
		//Respawn the bot after all 5 die
		for ( int i = 0; i < TeamSize; i++ )
		{
			OffSet.X = i * 500;
			BotListBlue[i]->RespawnBlueBot(SpawnPointBlue->GetActorLocation()+ OffSet);
		}
		for ( int j = 0; j < TeamSize; j++ )
		{
			OffSet.X = j * 500;
			BotListRed[j]->RespawnRedBot(SpawnPointRed->GetActorLocation() + OffSet);
		}
	}

}


FRating ASpawnController::TrueskillCalc(float BotMu, float BotSigma, int Outcome, float TeamMeanMu)
{
	K = 2.5;
	float q = 0.05; // Default value for q
	float c = 2.5; // Default value for c
	int n = 10; // Total number of players in the match
	int k = 4; // Number of teammates on the player's team
	
	float delta_Mu = BotMu - TeamMeanMu;
	float denom = sqrt(2 * (BotMu * BotMu + TeamMeanMu * TeamMeanMu));
	float delta = delta_Mu / denom;

	win_probility = 0.5 * (1 + erf((delta - 0) / (sqrt(2) * 1)));
	
	outputA = BotMu + K * (Outcome - win_probility);

	float new_sigma = sqrt((BotSigma * BotSigma * (n - k) + q * c * c) / (n - 1 + q));
	outputB = new_sigma;
	FRating temp;
	temp.TeamMu = outputA;
	temp.TeamSigma = outputB;
	return temp;

}

void ASpawnController::TeamTrueskillCalc()
{
	for(int i = 0; i < TeamSize; i++)
	{
		BlueTeamRating.TeamMu = BlueTeamRating.TeamMu + ConnectedBlueTeamBotServerData[i].Mu;
	}
	BlueTeamRating.TeamMu = BlueTeamRating.TeamMu / TeamSize ;

	for(int i = 0; i < TeamSize; i++)
	{
		RedTeamRating.TeamMu = RedTeamRating.TeamMu + ConnectedRedTeamBotServerData[i].Mu;
	}
	RedTeamRating.TeamMu = RedTeamRating.TeamMu / TeamSize ;
	
}





/*
void ASpawnController::EloCalc()
{
	e1 = 1.0 * 1.0/ (1+ 1.0* pow(10, 1.0 * (AverageBlueElo - AverageRedElo) / 400));
	e2 = 1.0 * 1.0/ (1+ 1.0* pow(10, 1.0 * (AverageRedElo - AverageBlueElo) / 400));

	if(BlueTeamWin)
	{
		NewBlueRating = AverageBlueElo + 40 * (1 - e1);
		NewRedRating = AverageRedElo + 40 * (0 - e2);
	}
	else
	{
		NewBlueRating = AverageBlueElo + 40 * (0 - e1);
		NewRedRating = AverageRedElo + 40 * (1, e2);
	}
	RedEloChange = NewRedRating - AverageRedElo;
	BlueEloChange = NewBlueRating - AverageBlueElo;

}
*/

void ASpawnController::EndGame()
{
	PercentageBasedSkillRating();
	TArray<int> BotIDs;
	for(int i = 0; i < TeamSize; i++)
	{
		/*
		if(RedTeamWin == true)
		{
			BotListRed[i]->WinString = "Win";
			BotsNewRating = TrueskillCalc(ConnectedRedTeamBotServerData[i].Mu,ConnectedRedTeamBotServerData[i].Sigma, 1, BlueTeamRating.TeamMu );
			ConnectedRedTeamBotServerData[i].Mu = BotsNewRating.TeamMu;
			ConnectedRedTeamBotServerData[i].Sigma = BotsNewRating.TeamSigma;
			
		}
		else
		{
			BotListRed[i]->WinString = "Lose";
			BotsNewRating = TrueskillCalc(ConnectedRedTeamBotServerData[i].Mu,ConnectedRedTeamBotServerData[i].Sigma, 0, BlueTeamRating.TeamMu );
			ConnectedRedTeamBotServerData[i].Mu = BotsNewRating.TeamMu;
			ConnectedRedTeamBotServerData[i].Sigma = BotsNewRating.TeamSigma;
		}
		BotListRed[i]->WriteBotDataToFile(); //Write Match Report
		OnTrueskill.Broadcast(ConnectedRedTeamBotServerData[i].IDNum, BotsNewRating);
		BotIDs.Add(RedMatchBots[i].IDNum);
	}
	for(int i = 0; i < TeamSize; i++)
	{
		
		if(BlueTeamWin == true)
		{
			BotListBlue[i]->WinString = "Win";
			BotsNewRating = TrueskillCalc(ConnectedBlueTeamBotServerData[i].Mu,ConnectedBlueTeamBotServerData[i].Sigma, 1, RedTeamRating.TeamMu );
			ConnectedBlueTeamBotServerData[i].Mu = BotsNewRating.TeamMu;
			ConnectedBlueTeamBotServerData[i].Sigma = BotsNewRating.TeamSigma;
		}
		else
		{
			BotListBlue[i]->WinString = "Lose";
			BotsNewRating = TrueskillCalc(ConnectedBlueTeamBotServerData[i].Mu,ConnectedBlueTeamBotServerData[i].Sigma, 0, RedTeamRating.TeamMu );
			ConnectedBlueTeamBotServerData[i].Mu = BotsNewRating.TeamMu;
			ConnectedBlueTeamBotServerData[i].Sigma = BotsNewRating.TeamSigma;
		}
		BotListBlue[i]->WriteBotDataToFile(); //Write Match Report
		OnTrueskill.Broadcast(ConnectedBlueTeamBotServerData[i].IDNum, BotsNewRating);
		BotIDs.Add(BlueMatchBots[i].IDNum);
		*/
	}
	
	UE_LOG(LogTemp, Warning, TEXT("SpawnerCalled End Game"));
	OnRoundEnd.Broadcast(SpawnControllerIDNum, BotIDs);
}

void ASpawnController::PercentageBasedSkillRating()
{
	for(int i = 0; i < TeamSize; i++)
	{
		BotListBlueKD[i]  = static_cast<float>(BotListBlue[i]->Kills) / static_cast<float>(RoundNumber);
		BotListRedKD[i]  = static_cast<float>(BotListRed[i]->Kills) / static_cast<float>(RoundNumber);
		
		totalKDBlue = totalKDBlue + BotListBlueKD[i];
		totalKDRed = totalKDRed + BotListRedKD[i];
	}
	if(BlueTeamWin)
	{
		for(int i = 0; i < TeamSize; i++)
		{
			IncreasePercentage[i] = (BotListBlueKD[i] / totalKDBlue) * 100;
			DecreasePercentage[i] = (BotListRedKD[i] / totalKDRed) * 100;
			DecreasePercentage[i] = 40 - DecreasePercentage[i];
			BotListBlue[i]->WinString = "Win";
			BotListRed[i]->WinString = "Lose";
			BotListBlue[i]->WriteBotDataToFile(BotListBlueKD[i]);
			BotListRed[i]->WriteBotDataToFile(BotListRedKD[i]);
			OnPertentileChange.Broadcast(ConnectedBlueTeamBotServerData[i].IDNum , IncreasePercentage[i]);
			OnPertentileChange.Broadcast(ConnectedRedTeamBotServerData[i].IDNum , -DecreasePercentage[i]);
		}
	}
	if(RedTeamWin)
	{
		for(int i = 0; i < TeamSize; i++)
		{
			IncreasePercentage[i] = (BotListRedKD[i] / totalKDRed) * 100;
			DecreasePercentage[i] = (BotListBlueKD[i] / totalKDBlue) * 100;
			DecreasePercentage[i] = 40 - DecreasePercentage[i];
			BotListBlue[i]->WinString = "Lose";
			BotListRed[i]->WinString = "Win";
			BotListBlue[i]->WriteBotDataToFile(BotListBlueKD[i]);
			BotListRed[i]->WriteBotDataToFile(BotListRedKD[i]);
			OnPertentileChange.Broadcast(ConnectedRedTeamBotServerData[i].IDNum , IncreasePercentage[i]);
			OnPertentileChange.Broadcast(ConnectedBlueTeamBotServerData[i].IDNum , -DecreasePercentage[i]);
		}
	}
	RestartGame();
}

void ASpawnController::RestartGame()
{
	for(int i = 0; i < TeamSize; i++)
	{
		BotListBlue[i]->Destroy();
		BotListBlue[i] = nullptr;
		
		
	}
	for(int i = 0; i < TeamSize; i++)
	{
		BotListRed[i]->Destroy();
		BotListRed[i] = nullptr;
	}
	IncreasePercentage.Empty();
	DecreasePercentage.Empty();
	IncreasePercentage.SetNum(5);
	DecreasePercentage.SetNum(5);
	totalKDRed = 0;
	totalKDBlue = 0;
	RedTeamWin = false;
	BlueTeamWin = false;
	RoundNumber = 0;
	RedMatchBots.Empty();
	BlueMatchBots.Empty();
	Score.X = 0;
	Score.Y = 0;
	
}
	

