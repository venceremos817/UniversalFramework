#pragma once
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <wrl/client.h>
#include <mutex>
#include <wincodec.h>
#include <memory>
#include <string>
#include <XMFLOAT_EX.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment( lib, "Windowscodecs.lib" )

class DefaultRenderTarget;
class RenderState;

/// <summary>
/// DirectX11デバイスクラス
/// </summary>
class DeviceResources
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="hWnd">ウィンドウハンドル</param>
	/// <param name="fullScreen">フルスクリーンかどうか</param>
	/// <param name="screenWidth">スクリーン横幅</param>
	/// <param name="screenHeight">スクリーン縦幅</param>
	DeviceResources(HWND hWnd, bool fullScreen, UINT screenWidth, UINT screenHeight);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DeviceResources();

	/// <summary>
	/// D3D11Device2インターフェイスを取得
	/// </summary>
	ID3D11Device2* GetD3DDevice()const;
	/// <summary>
	/// D3D11DeviceContext2インターフェイスを取得
	/// </summary>
	ID3D11DeviceContext2* GetD3DDeviceContext()const;
	/// <summary>
	/// DXGISwapChain1インターフェイスを取得
	/// </summary>
	IDXGISwapChain1* GetSwapChain()const;
	/// <summary>
	/// D3D_FEATURE_LEVELを取得
	/// </summary>
	D3D_FEATURE_LEVEL GetFeatureLevel()const;

	/// <summary>
	/// D2D1Factory2インターフェイスを取得
	/// </summary>
	ID2D1Factory2* GetD2DFactory()const;
	/// <summary>
	/// D2DDevice1インターフェイスを取得
	/// </summary>
	ID2D1Device1* GetD2DDevice()const;
	/// <summary>
	/// D2D1DeviceContex1インターフェイスを取得
	/// </summary>
	ID2D1DeviceContext1* GetD2DDeviceContext()const;
	/// <summary>
	/// DWriteFactory2インターフェイスを取得
	/// </summary>
	IDWriteFactory2* GetDWriteFactory()const;
	/// <summary>
	/// WICImagingFactory2インターフェイスを取得
	/// </summary>
	IWICImagingFactory2* GetWicImagingFactory()const;

	/// <summary>
	/// デフォルトレンダリングターゲットを取得
	/// </summary>
	std::shared_ptr<DefaultRenderTarget> GetDefaultRenderTarget();
	/// <summary>
	/// レンダリングステートを取得
	/// </summary>
	std::shared_ptr<RenderState> GetRenderState()const;
	/// <summary>
	/// 通常描画をクリア
	/// </summary>
	/// <param name="color">クリア色</param>
	virtual void ClearDefaultView(const Float4& color = Float4(0.2f, 0.2f, 0.2f, 1.0f));
	/// <summary>
	/// 通常描画開始
	/// </summary>
	virtual void StartDefaultDraw();
	/// <summary>
	/// 通常描画終了
	/// </summary>
	virtual void EndDefaultDraw();
	/// <summary>
	/// 描画後処理
	/// </summary>
	void InitailizeStates();

	/// <summary>
	/// フロントバッファに転送
	/// </summary>
	/// <param name="syncInterval">垂直同期タイミング(0なら即時対応/非0なら数値の時間経過後に対応)</param>
	/// <param name="flags">出力オプション(基本0(DXGI_PRESENT)</param>
	void Present(unsigned int syncInterval, unsigned int flags);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};

/// <summary>
/// ブレンドステート
/// </summary>
enum class BlendState
{
	Opaque,
	AlphaBlend,
	Additive,
	NonPremultiplied,
};
/// <summary>
/// デプスステンシルステート
/// </summary>
enum class DepthStencilState
{
	None,
	Default,
	Read,
};
/// <summary>
///  ラスタライザステート
/// </summary>
enum class RasterizerState
{
	CullNone,		// 両面描画
	CullFront,		// 裏面描画
	CullBack,		// 前面描画
	Wireframe,		// ワイヤフレーム
	DoubleDraw,		// 背面描画の後、前面描画
};
/// <summary>
/// サンプラーステート
/// </summary>
enum class SamplerState
{
	SamplerNone,
	PointWrap,
	PointClamp,
	LinearWrap,
	LinearClamp,
	AnisotropicWrap,
	AnisotropicClamp,
	ComparisonLinear,
};

