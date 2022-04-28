#pragma once
#include <memory>
#include <DirectXTex.h>
#include <wrl/client.h>
#include <string>
#include <Effekseer/Effekseer.h>

#pragma comment(lib,"DirectXTex.lib")

class ResourceInterface
	: public std::enable_shared_from_this<ResourceInterface>
{
	friend class ObjectFactory;
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	ResourceInterface() {};
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~ResourceInterface() {}

public:
	/// <summary>
	/// �O������
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// ����������
	/// </summary>
	virtual void OnCreate() {};
	bool IsCreated()const { return m_created; }

private:
	void SetCreated(bool b) { m_created = b; }

private:
	bool m_created{ false };

	// �R�s�[�֎~
	ResourceInterface(const ResourceInterface&) = delete;
	ResourceInterface& operator=(const ResourceInterface&) = delete;
	// ���[�u�֎~
	ResourceInterface(const ResourceInterface&&) = delete;
	ResourceInterface& operator=(const ResourceInterface&&) = delete;
};

/// <summary>
/// �������ɕێ����郊�\�[�X�e�N���X
/// </summary>
class BaseResource
	: public ResourceInterface
{
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	BaseResource() {}
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual	~BaseResource() {}
};


/// <summary>
/// �e�N�X�`�����\�[�X
/// </summary>
class TextureResouce
	: public BaseResource
{
	/// <summary>
	/// ObjectFactory����A�N�Z�X��
	/// </summary>
	friend class ObjectFactory;
public:
	/// <summary>
	/// �t�@�C���g���q
	/// </summary>
	enum class FileExtension
	{
		WIC,
		DDS,
		TGA,
	};

private:
	/// <summary>
	/// �v���C�x�[�g�R���X�g���N�^
	/// </summary>
	/// <param name="fileName">�t�@�C���p�X</param>
	/// <param name="fileExtention">�t�@�C���g���q</param>
	explicit TextureResouce(const std::string& fileName, const FileExtension& fileExtention = FileExtension::WIC);

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~TextureResouce();

	/// <summary>
	/// �e�N�X�`�����쐬
	/// </summary>
	/// <param name="fileName">�t�@�C���p�X</param>
	/// <param name="">�t�@�C���g���q</param>
	/// <returns></returns>
	static std::shared_ptr<TextureResouce> CreateTextureResouce(const std::string& fileName, const FileExtension fileExtention = FileExtension::WIC);

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[���擾
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView()const;


private:
	struct Param;
	std::unique_ptr<Param> m_pParam;
};


class EffekseerEffectResource
	: public BaseResource
{
	friend class ObjectFactory;
private:
	explicit EffekseerEffectResource(const std::u16string& fileName);
public:
	virtual ~EffekseerEffectResource();

	static std::shared_ptr<EffekseerEffectResource> CreateEffekseerResource(const std::u16string& fileName);

	Effekseer::EffectRef GetEffectResource();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};