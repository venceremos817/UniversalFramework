#include "Resource.h"
#include <d3d11.h>
#include <mutex>
#include <App.h>
#include <ThrowException.h>

#pragma region TextureResources

struct TextureResouce::Param
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;	// ���\�[�X�r���[
	std::string m_fileName;	// �e�N�X�`���t�@�C���p�X
	std::mutex  m_mutex;		// �~���[�e�b�N�X

	Param(const std::string& fileName, const FileExtension fileExtention);
	~Param() {}
};

TextureResouce::Param::Param(const std::string & fileName, const FileExtension fileExtention)
	: m_fileName(fileName)
{
	try
	{
		DWORD retCode;
		retCode = GetFileAttributes(m_fileName.c_str());
		if (retCode == -1)
		{
			throw StandardException(
				"�t�@�C�������݂��܂���",
				m_fileName,
				"TextureResouces::Param::Param()"
			);
		}

		HRESULT hr;
		// �e�N�X�`���쐬
		DirectX::TexMetadata metaData;
		DirectX::ScratchImage image;
		std::wstring widePath = std::wstring(m_fileName.begin(), m_fileName.end());

		if (FileExtension::WIC == fileExtention)
		{
			char driveBuff[_MAX_DRIVE];		// �h���C�u��
			char dirBuff[_MAX_DIR];			// �f�B���N�g����
			char fileNameBuff[_MAX_FNAME];	// �t�@�C����
			char extBuff[_MAX_EXT];			// �g���q��
			ZeroMemory(driveBuff, sizeof(driveBuff));
			ZeroMemory(dirBuff, sizeof(dirBuff));
			ZeroMemory(fileNameBuff, sizeof(fileNameBuff));
			ZeroMemory(extBuff, sizeof(extBuff));
			// �t�@�C���p�X�𕪉�
			_splitpath_s(m_fileName.c_str(),
				driveBuff, _MAX_DRIVE,
				dirBuff, _MAX_DIR,
				fileNameBuff, _MAX_FNAME,
				extBuff, _MAX_EXT);

			// �g���q���ɓǂݍ��ݕύX
			std::string extStr = extBuff;
			if (".dds" == extStr || ".DDS" == extStr)
			{
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image),
					"�e�N�X�`���ǂݍ��ݎ��s",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
			else if (".tga" == extStr || ".TGA" == extStr)
			{
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(widePath.c_str(), &metaData, image),
					"�e�N�X�`���ǂݍ��ݎ��s",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
			else
			{
				ThrowIfFailed(
					DirectX::LoadFromWICFile(widePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image),
					"�e�N�X�`���ǂݍ��ݎ��s",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
		}
		else if (FileExtension::DDS == fileExtention)
		{
			ThrowIfFailed(
				DirectX::LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image),
				"�e�N�X�`���ǂݍ��ݎ��s",
				m_fileName,
				"TextureResources::Param::Param()"
			);
		}
		else if (FileExtension::TGA == fileExtention)
		{
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(widePath.c_str(), &metaData, image),
				"�e�N�X�`���ǂݍ��ݎ��s",
				m_fileName,
				"TextureResources::Param::Param()"
			);
		}

		auto* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();

		DemandCreate(m_shaderResourceView, m_mutex, [&](ID3D11ShaderResourceView** ppResult)->HRESULT
		{
			// �摜����V�F�[�_�[���\�[�X�r���[���쐬
			hr = ThrowIfFailed(
				DirectX::CreateShaderResourceView(pDX11Device, image.GetImages(), image.GetImageCount(), metaData, ppResult),
				"�V�F�[�_�[���\�[�X�r���[�쐬���s",
				"DirectX::CreateShaderResourceView()",
				"TextureResources::Param::Param()"
			);
			return hr;
		});
	}
	catch (...)
	{
		throw;
	}
}


TextureResouce::TextureResouce(const std::string & fileName, const FileExtension& fileExtention)
	: BaseResource()
	, m_pParam(new Param(fileName, fileExtention))
{
}

TextureResouce::~TextureResouce()
{
}

std::shared_ptr<TextureResouce> TextureResouce::CreateTextureResouce(const std::string & fileName, const FileExtension fileExtention)
{
	try
	{
		return ObjectFactory::Create<TextureResouce>(fileName, fileExtention);
	}
	catch (...)
	{
		throw;
	}
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& TextureResouce::GetShaderResourceView() const
{
	return m_pParam->m_shaderResourceView;
}


#pragma endregion

#pragma region EffekseerEffectResource

struct EffekseerEffectResource::Param
{
	Effekseer::EffectRef effect;	// �G�t�F�N�g
	std::u16string fileName;		// �t�@�C���p�X

	Param(const std::u16string& fileName)
		: fileName(fileName)
	{
		effect = Effekseer::Effect::Create(APP->GetEffekseerManagerRef(), fileName.c_str());
	}
};

EffekseerEffectResource::EffekseerEffectResource(const std::u16string& fileName)
	: BaseResource()
	, m_param(new Param(fileName))
{
}

EffekseerEffectResource::~EffekseerEffectResource()
{
}

std::shared_ptr<EffekseerEffectResource> EffekseerEffectResource::CreateEffekseerResource(const std::u16string & fileName)
{
	return ObjectFactory::Create<EffekseerEffectResource>(fileName);
}

Effekseer::EffectRef EffekseerEffectResource::GetEffectResource()
{
	return m_param->effect;
}

#pragma endregion

