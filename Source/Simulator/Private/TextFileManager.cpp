// Fill out your copyright notice in the Description page of Project Settings.


#include "TextFileManager.h"

//Saves an array of string to a text file. The boolean result of operation is returned.
bool UTextFileManager::SaveArrayStringsToFile(FString SaveDirectory, FString FileName, TArray<FString> ArrayStrings,
	bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	return FFileHelper::SaveStringArrayToFile(ArrayStrings, *SaveDirectory);
}

//Returns a JSON string using a FJsonObject build with the specified SimulationConfiguration and SimulationData
FString UTextFileManager::SimulationDataToJsonString(FS_SimulationConfigurationAux SimulationConfiguration,
	TArray<FS_SimulationDataPerTimeAux> ArraySimulationDataPerTime)
{
	//Create JsonObject that is going to contain the whole JSON structure
	TSharedPtr< FJsonObject > GeneralJsonObject = MakeShareable(new FJsonObject);

	//Convert SimulationConfiguration to JSON format and save it in JsonObject
	TSharedRef<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	FJsonObjectConverter::UStructToJsonObject(FS_SimulationConfigurationAux::StaticStruct(), &SimulationConfiguration, JsonObject, 0, 0);

	//Add item "simulationConfiguration" to GeneralJsonObject 
	GeneralJsonObject->SetField(TEXT("simulationConfiguration"), MakeShareable(new FJsonValueObject(JsonObject)));

	//Create array of JsonValue to save each data row of ArraySimulationDataPerTime
	TArray< TSharedPtr<FJsonValue> > JsonValueArray;

	TSharedRef<FJsonObject> JsonObjectItem = MakeShareable(new FJsonObject);
	TSharedRef< FJsonValueObject > JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));

	for (FS_SimulationDataPerTimeAux& Each : ArraySimulationDataPerTime)
	{
		//Convert a row in ArraySimulationDataPerTime to JSON format and save it in JsonObjectItem
		JsonObjectItem = MakeShareable(new FJsonObject);
		FJsonObjectConverter::UStructToJsonObject(FS_SimulationDataPerTimeAux::StaticStruct(), &Each, JsonObjectItem, 0, 0);

		//Save JsonObjectItem as JsonValue and add this to the array
		JsonValue = MakeShareable(new FJsonValueObject(JsonObjectItem));
		JsonValueArray.Add(JsonValue);
	}

	//Add item "simulationData" to GeneralJsonObject. 
	//This item contains the values of each row in ArraySimulationDataPerTime
	GeneralJsonObject->SetArrayField("simulationData", JsonValueArray);

	//Convert GeneralJsonObject to string and save it in OutputString
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(GeneralJsonObject.ToSharedRef(), Writer);

	return OutputString;
}

//Saves a string to a text file. The boolean result of operation is returned.
bool UTextFileManager::SaveStringToFile(FString SaveDirectory, FString FileName, FString StringToSave,
	bool AllowOverWriting = false)
{

	//Set complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	if (!AllowOverWriting) {

		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory)) {
			return false;
		}

	}

	return FFileHelper::SaveStringToFile(StringToSave, *SaveDirectory);
}