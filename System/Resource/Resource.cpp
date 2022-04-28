#include "Resource.h"
#include <d3d11.h>
#include <mutex>
#include <App.h>
#include <ThrowException.h>

#pragma region TextureResources

struct TextureResouce::Param
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;	// リソースビュー
	std::string m_fileName;	// テクスチャファイルパス
	std::mutex  m_mutex;		// ミューテックス

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
				"ファイルが存在しません",
				m_fileName,
				"TextureResouces::Param::Param()"
			);
		}

		HRESULT hr;
		// テクスチャ作成
		DirectX::TexMetadata metaData;
		DirectX::ScratchImage image;
		std::wstring widePath = std::wstring(m_fileName.begin(), m_fileName.end());

		if (FileExtension::WIC == fileExtention)
		{
			char driveBuff[_MAX_DRIVE];		// ドライブ名
			char dirBuff[_MAX_DIR];			// ディレクトリ名
			char fileNameBuff[_MAX_FNAME];	// ファイル名
			char extBuff[_MAX_EXT];			// 拡張子名
			ZeroMemory(driveBuff, sizeof(driveBuff));
			ZeroMemory(dirBuff, sizeof(dirBuff));
			ZeroMemory(fileNameBuff, sizeof(fileNameBuff));
			ZeroMemory(extBuff, sizeof(extBuff));
			// ファイルパスを分解
			_splitpath_s(m_fileName.c_str(),
				driveBuff, _MAX_DRIVE,
				dirBuff, _MAX_DIR,
				fileNameBuff, _MAX_FNAME,
				extBuff, _MAX_EXT);

			// 拡張子毎に読み込み変更
			std::string extStr = extBuff;
			if (".dds" == extStr || ".DDS" == extStr)
			{
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image),
					"テクスチャ読み込み失敗",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
			else if (".tga" == extStr || ".TGA" == extStr)
			{
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(widePath.c_str(), &metaData, image),
					"テクスチャ読み込み失敗",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
			else
			{
				ThrowIfFailed(
					DirectX::LoadFromWICFile(widePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData, image),
					"テクスチャ読み込み失敗",
					m_fileName,
					"TextureResouces::Param::Param()"
				);
			}
		}
		else if (FileExtension::DDS == fileExtention)
		{
			ThrowIfFailed(
				DirectX::LoadFromDDSFile(widePath.c_str(), DirectX::DDS_FLAGS_NONE, &metaData, image),
				"テクスチャ読み込み失敗",
				m_fileName,
				"TextureResources::Param::Param()"
			);
		}
		else if (FileExtension::TGA == fileExtention)
		{
			ThrowIfFailed(
				DirectX::LoadFromTGAFile(widePath.c_str(), &metaData, image),
				"テクスチャ読み込み失敗",
				m_fileName,
				"TextureResources::Param::Param()"
			);
		}

		auto* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();

		DemandCreate(m_shaderResourceView, m_mutex, [&](ID3D11ShaderResourceView** ppResult)->HRESULT
		{
			// 画像からシェーダーリソースビューを作成
			hr = ThrowIfFailed(
				DirectX::CreateShaderResourceView(pDX11Device, image.GetImages(), image.GetImageCount(), metaData, ppResult),
				"シェーダーリソースビュー作成失敗",
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
	Effekseer::EffectRef effect;	// エフェクト
	std::u16string fileName;		// ファイルパス

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