/// <summary>
/// レンダリングステート
/// </summary>
class RenderState
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	RenderState();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~RenderState();

	/// <summary>
	/// 塗りつぶしのブレンドステートを取得
	/// </summary>
	ID3D11BlendState* GetOpaque()const;
	/// <summary>
	/// 単純な透明処理を取得
	/// </summary>
	/// <returns></returns>
	ID3D11BlendState* GetAlphaBlend()const;
	/// <summary>
	/// 半透明処理を取得
	/// </summary>
	ID3D11BlendState* GetAlphaBlendEx()const;
	/// <summary>
	/// 加算処理を取得
	/// </summary>
	ID3D11BlendState* GetAdditive()const;
	/// <summary>
	/// アルファを使用した元のデータと対象データをブレンド処理を取得
	/// </summary>
	ID3D11BlendState* GetNonPremultiplied()const;

	/// <summary>
	/// 深度処理無しのデプスステンシルステートを取得
	/// </summary>
	ID3D11DepthStencilState* GetDepthNone()const;
	/// <summary>
	/// 通常深度のデプスステンシルステートを取得
	/// </summary>
	ID3D11DepthStencilState* GetDepthDefault()const;
	/// <summary>
	/// 読み取り専用のデプスステンシルステートを取得
	/// </summary>
	ID3D11DepthStencilState* GetDepthRead()const;

	/// <summary>
	/// カリング無しのラスタライザステートを取得
	/// </summary>
	ID3D11RasterizerState* GetCullNone()const;
	/// <summary>
	/// カリング無しのラスタライザステート(シーサー処理)を取得
	/// </summary>
	ID3D11RasterizerState* GetCullNoneScissor()const;
	/// <summary>
	/// フロントカリング(背面描画のみ)のラスタライザステートを取得
	/// </summary>
	ID3D11RasterizerState* GetCullFront()const;
	/// <summary>
	/// フロントカリング(背面描画のみ)のラスタライザステート(シーサー処理)を取得
	/// </summary>
	ID3D11RasterizerState* GetCullFrontScissor()const;
	/// <summary>
	/// バックカリング(前面描画のみ)のラスタライザステートを取得
	/// </summary>
	ID3D11RasterizerState* GetCullBack()const;
	/// <summary>
	/// バックカリング(前面描画のみ)のラスタライザステート(シーサー処理)を取得
	/// </summary>
	ID3D11RasterizerState* GetCullBackScissor()const;
	/// <summary>
	/// ワイヤフレームのラスタライザステートを取得
	/// </summary>
	ID3D11RasterizerState* GetWireframe()const;
	/// <summary>
	/// ワイヤフレームのラスタライザステート(シーサー処理)を取得
	/// </summary>
	/// <returns></returns>
	ID3D11RasterizerState* GetWireframeScissor()const;

	/// <summary>
	/// 点フィルタリングとテクスチャ座標ラップのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetPointWrap()const;
	/// <summary>
	/// 点フィルタリングとテクスチャ座標クランプのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetPointClamp()const;
	/// <summary>
	/// 線形フィルタリングとテクスチャ座標ラップのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetLinearWrap()const;
	/// <summary>
	/// 線形フィルタリングとテクスチャ座標クランプのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetLinearClamp()const;
	/// <summary>
	/// 異方性フィルタリングとテクスチャ座標ラップのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetAnisotropicWrap()const;
	/// <summary>
	/// 異方性フィルタリングとテクスチャ座標クランプのサンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetAnisotropicClamp()const;
	/// <summary>
	/// 線形フィルタリングを使った比較サンプラーステートを取得
	/// </summary>
	ID3D11SamplerState* GetComparisonLinear()const;

	/// <summary>
	/// ブレンドステートの設定
	/// </summary>
	void SetBlendState(ID3D11DeviceContext2* pContext, BlendState state);
	/// <summary>
	/// デプスステンシルの設定
	/// </summary>
	void SetDepthStencilState(ID3D11DeviceContext2* pContext, DepthStencilState state);
	/// <summary>
	/// ラスタライザステートの設定
	/// </summary>
	/// <param name="pContext"></param>
	/// <param name="state"></param>
	void SetRasterizerState(ID3D11DeviceContext2* pContext, RasterizerState state);
	/// <summary>
	/// 1つのサンプラーステートの設定
	/// </summary>
	/// <param name="pContext"></param>
	/// <param name="state"></param>
	/// <param name="slot">スロット番号</param>
	void SetSamplerState(ID3D11DeviceContext2* pContext, SamplerState state, UINT slot);
	/// <summary>
	/// サンプラーステートをすべてクリアする
	/// </summary>
	void SetSamplerAllClear(ID3D11DeviceContext2* pContext);

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

	// コピー禁止
	RenderState(const RenderState&) = delete;
	RenderState& operator=(const RenderState&) = delete;
	// ムーブ禁止
	RenderState(const RenderState&&) = delete;
	RenderState& operator=(const RenderState&&) = delete;
};

