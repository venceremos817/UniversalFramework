#pragma once
#include <Component.h>
#include <App.h>
#include <System/Resource/Shader.h>



class MeshResource;

/// <summary>
/// 描画コンポーネントインターフェイスクラス
/// </summary>
class DrawComponentBase
	: public Component
{
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	explicit DrawComponentBase(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~DrawComponentBase();

public:

	virtual void Draw() = 0;

	/// <summary>
	/// ブレンドステートを取得
	/// </summary>
	virtual BlendState GetBlendState()const;
	/// <summary>
	/// デプスステンシルステートを取得
	/// </summary>
	virtual DepthStencilState GetDepthStencilState()const;

	/// <summary>
	/// ラスタライザステートを取得
	/// </summary>
	virtual RasterizerState GetRasterizerState()const;
	/// <summary>
	/// サンプラーステートを取得
	/// </summary>
	virtual SamplerState GetSamplerState()const;

	/// <summary>
	/// ブレンドステートを設定
	/// </summary>
	/// <param name="state"></param>
	virtual void SetBlendState(const BlendState& blendState);
	/// <summary>
	/// デプスステンシルステートを設定
	/// </summary>
	virtual void SetDepthStencilState(const DepthStencilState& depthStencilState);
	/// <summary>
	/// ラスタライザステートを設定
	/// </summary>
	/// <param name="rasterizerState"></param>
	virtual void SetRasterizerState(const RasterizerState& rasterizerState);
	/// <summary>
	/// サンプラーステートを設定
	/// </summary>
	/// <param name="samplerState"></param>
	virtual void SetSamplerState(const SamplerState& samplerState);

	/// <summary>
	/// ブレンドステートをデバイスに設定
	/// </summary>
	virtual void BindDeviceBlendState();
	/// <summary>
	/// デプスステンシルステートをデバイスに設定
	/// </summary>
	virtual void BindDeviceDepthStencilState();
	/// <summary>
	/// ラスタライザステートをデバイスに設定
	/// </summary>
	virtual void BindDeviceRasterizerState();
	/// <summary>
	/// サンプラーステートをデバイスに設定
	/// </summary>
	virtual void BindDeviceSamplerState();

	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完する行列を取得
	/// </summary>
	virtual const Float4x4& GetMeshOffsetTransform()const;
	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完する行列を設定
	/// </summary>
	virtual void SetMeshOffsetTransform(const Float4x4& offsetTransform);

	/// <summary>
	/// メッシュリソースを取得
	/// </summary>
	virtual std::shared_ptr<MeshResource> GetMeshResource()const;
	/// <summary>
	/// メッシュリソースを設定
	/// </summary>
	virtual void SetMeshResource(const std::shared_ptr<MeshResource>& meshRes);

	/// <summary>
	/// テクスチャリソースを設定
	/// </summary>
	virtual void SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, UINT slot);
	virtual void SetTextureResource(const std::string textureKey, UINT slot);
	/// <summary>
	/// テクスチャリソースを取得
	/// </summary>
	virtual std::shared_ptr<TextureResouce> GetTextureResource(UINT slot)const;

	/// <summary>
	/// エミッシブを取得
	/// </summary>
	virtual Float4 GetEmissive()const;
	/// <summary>
	/// エミッシブを設定
	/// </summary>
	virtual void SetEmissive(const Float4& emissive);

	/// <summary>
	/// ディフューズを取得
	/// </summary>
	virtual Float4 GetDiffuse()const;
	/// <summary>
	/// ディフューズを設定
	/// </summary>
	/// <param name="diffuse"></param>
	virtual void SetDiffuse(const Float4& diffuse);

	virtual Float4 GetAmbientLightColor()const;
	virtual void SetAmbientLightColor(const Float4& ambientLightColor);

	/// <summary>
	/// スペキュラーを取得
	/// </summary>
	virtual Float4 GetSpecular()const;
	/// <summary>
	/// スペキュラーを設定
	/// </summary>
	virtual void SetSpecular(const Float4& specular);
	/// <summary>
	/// スペキュラーパワーを取得
	/// </summary>
	virtual float GetSpecularPower()const;
	/// <summary>
	/// スペキュラーパワーを設定
	/// </summary>
	virtual void SetSpecularPower(float power);
	/// <summary>
	/// スペキュラーをなくす
	/// </summary>
	virtual void DisableSpecular();

	/// <summary>
	/// アルファ値を取得
	/// </summary>
	virtual float GetAlpha()const;
	/// <summary>
	/// アルファ値を設定
	/// </summary>
	virtual void SetAlpha(float a);

	/// <summary>
	/// カラーとアルファを取得
	/// </summary>
	/// <returns>Float4(diffuse.xyz,alpha)</returns>
	virtual Float4 GetColorAndAlpha()const;
	/// <summary>
	/// カラーとアルファ値を設定
	/// </summary>
	virtual void SetColorAlpha(const Float4& color);

	/// <summary>
	/// シェーダーを設定
	/// </summary>
	template<class T_VShader, class T_PShader>
	void SetShader()
	{
		SetVertexShader(T_VShader::GetInstance()->GetShader(),
			T_VShader::GetInstance()->GetInputLayout());
		SetPixelShader(T_PShader::GetInstance()->GetShader());
	}

	/// <summary>
	/// 頂点シェーダを設定
	/// </summary>
	void SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pIL);
	/// <summary>
	/// 頂点シェーダを取得
	/// </summary>
	/// <returns></returns>
	ID3D11VertexShader* GetVertexShader();
	/// <summary>
	/// インプットレイアウトを取得
	/// </summary>
	/// <returns></returns>
	ID3D11InputLayout* GetInputLayout();
	/// <summary>
	/// ピクセルシェーダを設定
	/// </summary>
	void SetPixelShader(ID3D11PixelShader* pPS);
	ID3D11PixelShader* GetPixelShader();

	/// <summary>
	/// UVスケールを設定
	/// </summary>
	/// <param name="scale"></param>
	void SetUVScale(const Float2& scale);
	/// <summary>
	/// UVスケールを取得
	/// </summary>
	/// <returns></returns>
	const Float2& GetUVScale();

	/// <summary>
	/// ライティングを有効にするか設定
	/// </summary>
	/// <param name="b"></param>
	void SetLightingEnable(bool b);
	/// <summary>
	/// ライティングが有効かどうか取得
	/// </summary>
	/// <returns></returns>
	bool IsLightingEnable();

	/// <summary>
	/// 影を描画するかどうかを設定
	/// </summary>
	/// <param name="b"></param>
	void SetOwnShadowActive(bool b);
	/// <summary>
	/// 影を描画するかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsOwnShadowActive();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


