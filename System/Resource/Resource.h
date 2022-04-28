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
	/// プロテクトコンストラクタ
	/// </summary>
	ResourceInterface() {};
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~ResourceInterface() {}

public:
	/// <summary>
	/// 前初期化
	/// </summary>
	virtual void OnPreCreate() {}
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void OnCreate() {};
	bool IsCreated()const { return m_created; }

private:
	void SetCreated(bool b) { m_created = b; }

private:
	bool m_created{ false };

	// コピー禁止
	ResourceInterface(const ResourceInterface&) = delete;
	ResourceInterface& operator=(const ResourceInterface&) = delete;
	// ムーブ禁止
	ResourceInterface(const ResourceInterface&&) = delete;
	ResourceInterface& operator=(const ResourceInterface&&) = delete;
};

/// <summary>
/// メモリに保持するリソース親クラス
/// </summary>
class BaseResource
	: public ResourceInterface
{
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	BaseResource() {}
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual	~BaseResource() {}
};


/// <summary>
/// テクスチャリソース
/// </summary>
class TextureResouce
	: public BaseResource
{
	/// <summary>
	/// ObjectFactoryからアクセス可
	/// </summary>
	friend class ObjectFactory;
public:
	/// <summary>
	/// ファイル拡張子
	/// </summary>
	enum class FileExtension
	{
		WIC,
		DDS,
		TGA,
	};

private:
	/// <summary>
	/// プライベートコンストラクタ
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <param name="fileExtention">ファイル拡張子</param>
	explicit TextureResouce(const std::string& fileName, const FileExtension& fileExtention = FileExtension::WIC);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~TextureResouce();

	/// <summary>
	/// テクスチャを作成
	/// </summary>
	/// <param name="fileName">ファイルパス</param>
	/// <param name="">ファイル拡張子</param>
	/// <returns></returns>
	static std::shared_ptr<TextureResouce> CreateTextureResouce(const std::string& fileName, const FileExtension fileExtention = FileExtension::WIC);

	/// <summary>
	/// シェーダーリソースビューを取得
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