/// <summary>
/// レンダーターゲット基底クラス
/// </summary>
class RenderTarget
{
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	RenderTarget();
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~RenderTarget();

public:
	/// <summary>
	/// レンダリングターゲットをクリア
	/// </summary>
	/// <param name="color">クリア色</param>
	virtual void ClearViews(const Float4& color) = 0;
	/// <summary>
	/// レンダリングターゲットを開始
	/// </summary>
	virtual void StartRenderTarget() = 0;
	/// <summary>
	/// レンダリングターゲットを終了
	/// </summary>
	virtual void EndRenderTarget() = 0;

	/// <summary>
	/// ビューポートを取得
	/// </summary>
	const D3D11_VIEWPORT& GetViewPort()const;
	/// <summary>
	/// ビューポートを設定
	/// </summary>
	/// <param name="viewPort"></param>
	void SetViewPort(const D3D11_VIEWPORT& viewPort);

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

	// コピー禁止
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;
	// ムーブ禁止
	RenderTarget(const RenderTarget&&) = delete;
	RenderTarget& operator=(const RenderTarget&&) = delete;
};


/// <summary>
/// デフォルトレンダリングターゲット
/// </summary>
class DefaultRenderTarget
	: public RenderTarget
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	explicit DefaultRenderTarget();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~DefaultRenderTarget();

	/// <summary>
	/// レンダリングターゲットをクリア
	/// </summary>
	/// <param name="color">クリア色</param>
	virtual void ClearViews(const Float4& color)override;
	/// <summary>
	/// レンダリングターゲットを開始
	/// </summary>
	virtual void StartRenderTarget()override;
	/// <summary>
	/// レンダリングターゲットを終了
	/// </summary>
	virtual void EndRenderTarget()override;

	/// <summary>
	/// レンダリングターゲットビューを取得
	/// </summary>
	ID3D11RenderTargetView* GetRenderTargetView()const;
	/// <summary>
	/// デプスステンシルテクスチャを取得
	/// </summary>
	ID3D11Texture2D* GetDepthStencil()const;
	/// <summary>
	/// デプスステンシルビューを取得
	/// </summary>
	ID3D11DepthStencilView* GetDepthStencilView()const;
	/// <summary>
	/// 2次元ターゲットのビットマップを取得
	/// </summary>
	ID2D1Bitmap1* GetD2DTargetBitmap()const;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

};