/// <summary>
/// マテリアル
/// </summary>
struct Material
{
	UINT m_startIndex;	// 開始インデックス
	UINT m_indexCount;	// インデックスカウント
	Float4 m_diffuse;	// ディフューズ
	Float4 m_specular;	// スペキュラー
	Float4 m_ambient;	// アンビエント
	Float4 m_emissive;	// エミッシブ
	std::shared_ptr<TextureResouce> m_textureResource;	// テクスチャリソース
};

/// <summary>
/// 頂点ごとのスキニング情報
/// </summary>
struct SkinPrimData
{
	uint32_t indexes[4];
	float weights[4];
};

/// <summary>
/// メッシュデータ
/// </summary>
struct MeshPrimData
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuff;	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuff;	// インデックスバッファ
	UINT m_verticesNum;	// 頂点数
	UINT m_indexesNum;	// インデックス数
	std::type_index m_meshTypeIndex;	// メッシュの形のインデックス
	UINT m_strideNum;	// ストライド数
	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;	// 描画トポロジー
	std::shared_ptr<BackupDataBase> m_backupData;	// バックアップデータ
	Float4x4 m_meshOffsetTransform;			// メッシュとオブジェクトの差を補完する行列(メッシュ単位で設定する場合利用)
	bool m_useMeshOffsetTransform;			// メッシュとオブジェクトの差を保管する行列を利用するか
	std::weak_ptr<TextureResouce> m_textureResource;	// テクスチャリソース(メッシュ単位で設定する場合利用)
	std::vector<Material> m_materialVec;	// マテリアル配列(モデルで使用)

	bool m_isSkining;		// ボーンかどうか
	UINT m_boneNum;			// ボーンの数
	UINT m_sampleNum;		// サンプリング数
	std::vector<Float4x4> m_sampleMatrixVec;	// サンプリングされたボーン行列
	UINT m_multiMeshIndex;	// マルチメッシュの場合のメッシュインデックス
	std::vector<VertexPosition> m_vertices;	// 汎用に使えるローカル頂点配列
	std::vector<SkinPrimData> m_skins;	// スキニング情報

	MeshPrimData()
		: m_isSkining(false), m_boneNum(0), m_sampleNum(0)
		, m_meshTypeIndex(typeid(VertexPosition))
		, m_strideNum(sizeof(VertexPosition))
		, m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, m_meshOffsetTransform(), m_useMeshOffsetTransform(false)
		, m_multiMeshIndex(0)
	{}

};


