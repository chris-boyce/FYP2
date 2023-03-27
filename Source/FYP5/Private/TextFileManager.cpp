// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"

#include "DiffResults.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFileManager.h"

bool UTextFileManager::SaveArrayText(FString SaveDirectory, FString FileName, TArray<FString> SaveText,
	bool AllowOverWriting = false)
{
	SaveDirectory += "\\";
	SaveDirectory += FileName;
	if(!AllowOverWriting)
	{
		if(FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}
	FString FinalString = "";
	for (FString& Each : SaveText)
	{
		FinalString += Each;
		FinalString += LINE_TERMINATOR;
	}
	return  FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

FString UTextFileManager::LoadFileText(FString FileName)
{
	FString Directory = FPaths::ProjectContentDir();
	FString Result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();
	if(file.CreateDirectory(*Directory))
	{
		FString myFile = Directory + "/" + FileName;
		FFileHelper::LoadFileToString(Result,*myFile);
	}
	return Result;
	
}