/// <summary>
/// 安全にCOMインターフェイスのポインタを取得する
/// </summary>
/// <param name="pCom">COMポインタ</param>
/// <param name="mutex">ミューテックス</param>
/// <param name="createFunc">作成関数</param>
/// <returns>COMが無効の場合は新しく作成して、それ以外はCOMから得たCOMインターフェイスのポインタ</returns>
template<class T, class TCreateFunc>
static inline T* DemandCreate(Microsoft::WRL::ComPtr<T>& pCom, std::mutex& mutex, TCreateFunc createFunc)
{
	T* result = pCom.Get();
	// ロック状態をチェック
	MemoryBarrier();
	if (!result)
	{
		std::lock_guard<std::mutex> lock(mutex);
		result = pCom.Get();
		if (!result)
		{
			createFunc(&result);
			MemoryBarrier();
			pCom.Attach(result);
		}
	}
	return result;
}


/// <summary>
/// シェーダー関連リソースのインターフェイス
/// </summary>
class ShaderResource
{
protected:
	/// <summary>
	/// プロテクトコンストラクタ
	/// </summary>
	ShaderResource();
	/// <summary>
	/// プロテクトデストラクタ
	/// </summary>
	virtual ~ShaderResource();

public:
	/// <summary>
	/// バイナリファイルを読み込む
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="data">バイナリデータの戻り</param>
	/// <param name="csoSize">サイズの戻り</param>
	static void ReadBinaryFile(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, size_t& csoSize);

	/// <summary>
	/// バイナリデータから頂点シェーダを作成する
	/// </summary>
	/// <param name="data">バイナリデータ</param>
	/// <param name="csoSize">サイズ</param>
	/// <param name="ppResult">受け取るシェーダ</param>
	static void CreateVertexShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11VertexShader** ppResult);
	/// <summary>
	/// バイナリデータからインプットレイアウトを作成する
	/// </summary>
	/// <param name="data">バイナリデータ</param>
	/// <param name="csoSize">サイズ</param>
	/// <param name="pElement">頂点定義</param>
	/// <param name="elementsNum">頂点定義数</param>
	/// <param name="ppResult">受け取るレイアウト</param>
	static void CreateInputLayout(std::unique_ptr<uint8_t[]>& data, size_t csoSize, const D3D11_INPUT_ELEMENT_DESC* pElement, UINT elementsNum, ID3D11InputLayout** ppResult);
	/// <summary>
	/// バイナリデータからピクセルシェーダを作成する
	/// </summary>
	/// <param name="data">バイナリデータ</param>
	/// <param name="csoSize">サイズ</param>
	/// <param name="ppResult">受け取るシェーダ</param>
	static void CreatePixelShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11PixelShader** ppResult);

	/// <summary>
	/// 定数バッファを作成する
	/// </summary>
	/// <param name="buffSize">サイズ</param>
	/// <param name="ppResult">受け取る定数バッファ</param>
	static void CreateConstantBuffer(UINT buffSize, ID3D11Buffer** ppResult);

protected:
	/// <summary>
	/// 頂点シェーダアクセサ
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="pVS">検証するシェーダCOM</param>
	ID3D11VertexShader* GetVertexShaderBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& pVS);
	/// <summary>
	/// ピクセルシェーダアクセサ
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="pPS">検証するシェーダCOM</param>
	/// <returns></returns>
	ID3D11PixelShader* GetPixelShaderBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pPS);
	/// <summary>
	/// インプットレイアウトアクセサ
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <param name="pIL">検証するインプットレイアウトCOM</param>
	/// <param name="pElem">頂点定義</param>
	/// <param name="elementsNum">頂点定義数</param>
	/// <returns></returns>
	ID3D11InputLayout* GetInputLayoutBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11InputLayout>& pIL, const D3D11_INPUT_ELEMENT_DESC* pElem, UINT elementsNum);
	/// <summary>
	/// 定数バッファアクセサ
	/// </summary>
	/// <param name="buffSize">サイズ</param>
	/// <param name="buffer">検証するバッファCOM</param>
	/// <returns></returns>
	ID3D11Buffer* GetConstantBufferBase(UINT buffSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer);