/// <summary>
/// メッシュリソース
/// </summary>
class MeshResource
	: public BaseResource
{
	friend class ObjectFactory;
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MeshResource();

	/// <summary>
	/// メッシュデータを取得
	/// </summary>
	/// <returns></returns>
	const MeshPrimData& GetMeshData()const;
	/// <summary>
	/// メッシュデータを設定
	/// </summary>
	/// <param name="meshData"></param>
	void SetMeshData(const MeshPrimData& meshData);

	/// <summary>
	/// 頂点バッファを取得
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer()const;
	/// <summary>
	/// 頂点数を取得
	/// </summary>	
	UINT GetVerticesNum()const;
	/// <summary>
	/// インデックスバッファを取得
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer()const;
	/// <summary>
	/// インデックス数を取得
	/// </summary>	
	UINT GetindexesNum()const;
	/// <summary>
	/// ストライド数を取得
	/// </summary>	
	UINT GetStridesNum()const;

	/// <summary>
	/// 描画トポロジーを取得
	/// </summary>
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology()const;
	/// <summary>
	/// 描画トポロジーを設定
	/// </summary>
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	/// <summary>
	/// type_indexを取得
	/// </summary>
	std::type_index GetTypeIndex()const;
	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完する行列を取得
	/// </summary>
	const Float4x4& GetMeshOffsetTransform()const;
	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完する行列を設定
	/// </summary>
	void SetMeshOffsetTransform(const Float4x4& offsetTransform);
	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完するか
	/// </summary>
	bool IsUseMeshOffsetTransform();
	/// <summary>
	/// メッシュとゲームオブジェクトの差を補完するかどうか設定
	/// </summary>
	void SetUseMeshOffsetTransform(bool useOffsetTransform);
	/// <summary>
	/// テクスチャリソースを設定(メッシュ毎の場合)
	/// </summary>
	/// <param name="textureResource"></param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource);
	/// <summary>
	/// テクスチャリソースを設定(メッシュ毎の場合)
	/// </summary>
	/// <param name="textureKey"></param>
	void SetTextureResource(const std::string& textureKey);
	/// <summary>
	/// テクスチャリソースを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> GetTextureResource()const;
	/// <summary>
	/// マテリアル配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<Material>& GetMaterialVec()const;


	/// <summary>
	/// バッファを作成する
	/// </summary>
	/// <param name="pDX11Device">デバイス</param>
	/// <param name="pBufferDesc">バッファデスク</param>
	/// <param name="pDataDesc">データデスク</param>
	/// <param name="ppBuffer">頂点バッファ</param>
	static void CreatePrimitiveBuffer(ID3D11Device* pDX11Device, const D3D11_BUFFER_DESC* pBufferDesc, const D3D11_SUBRESOURCE_DATA* pDataDesc, ID3D11Buffer** ppBuffer);
	/// <summary>
	/// バッファを作成
	/// </summary>
	/// <param name="pDX11Device">デバイス</param>
	/// <param name="data">データ</param>
	/// <param name="bindFlag">バインドフラグ</param>
	/// <param name="ppBuffer">バッファ</param>
	template<class T>
	static void CreatePrimitiveBuffer(ID3D11Device* pDX11Device, const T& data, D3D11_BIND_FLAG bindFlag, ID3D11Buffer** ppBuffer)
	{
		D3D11_BUFFER_DESC buffDesc = { 0 };
		buffDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		buffDesc.BindFlags = bindFlag;
		buffDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if (FAILED(pDX11Device->CreateBuffer(&buffDesc, &dataDesc, ppBuffer)))
		{
			throw StandardException(
				"バッファ作成失敗",
				"if(FAILED(pDX11Device->CreateBuffer()))",
				"MeshResource::CreatePrimitiveBuffer()"
			);
		}
	}

	/// <summary>
	/// 頂点バッファを作成
	/// </summary>
	/// <param name="vertexBuffer">返されるバッファ</param>
	/// <param name="data">データ</param>
	template<class T>
	static void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(vertexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// 頂点バッファ作成
			CreatePrimitiveBuffer(pDX11Device, data, D3D11_BIND_VERTEX_BUFFER, ppResult);
		});
	}

	/// <summary>
	/// インデックスバッファを作成
	/// </summary>
	/// <param name="indexBuffer">返されるバッファ</param>
	/// <param name="data">データ</param>
	template<class T>
	static void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(indexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// インデックスバッファ作成
			CreatePrimitiveBuffer(pDX11Device, data, D3D11_BIND_INDEX_BUFFER, ppResult);
		});
	}

	/// <summary>
	/// 頂点データを書き換えられる頂点バッファを作成
	/// </summary>
	/// <param name="pDX11Device">デバイス</param>
	/// <param name="data">データ</param>
	/// <param name="ppBuffer">返されるバッファ</param>
	template<class T>
	static void CreateDynamicPrimitiveVertexBuffer(ID3D11Device* pDX11Device, const T& data, ID3D11Buffer** ppBuffer)
	{
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if (FAILED(pDX11Device->CreateBuffer(&bufferDesc, &dataDesc, ppBuffer)))
		{
			throw StandardException(
				"バッファ作成失敗",
				"if(FAILED(pDX11Device->CreateBuffer()))",
				"MeshResource::CreateDynamicPrimitiveVertexBuffer()"
			);
		}
	}

	/// <summary>
	/// 頂点データを書き換えられる頂点バッファを作成
	/// </summary>
	/// <param name="vertexBuffer">返されるバッファ</param>
	/// <param name="data">データ</param>
	template<class T>
	static void CreateDynamicVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(vertexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// 頂点バッファを作成
			CreateDynamicPrimitiveVertexBuffer(pDX11Device, data, ppResult);
		});
	}

	/// <summary>
	/// 矩形頂点を作成
	/// </summary>
	/// <param name="size"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateSquareVertices(float size,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// 立方体頂点を作成
	/// </summary>
	/// <param name="size"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCubeVertices(float size,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// 球頂点を作成
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateSphereVertices(float diameter, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// カプセル頂点を作成
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCapsuleVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// シリンダー頂点を作成
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCylinderVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// コーン頂点を作成
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateConeVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// トーラス頂点を作成
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="thickness"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateTorusVertices(float diameter, float thickness, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);


	/// <summary>
	/// 矩形メッシュリソースを作成
	/// </summary>
	/// <param name="size">1辺の長さ</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateSquareMeshResource(float size, bool accessWrite = false);
	/// <summary>
	/// 立方体メッシュリソースを作成
	/// </summary>
	/// <param name="size">1辺の長さ</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCubeMeshResource(float size, bool accessWrite = false);
	/// <summary>
	/// 球メッシュリソースを作成
	/// </summary>
	/// <param name="diameter">直径</param>
	/// <param name="tessellation">分割数</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateSphereMeshResource(float diameter, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// カプセルメッシュリソースを作成
	/// </summary>
	/// <param name="diameter">直径</param>
	/// <param name="height">高さ</param>
	/// <param name="tessellation">分割数</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCapsuleMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// シリンダーメッシュリソースを作成
	/// </summary>
	/// <param name="diameter">直径</param>
	/// <param name="height">高さ</param>
	/// <param name="tessellation">分割数</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCylinderMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// コーンメッシュリソースを作成
	/// </summary>
	/// <param name="diameter">直径</param>
	/// <param name="height">高さ</param>
	/// <param name="tessellation">分割数</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateConeMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// トーラスメッシュリソースを作成
	/// </summary>
	/// <param name="diameter">直径</param>
	/// <param name="thickness">太さ</param>
	/// <param name="tessellation">分割数</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateTorusMeshResource(float diameter, float thickness, size_t tessellation, bool accessWrite = false);


	/// <summary>
	/// メッシュリソースを作成
	/// </summary>
	/// <param name="veritices">頂点配列</param>
	/// <param name="accessWrite">書き換え可能かどうか</param>
	/// <returns></returns>
	template<class T>
	static std::shared_ptr<MeshResource> CreateMeshResource(const std::vector<T>& vertices, bool accessWrite = false)
	{
		std::mutex mutex;
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		auto pMeshResource = ObjectFactory::Create<MeshResource>();
		// 汎用頂点を作成
		for (auto& v : vertices)
		{
			VertexPosition vertex;
			vertex.position = v.position;
			pMeshResource->m_meshPrimData.m_vertices.push_back(vertex);
		}
		// バッファ作成
		if (accessWrite)
		{
			DemandCreate(pMeshResource->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				auto pBackup = std::shared_ptr<BackupData<T>>(new BackupData<T>());
				for (auto& v : vertices)
					pBackup->m_vertices.push_back(v);
				pMeshResource->m_meshPrimData.m_backupData = pBackup;
				// 頂点バッファ作成
				CreateDynamicPrimitiveVertexBuffer(pDX11Device, vertices, ppResult);
			});
		}
		else
		{
			DemandCreate(pMeshResource->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				CreatePrimitiveBuffer(pDX11Device, vertices, D3D11_BIND_VERTEX_BUFFER, ppResult);
			});
		}
		// 超点数設定
		pMeshResource->m_meshPrimData.m_verticesNum = static_cast<UINT>(vertices.size());
		pMeshResource->m_meshPrimData.m_meshTypeIndex = typeid(T);
		pMeshResource->m_strideNum = sizeof(T);

		return pMeshResource;
	}

	/// <summary>
	/// メッシュリソースを作成
	/// </summary>
	/// <param name="vertices">頂点配列</param>
	/// <param name="indexes">インデックス配列</param>
	/// <param name="accessWrite">書き換え可能かどうか</param>
	/// <returns></returns>
	template<class T>
	static std::shared_ptr<MeshResource> CreateMeshResource(const std::vector<T>& vertices, const std::vector<uint16_t>& indexes, bool accessWrite = false)
	{
		std::mutex mutex;
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
		auto pMeshResouce = ObjectFactory::Create<MeshResource>();
		// 汎用頂点作成
		// インデックスをもとに作成
		for (auto i : indexes)
		{
			VertexPosition vertex;
			vertex.position = vertices[i].position;
			pMeshResouce->m_meshPrimData.m_vertices.push_back(vertex);
			// スキン情報を持つか
			if (typeid(T) == typeid(VertexPositionNormalTextureSkinning))
			{
				auto& ref = (VertexPositionNormalTextureSkinning&)vertices[i];
				SkinPrimData skinData;
				for (int i = 0; i < 4; ++i)
				{
					skinData.indexes[i] = ref.indexes[i];
					skinData.weights[i] = ref.weights[i];
				}
				pMeshResouce->m_meshPrimData.m_skins.push_back(skinData);
			}
			else if (typeid(T) == typeid(VertexPositionNormalTangentTextureSkinning))
			{
				auto& ref = (VertexPositionNormalTangentTextureSkinning&)vertices[i];
				SkinPrimData skinData;
				for (int i = 0; i < 4; ++i)
				{
					skinData.indexes[i] = ref.indexes[i];
					skinData.weights[i] = ref.weights[i];
				}
				pMeshResouce->m_meshPrimData.m_skins.push_back(skinData);
			}
		}
		// バッファ作成
		if (accessWrite)
		{
			DemandCreate(pMeshResouce->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				auto pBackup = std::shared_ptr<BackupData<T>>(new BackupData<T>());
				for (auto& v : vertices)
					pBackup->m_vertices.push_back(v);
				pMeshResouce->m_meshPrimData.m_backupData = pBackup;
				// 頂点バッファ作成
				CreateDynamicPrimitiveVertexBuffer(pDX11Device, vertices, ppResult);
			});
		}
		else
		{
			DemandCreate(pMeshResouce->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				// 頂点バッファ作成
				CreatePrimitiveBuffer(pDX11Device, vertices, D3D11_BIND_VERTEX_BUFFER, ppResult);
			});
		}
		// 頂点数設定
		pMeshResouce->m_meshPrimData.m_verticesNum = static_cast<UINT>(vertices.size());
		// インデックス作成
		DemandCreate(pMeshResouce->m_meshPrimData.m_indexBuff, mutex, [&](ID3D11Buffer** ppResult)
		{
			if (pMeshResouce->m_meshPrimData.m_backupData)
			{
				auto pBackup = std::dynamic_pointer_cast<BackupData<T>>(pMeshResouce->m_meshPrimData.m_backupData);
				for (auto& v : indexes)
					pBackup->m_indexes.push_back(v);
			}
			// インデックスバッファ作成
			CreatePrimitiveBuffer(pDX11Device, indexes, D3D11_BIND_INDEX_BUFFER, ppResult);
		});
		// インデックス数設定
		pMeshResouce->m_meshPrimData.m_indexesNum = static_cast<UINT>(indexes.size());
		pMeshResouce->m_meshPrimData.m_meshTypeIndex = typeid(T);
		pMeshResouce->m_meshPrimData.m_strideNum = sizeof(T);
		return pMeshResouce;
	}

	/// <summary>
	/// VertexPositionNormalTexture頂点をもとに
	/// それぞれの頂点情報に変換
	/// </summary>
	/// <param name="originVertices">元となる頂点</param>
	/// <param name="pcVertices">座標、色を持つ頂点</param>
	/// <param name="ptVertices">座標、UVを持つ頂点</param>
	/// <param name="pnttnVertices">座標、法線、UV、接ベクトルを持つ頂点</param>
	static void ConvertVertex(const std::vector<VertexPositionNormalTexture>& originVertices,
		std::vector<VertexPositionColor>& pcVertices,
		std::vector<VertexPositionTexture>& ptVertices,
		std::vector<VertexPositionNormalTextureTangent>& pnttgVertices);

protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	MeshResource();

	/// <summary>
	/// 頂点バッファを設定
	/// </summary>
	/// <param name="vertexBuffer"></param>
	void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer);
	/// <summary>
	/// 頂点数を設定
	/// </summary>
	/// <param name="verticesNum"></param>
	void SetVerticiesNum(UINT verticesNum);
	/// <summary>
	/// インデックスバッファを設定
	/// </summary>
	/// <param name="indexBuffer"></param>
	void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer);
	/// <summary>
	/// インデックス数を設定
	/// </summary>
	/// <param name="indexesNum"></param>
	void SetindexesNum(UINT indexesNum);

	/// <summary>
	/// 頂点の型とストライドを設定
	/// </summary>
	template<class T>
	void SetVertexType()
	{
		m_meshPrimData.m_meshTypeIndex = typeid(T);
		m_meshPrimData.m_strideNum = sizeof(T);
	}

private:
	MeshPrimData m_meshPrimData;
};



/// <summary>
/// マルチメッシュリソース
/// </summary>
class MultiMeshResource
	: public BaseResource
{
	friend class ObjectFactory;
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	MultiMeshResource();

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~MultiMeshResource();

	/// <summary>
	/// メッシュデータ数を取得
	/// </summary>
	size_t GetMeshVecNum()const;
	/// <summary>
	/// メッシュデータ配列を取得
	/// </summary>
	const std::vector<MeshPrimData>& GetMeshVec()const;

	/// <summary>
	/// メッシュデータを追加
	/// </summary>
	/// <param name="data">メッシュデータ</param>
	/// <returns>追加されたインデックス</returns>
	size_t AddMesh(const MeshPrimData& data);
	/// <summary>
	/// メッシュデータを追加
	/// </summary>
	/// <param name="meshRes">メッシュリソース</param>
	/// <returns>追加されたインデックス</returns>
	size_t AddMesh(const std::shared_ptr<MeshResource>& meshRes);
	/// <summary>
	/// メッシュデータを追加
	/// </summary>
	/// <param name="meshKey">メッシュキー</param>
	/// <returns>追加されたインデックス</returns>
	size_t AddMesh(const std::string& meshKey);
	/// <summary>
	/// メッシュリソースを追加
	/// </summary>
	/// <param name="vertices">頂点配列</param>
	/// <param name="AccessWrite">書き換え可能かどうか</param>
	/// <returns>追加されたインデックス</returns>
	template<class T>	// 頂点の型
	size_t AddMesh(const std::vector<T>& vertices, bool accessWrite = false)
	{
		auto meshRes = MeshResource::CreateMeshResource<T>(vertices, accessWrite);
		return AddMesh(meshRes);
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="vertices">頂点配列</param>
	/// <param name="indexes">インデックス配列</param>
	/// <param name="accessWrite">書き換え可能かどうか</param>
	/// <returns></returns>
	template<class T>	// 頂点の型
	size_t AddMesh(const std::vector<T>& vertices, const std::vector<uint16_t>& indexes, bool accessWrite = false)
	{
		auto pMeshRes = MeshResource::CreateMeshResource<T>(vertices, indexes, accessWrite);
		return AddMesh(pMeshRes);
	}
	/// <summary>
	/// メッシュデータを削除する
	/// </summary>
	/// <param name="index">削除するメッシュデータ配列のインデックス</param>
	void RemoveMesh(size_t index);

	/// <summary>
	/// メッシュデータを取得
	/// </summary>
	/// <param name="outData">取得するデータ参照</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void GetMeshData(MeshPrimData& outData, size_t index)const;
	/// <summary>
	/// 頂点バッファを取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer(size_t index)const;
	/// <summary>
	/// インデックスバッファを取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer(size_t index)const;
	/// <summary>
	/// 頂点数を取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	UINT GetVerticesNum(size_t index)const;
	/// <summary>
	/// インデックス数を取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	UINT GetIndexesNum(size_t index)const;
	/// <summary>
	/// ストライド数を取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	UINT GetStrideNum(size_t index)const;
	/// <summary>
	/// 描画トポロジーを取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(size_t index)const;
	/// <summary>
	/// 描画トポロジーを設定
	/// </summary>
	/// <param name="topology">描画トポロジー</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology, size_t index);
	/// <summary>
	/// type_indexを取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	/// <returns></returns>
	std::type_index GetTypeIndex(size_t index)const;
	/// <summary>
	/// ゲームオブジェクトとメッシュのずれを補完する行列を取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	const Float4x4& GetMeshOffsetTransform(size_t index)const;
	/// <summary>
	/// ゲームオブジェクトとメッシュのずれを補完する行列を設定
	/// </summary>
	/// <param name="offsetTransform">ずれ行列</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void SetMeshOffsetTransform(const Float4x4& offsetTransform, size_t index);
	/// <summary>
	/// ゲームオブジェクトとのずれを補完する行列を使用するかどうか
	/// </summary>
	/// <param name="index"></param>
	bool IsUseMeshOffsetTransform(size_t index)const;
	/// <summary>
	/// ゲームオブジェクトとのずれを補完する行列を使用するかどうか設定する
	/// </summary>
	/// <param name="b"></param>
	/// <param name="index"></param>
	void SetIsUseMeshOffsetTransform(bool b, size_t index);
	/// <summary>
	/// テクスチャリソースを設定(メッシュ毎に異なる場合使用
	/// </summary>
	/// <param name="textureRes">テクスチャリソース</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, size_t index);
	/// <summary>
	/// テクスチャリソースを設定(メッシュ毎に異なる場合使用
	/// </summary>
	/// <param name="textureKey">メッシュリソースキー</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void SetTextureResource(const std::string& textureKey, size_t index);
	/// <summary>
	/// テクスチャリソースを取得(メッシュ毎に異なる場合使用
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> GetTextureResource(size_t index)const;
	/// <summary>
	/// マテリアル配列を取得
	/// </summary>
	/// <param name="index">メッシュデータ配列を取得</param>
	const std::vector<Material>& GetMaterialVec(size_t index)const;

	/// <summary>
	/// スキニングするかどうか
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	virtual bool IsSkining(size_t index)const;
	/// <summary>
	/// ボーン数を取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	UINT GetBoneNum(size_t index)const;
	/// <summary>
	/// サンプリング数を取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	UINT GetSampleNum(size_t index)const;
	/// <summary>
	/// サンプリングされた行列の配列を取得
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	const std::vector<Float4x4>& GetSampleMatrixVec(size_t index)const;

	/// <summary>
	/// バックアップデータを取得
	/// </summary>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	template<class T>
	std::vector<T>& GetBackupVertices(size_t index)const
	{
		auto pBackup = std::dynamic_pointer_cast<BackupData<T>>(m_meshPrimDataVec[index].m_backupData);
		return pBackup->m_vertices;
	}

	/// <summary>
	/// 頂点を変更
	/// </summary>
	/// <param name="newBuffer">頂点配列</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	template<class T>
	void UpdateVertices(const std::vector<T>& newBuffer, size_t index)
	{
		auto pBakcup = std::dynamic_pointer_cast<BackupData<T>>(m_meshPrimDataVec[index].m_backupData);
		auto dev = APP->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD2DDeviceContext();
		// 頂点バッファをリソースから取り出す
		auto pVertexBuffer = GetVertexBuffer(index).Get();

		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;	// GPUに邪魔されない
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		// 頂点を変更
		T* vertices = (T*)mappedBuffer.pData;
		for (size_t i = 0; i < newBuffer.size(); ++i)
		{
			vertices[i] = newBuffer[i];
		}
		// アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



public:
	/// <summary>
	/// マルチメッシュリソースを作成
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<MultiMeshResource> CreateMultiMeshResource();
	/// <summary>
	/// オリジナルマルチメッシュを作成(スタティックメッシュ
	/// </summary>
	/// <param name="binDataDir">基準ディレクトリ</param>
	/// <param name="binDataFile">ファイル名</param>
	/// <param name="accessWrite">頂点を変更できるかどうか</param>
	/// <returns>作成したリソース</returns>
	static std::shared_ptr<MultiMeshResource> CreateStaticModelMultiMesh(const std::string& binDataDir, const std::string& binDataFile, bool accessWrite = false);
	/// <summary>
	/// オリジナルマルチメッシュを作成(ボーンメッシュ
	/// </summary>
	/// <param name="binDataDir"></param>
	/// <param name="binDataFile"></param>
	/// <param name="accessWrite"></param>
	/// <returns></returns>
	static std::shared_ptr<MultiMeshResource> CreateBoneModelMultiMesh(const std::string& binDataDir, const std::string& binDataFile, bool accessWrite = false);




protected:
	/// <summary>
	/// 頂点バッファを設定
	/// </summary>
	/// <param name="vertexBuffer">頂点バッファ</param>
	/// <param name="index">メッシュデータ配列のインデックス</param>
	void SetVertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, size_t index);
	/// <summary>
	/// 頂点数を設定
	/// </summary>
	/// <param name="verticesNum">頂点数</param>
	/// <param name="index">メッシュデータ配列インデックス</param>
	void SetVerticesNum(UINT verticesNum, size_t index);

	/// <summary>
	/// インデックスバッファを設定
	/// </summary>
	/// <param name="indexBuffer">インデックスバッファ</param>
	/// <param name="index">メッシュデータ配列インデックス</param>
	void SetIndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, size_t index);
	/// <summary>
	/// インデックス数を設定
	/// </summary>
	/// <param name="indexesNum">インデックス数</param>
	/// <param name="index">メッシュデータ配列インデックス</param>
	void SetIndexesNum(UINT indexesNum, size_t index);

	/// <summary>
	/// 頂点の型とストライドを設定
	/// </summary>
	/// <param name="index">メッシュデータ配列インデックス</param>
	template<class T>
	void SetVertexType(size_t index)
	{
		m_meshPrimDataVec[index].m_meshTypeIndex = typeid(T);
		m_meshPrimDataVec[index].m_strideNum = sizeof(T);
	}

private:
	std::vector<MeshPrimData> m_meshPrimDataVec;	// メッシュデータ配列
};



