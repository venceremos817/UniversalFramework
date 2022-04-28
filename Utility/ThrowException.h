#pragma once
#include <string>
#include <Windows.h>

/// <summary>
/// 例外throwクラス
/// </summary>
class StandardException
	: public std::exception
{
	std::string m_message;
public:
	StandardException(const std::string& m1, const std::string& m2, const std::string& m3)
	{
		m_message = m1;
		m_message += m2;
		m_message += "\r\n";
		m_message += m2;
		m_message += "\r\n";
		m_message += m3;
		m_message += "\r\n";
	}
};

/// <summary>
/// ランタイム例外をthrowする
/// </summary>
/// <param name="m1"></param>
/// <param name="m2"></param>
/// <param name="m3"></param>
inline void ThrowBaseException(const std::string& m1, const std::string& m2, const std::string& m3)
{
	std::string message = m1;
	message += "\r\n";
	message += m2;
	message += "\r\n";
	message += m3;
	throw std::runtime_error(message);
}

/// <summary>
/// インターフェイスの例外
/// </summary>
/// <param name="hr"></param>
/// <param name="str1"></param>
/// <param name="str2"></param>
/// <param name="str3"></param>
/// <returns></returns>
inline HRESULT ThrowIfFailed(HRESULT hr, const std::string& str1, const std::string& str2, const std::string& str3)
{
	if (FAILED(hr))
	{
		throw StandardException(str1, str2, str3);
	}
	return hr;
}