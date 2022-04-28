#pragma once
#include <Windows.h>
#include <string>

/// <summary>
/// �o�C�i���f�[�^�ǂݍ���
/// </summary>
class BinaryReader
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	explicit BinaryReader(const std::string& fileName);


	/// <summary>
	/// �t�@�C�����烁�����ւ̓ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="data">�f�[�^�̎Q��</param>
	/// <param name="dataSize">�ǂݍ��񂾐�</param>
	/// <returns></returns>
	static HRESULT ReadEntireFile(const std::string& fileName, _Inout_ std::unique_ptr<uint8_t[]>& data, _Out_ size_t* dataSize);

};