/// <summary>
/// スプライトベース
/// </summary>
class SpriteBase
	: public DrawComponentBase
{
public:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	explicit SpriteBase(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~SpriteBase();

public:
	void OnCreate()override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw()override;

	/// <summary>
	/// 頂点変更できるスプライトメッシュを作成
	/// すでに作成済みの場合は差し替える
	/// </summary>
	/// <param name="vertices">頂点配列</param>
	/// <param name="indexes">頂点インデックス配列</param>
	template<class T>
	void CreateMesh(std::vector<T>& vertices, std::vector<uint16_t>& indexes)
	{
		try
		{
			// メッシュ作成(頂点変更可)
			auto mesh = MeshResource::CreateMeshResouce(vertices, indexes, true);
			SetMeshResource(mesh);
		}
		catch (...)
		{
			throw;
		}
	}
	/// <summary>
	/// 頂点バッファを更新
	/// </summary>
	/// <param name="verticies">頂点配列</param>
	template<class T>
	void UpdateVertices(const std::vector<T>& verticiesVec)
	{
		auto mesh = GetMeshResource();
		if (!mesh)
		{
			throw StandardException(
				"メッシュが作成されていない",
				"if(!GetMeshResource())",
				"SpriteBase::UpdateVertices()"
			);
		}
		if (verticiesVec.size() > mesh->GetVerticesNum())
		{
			throw StandardException(
				"更新する頂点数が大きい",
				"if(Vertices.size() > mesh->GetVerticesNum()",
				"SpriteBase::UpdateVertices()"
			);
		}

		auto dev = APP->GetDeviceResources();
		auto pD3D11DeviceContext = dev->GetD3DDeviceContext();

		// 頂点変更
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;	// GPUに邪魔されないようにする
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		// 頂点マップ
		if (FAILED(pD3D11DeviceContext->Map(mesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer)))
		{
			throw StandardException(
				"頂点のMapに失敗",
				"if(FAILED(pD3D11DeviceContext->Map()))",
				"SpriteBase::UpdateVertices()"
			);
		}
		// 頂点変更
		T* vertices = (T*)mappedBuffer.pData;
		for (size_t i = 0; i < mesh->GetVerticesNum(); ++i)
		{
			vertices[i] = verticiesVec[i];
		}
		// アンマップ
		pD3D11DeviceContext->Unmap(mesh->GetVertexBuffer().Get(), 0);
	}

	/// <summary>
	/// メッシュリソースを取得
	/// </summary>
	/// <returns></returns>
	std::shared_ptr <MeshResource> GetMeshResource()const;
	/// <summary>
	/// メッシュリソースを設定
	/// </summary>
	/// <param name="meshResource"></param>
	void SetMeshResource(const std::shared_ptr<MeshResource>& meshResource);
	/// <summary>
	/// テクスチャリソースを設定
	/// </summary>
	/// <param name="textureResource"></param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource);
	/// <summary>
	/// テクスチャリソースを設定
	/// </summary>
	/// <param name="textureKey">登録されているキー</param>
	void SetTextureResource(const std::string& textureKey);
	/// <summary>
	/// テクスチャリソースを取得
	/// </summary>
	std::shared_ptr<TextureResouce> GetTextureResource()const;

	/// <summary>
	/// エミッシブを取得
	/// </summary>
	Float4 GetEmissive()const;
	/// <summary>
	/// エミッシブを設定
	/// </summary>
	void SetEmissive(const Float4& emissive);
	/// <summary>
	/// ディフューズを取得
	/// </summary>
	Float4 GetDiffese()const;
	/// <summary>
	/// ディフューズを設定
	/// </summary>
	void SetDiffuse(const Float4& diffuse);

	/// <summary>
	/// シェーダーを設定
	/// </summary>
	template<class T_VShader, class T_PShader>
	void SetShader()
	{
		SetVertexShader(T_VShader::GetInstance()->GetShader(),
			T_VShader::GetInstance()->GetInputLayout());
		SetPixelShader(T_PShader::GetInstance()->GetShader());
	}

	/// <summary>
	/// 頂点シェーダを設定
	/// </summary>
	void SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pIL);
	/// <summary>
	/// ピクセルシェーダを設定
	/// </summary>
	void SetPixelShader(ID3D11PixelShader* pPS);

protected:
	/// <summary>
	/// 定数バッファを設定
	/// </summary>
	/// <param name="spriteConstant"></param>
	void WriteConstantBuffer(SpriteConstants& spriteConstant);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};




