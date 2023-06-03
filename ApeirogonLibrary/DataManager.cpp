#include "pch.h"
#include "DataManager.h"

using namespace std;

DataManager::DataManager()
{
}

DataManager::~DataManager()
{
	wprintf(L"[DataManager::~DataManager()]\n");
}

bool DataManager::Prepare(ServicePtr service)
{
	this->mService = service;
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

	DataManagerLog(L"[DataManager::Shutdown()] Data manager successfully shutdown\n");
	mService.reset();
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

CSVRow* DataManager::PeekRow(uint8 inDataNumber, int32 inRowNumber)
{
	if (!(0 <= inDataNumber && inDataNumber < mDatas.size()))
	{
		return nullptr;
	}

	if (0 <= inRowNumber && inRowNumber < mDatas[inDataNumber].size())
	{
		return &mDatas[inDataNumber][inRowNumber];
	}

	return nullptr;
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

		csvDatas.push_back(rows);
		rows.clear();
	}

	this->mDatas.push_back(csvDatas);

	for (CSVRow& row : csvDatas)
	{
		row.clear();
	}
	csvDatas.clear();

	readStream.close();
	return true;
}

void DataManager::DataManagerLog(const WCHAR* log, ...)
{
	mService->GetLoggerManager()->WriteLog(log);
}