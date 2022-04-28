#pragma once
#include <Windows.h>
#include <string>

/// <summary>
/// バイナリデータ読み込み
/// </summary>
class BinaryReader
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	explicit BinaryReader(const std::string& fileName);


	/// <summary>
	/// ファイルからメモリへの読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="data">データの参照</param>
	/// <param name="dataSize">読み込んだ数</param>
	/// <returns></returns>
	static HRESULT ReadEntireFile(const std::string& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);

};