/// <summary>
/// SimpleConstantBufferを使う描画コンポーネント
/// </summary>
class SimpleCBDrawComponent
	: public DrawComponentBase
{
public:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	explicit SimpleCBDrawComponent(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~SimpleCBDrawComponent();

public:
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw()override;


protected:
	/// <summary>
	/// 定数バッファを更新
	/// </summary>
	/// <param name="SCB"></param>
	/// <param name="data"></param>
	void WriteConstants(SimpleConstants& SCB, const MeshPrimData& data);


private:
};

/// <summary>
/// BasicConstantBufferを使う描画コンポーネント
/// </summary>
class BasicCBDrawComponent
	:public DrawComponentBase
{
public:
	explicit BasicCBDrawComponent(const std::shared_ptr<GameObject>& pGameObject);
	virtual ~BasicCBDrawComponent();

	void Draw()override;

private:
	/// <summary>
	/// 定数バッファを更新
	/// </summary>
	/// <param name="BCB"></param>
	void WriteConstants(BasicConstants& BCB, const MeshPrimData& data);
};

///// <summary>
///// 座標・法線・UVを持つ3DDrawComponent
///// </summary>
//class PNTStaticDraw
//	: public DrawSolidBase
//{
//public:
//	PNTStaticDraw();
//	virtual ~PNTStaticDraw();
//
//	/// <summary>
//	/// 初期化処理
//	/// </summary>
//	virtual void OnCreate()override;
//	/// <summary>
//	/// 描画処理
//	/// </summary>
//	virtual void Draw()override;
//};


#if 0
/// <summary>
/// 描画に使用する構造体(ベース)
/// </summary>
struct DrawObjectBase
{
	std::weak_ptr<MeshResource> m_meshResource;	// メッシュリソース
	std::weak_ptr<MultiMeshResource> m_multiMeshResource; // マルチメッシュリソース

	//std::vector<Float4x4> m_localBoneMatrix;	// ローカルボーン行列
	//std::vector<std::vector<Float4x4>> m_multiLocalBonesMatrix;	// ローカルボーン行列(マルチメッシュ用
	//
	//std::map<std::string, animationdata> m_animationMap;	// アニメーションマップ
	//std::string m_currentAnimationName;	// 現在のアニメーション名
	//float m_currentAnimationTime;	// 現在のアニメーションタイム

	DrawObjectBase()
		//:m_currentAnimationName(""), m_currentAnimationTime(0.0f)
	{}


};
#endif // 0