public:
	static const int m_lightMax{ 3 };	// ライト最大数

protected:
	std::mutex mutexBase;	// ミューテックス

private:
	//struct Param;
	//std::unique_ptr<Param> m_param;

	// コピー禁止
	ShaderResource(const ShaderResource&) = delete;
	ShaderResource& operator=(const ShaderResource&) = delete;
	// ムーブ禁止
	ShaderResource(const ShaderResource&&) = delete;
	ShaderResource&& operator=(const ShaderResource&&) = delete;
};

/// <summary>
/// 頂点シェーダー(シングルトン)
/// </summary>
template<class ShaderType, class VertexType>
class VertexShader
	: public ShaderResource
{
protected:
	struct Deleter
	{
		void operator()(ShaderType* p) { delete p; }
	};

	VertexShader<ShaderType, VertexType>(const std::string& fileName) : ShaderResource(), m_fileName(fileName) {}
	virtual ~VertexShader() {}

public:
	/// <summary>
	/// 頂点シェーダを取得
	/// </summary>
	ID3D11VertexShader* GetShader() { return GetVertexShaderBase(m_fileName, m_pVS); }
	/// <summary>
	/// インプットレイアウトを取得
	/// </summary>
	ID3D11InputLayout* GetInputLayout() { return GetInputLayoutBase(m_fileName, m_pIL, VertexType::GetVertexElement(), VertexType::GetElementsNum()); }

	/// <summary>
	/// インスタンスを取得
	/// </summary>
	static std::unique_ptr<ShaderType, Deleter>& GetInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new ShaderType());
		return m_pInstance;
	}

protected:
	static std::unique_ptr<ShaderType, Deleter> m_pInstance;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVS;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pIL;
	std::mutex mutex;

private:
	std::string m_fileName;
};

template<class ShaderType>
class PixelShader
	:public ShaderResource
{
protected:
	// デリーター
	struct Deleter
	{
		void operator()(ShaderType* p) { delete p; }
	};

	PixelShader<ShaderType>(const std::string& fileName) : ShaderResource(), m_fileName(fileName) {}
	virtual ~PixelShader() {}

public:
	/// <summary>
	/// ピクセルシェーダーを取得
	/// </summary>
	/// <returns></returns>
	ID3D11PixelShader* GetShader() { return GetPixelShaderBase(m_fileName, m_pPS); }

	static std::unique_ptr<ShaderType, Deleter>& GetInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new ShaderType());
		return m_pInstance;
	}

protected:
	static std::unique_ptr<ShaderType, Deleter> m_pInstance;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPS;
	std::mutex mutex;

private:
	std::string m_fileName;
};

/// <summary>
/// 定数バッファ(シングルトン)
/// </summary>
template<class ShaderType, class ConstantType>
class ConstantBuffer : public ShaderResource
{
protected:
	// デリータ
	struct Deleter
	{
		void operator()(ShaderType* p) { delete p; }
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::mutex m_mutex;		// ミューテックス
	// 構築と破棄
	ConstantBuffer<ShaderType, ConstantType>() : ShaderResource() {}
	virtual ~ConstantBuffer() {}
	static std::unique_ptr<ShaderType, Deleter> m_pInstance;


public:
	/// <summary>
	/// 定数バッファを取得
	/// </summary>
	/// <returns></returns>
	ID3D11Buffer* GetBuffer()
	{
		return GetConstantBufferBase(sizeof(ConstantType), m_buffer);
	}

	static std::unique_ptr<ShaderType, Deleter>& GetInstance()
	{
		if (!m_pInstance)
			m_pInstance.reset(new ShaderType());
		return m_pInstance;
	}

private:
	// コピー禁止
	ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&) = delete;
	ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&) = delete;
	// ムーブ禁止
	ConstantBuffer(const ConstantBuffer&&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&&) = delete;
};



