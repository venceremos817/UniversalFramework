#include "BinaryReader.h"

struct handle_closer { void operator()(HANDLE h) { if (h)CloseHandle(h); } };
typedef public std::unique_ptr<void, handle_closer> ScopedHandle;
inline HANDLE safe_handle(HANDLE h) { return (h == INVALID_HANDLE_VALUE) ? 0 : h; }

HRESULT BinaryReader::ReadEntireFile(const std::string & fileName, std::unique_ptr<uint8_t[]>& data, size_t * dataSize)
{
	std::wstring widePath = std::wstring(fileName.begin(), fileName.end());
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	ScopedHandle hFile(safe_handle(CreateFile2(widePath.c_str(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, nullptr)));
#else
	ScopedHandle hFile(safe_handle(CreateFileW(widePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)));
#endif // (_WIN32_WINNT >= _WIN32_WINNT_WIN8)

	if (!hFile)
		return HRESULT_FROM_WIN32(GetLastError());

	// サイズ取得
	LARGE_INTEGER fileSize = { 0 };

#if (_WIN32_WINNT >= _WIN32_WINNT_VISTA)
	FILE_STANDARD_INFO fileInfo;
	if (!GetFileInformationByHandleEx(hFile.get(), FileStandardInfo, &fileInfo, sizeof(fileInfo)))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}
	fileSize = fileInfo.EndOfFile;
#else
	GetFileSizeEx(hFile.get(), &fileSize);
#endif // (_WIN32_WINNT >= _WIN32_WINNT_VISTA)

	// ファイルが制限を超えたらエラー
	if (fileSize.HighPart > 0)
		return E_FAIL;

	// 同じサイズのメモリを作成
	data.reset(new uint8_t[fileSize.LowPart]);

	if (!data)
		return E_OUTOFMEMORY;

	// データの読み込み
	DWORD bytesRead = 0;

	if (!ReadFile(hFile.get(), data.get(), fileSize.LowPart, &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (bytesRead < fileSize.LowPart)
		return E_FAIL;
	
	*dataSize = bytesRead;

	return S_OK;
}
