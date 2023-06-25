#pragma once

#define CastCSVData(x) static_cast<uint8>(x);

using CSVRow	= std::vector<std::wstring>;
using CSVDatas	= std::vector<CSVRow>;

class DataManager
{
public:
	APEIROGON_API DataManager(const int32 inMaxDatas);
	APEIROGON_API virtual ~DataManager();

protected:
	DataManager(const DataManager&) = delete;
	DataManager(DataManager&&) = delete;

	DataManager operator=(const DataManager&) = delete;
	DataManager operator=(DataManager&&) = delete;

public:
	bool Prepare(ServicePtr service);
	void Shutdown();

public:
	APEIROGON_API virtual bool InitDatas() abstract;
	APEIROGON_API bool GetData(CSVDatas& outData, uint8 inDataNumber);
	APEIROGON_API bool GetRow(CSVRow& outRow, uint8 inDataNumber, int32 inRowNumber);
	APEIROGON_API CSVRow* PeekRow(uint8 inDataNumber, int32 inRowNumber);

protected:
	APEIROGON_API bool PushData(const WCHAR* inFilePath, const int32 inDataType);
	APEIROGON_API void DataManagerLog(const WCHAR* log, ...);

private:
	ServicePtr				mService;
	int32					mDataCounts;
	std::vector<CSVDatas>	mDatas;
};

