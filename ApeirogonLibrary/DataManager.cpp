#include "pch.h"
#include "DataManager.h"

using namespace std;

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	wprintf(L"[DatabaseManager::~DatabaseManager()]\n");
}

bool DataManager::Prepare(const ServicePtr& service)
{
	mService = service;
	if (nullptr == mService)
	{
		return false;
	}

	if (false == InitDatas())
	{
		return false;
	}

	DataManagerLog(L"[DataManager::Prepare()] load datas to success\n");

	return true;
}

void DataManager::Shutdown()
{
	for (CSVDatas& datas : mDatas)
	{
		datas.clear();
	}
}

bool DataManager::GetData(CSVDatas& outData, uint8 inDataNumber)
{
	if (0 <= inDataNumber && inDataNumber < mDatas.size())
	{
		outData = mDatas[inDataNumber];
		return true;
	}

	return false;
}

bool DataManager::GetRow(CSVRow& outRow, uint8 inDataNumber, int32 inRowNumber)
{
	inRowNumber += 1;

	if (!(0 <= inDataNumber && inDataNumber < mDatas.size()))
	{
		return false;
	}

	if (0 <= inRowNumber && inRowNumber < mDatas[inDataNumber].size())
	{
		outRow = mDatas[inDataNumber][inRowNumber];
		return true;
	}

	return false;
}

bool DataManager::PushData(const WCHAR* inFilePath)
{

	DataManagerLog(L"[DataManager::PushData] Load csv file (%ws)\n", inFilePath);

	CSVDatas csvDatas;
	wifstream readStream;
	readStream.open(inFilePath, ios::in);

	if (false == readStream.is_open())
	{
		return false;
	}

	CSVRow	rows;
	wstring row;
	wstring cell;
	while (!readStream.eof())
	{
		getline(readStream, row, L'\n');

		std::wstringstream cells(row);
		while (getline(cells, cell, L','))
		{
			rows.push_back(cell);
		}

		csvDatas.emplace_back(rows);
		rows.clear();
	}

	readStream.close();
	mDatas.emplace_back(std::move(csvDatas));
	return true;
}

void DataManager::DataManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}
