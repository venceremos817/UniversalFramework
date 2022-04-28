#include "DeviceResource.h"
#include <App.h>
#include <BinaryReader.h>
#include <ThrowException.h>


//===========================================
// DirectX11デバイスクラス
//===========================================
#pragma region DeviceResources
struct DeviceResources::Param
{
	D3D_DRIVER_TYPE		D3DDriverType;	// ドライバの種類
	D3D_FEATURE_LEVEL	D3DFeatureLevel;	// 機能レベル
	// Direct3D
	Microsoft::WRL::ComPtr<ID3D11Device2>			pD3D11Device;		// ID3D11Deviceインターフェイス
	Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	pD3D11Context;		// デバイスコンテキスト
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			pD3D11SwapChain;	// IDXGIDrapChainインターフェイス
	// Direct2D
	Microsoft::WRL::ComPtr<ID2D1Factory2>			pD2DFactory;	// ID2D1Factoryインターフェイス
	Microsoft::WRL::ComPtr<ID2D1Device1>			pD2DDevice;		// ID2D1Deviceインターフェイス
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1>		pD2DContext;	// ID2D1デバイスコンテキスト
	// DirectWrite
	Microsoft::WRL::ComPtr<IDWriteFactory2>		pDwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	pWicFactory;

	float dpi;	//DPI

	std::shared_ptr<DefaultRenderTarget>	pDefaultRenderTarget;	// デフォルトレンダリングターゲット
	std::shared_ptr<RenderState>			pRenderState;			// レンダリングステート

	/// <summary>
	/// サンプラーステートを作成
	/// </summary>
	/// <param name="filter"></param>
	/// <param name="addressMode"></param>
	/// <param name="ppResult"></param>
	void CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** ppResult)
	{
		try
		{
			auto dev = APP->GetDeviceResources();
			auto pDX11Device = dev->GetD3DDevice();

			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Filter = filter;

			desc.AddressU = addressMode;
			desc.AddressV = addressMode;
			desc.AddressW = addressMode;

			desc.MaxAnisotropy = (pD3D11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

			desc.MaxLOD = FLT_MAX;
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

			HRESULT hr = pD3D11Device->CreateSamplerState(&desc, ppResult);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw StandardException(
					"サンプラーステート作成失敗",
					"if(FAILED(pDX11Device->CreteSamplerState()))",
					"DeviceResources::Param::CreateSamplerState()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	/// <summary>
	/// ブレンドステートを作成
	/// </summary>
	/// <param name="srcBlend"></param>
	/// <param name="destBlend"></param>
	/// <param name="ppResult"></param>
	void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** ppResult)
	{
		try
		{
			auto dev = APP->GetDeviceResources();
			auto pDX11Device = dev->GetD3DDevice();

			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);

			desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
			desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
			desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			HRESULT hr = pD3D11Device->CreateBlendState(&desc, ppResult);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw StandardException(
					"ブレンドステート作成に失敗",
					"if(FAILED(pDX11Device->CreateBlendState()))",
					"DeviceResources::Param::CreateBlendState()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	/// <summary>
	/// デプスステンシルステートを作成
	/// </summary>
	/// <param name="enable"></param>
	/// <param name="writeEnable"></param>
	/// <param name="ppResult"></param>
	void CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState** ppResult)
	{
		try
		{
			auto dev = APP->GetDeviceResources();
			auto pDX11Device = dev->GetD3DDevice();

			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.DepthEnable = enable;
			desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			desc.BackFace = desc.FrontFace;

			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, ppResult);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw StandardException(
					"デプスステンシルステート作成失敗",
					"if(FAILED(pDX11Device->CreateDepthStencilState()))",
					"DeviceResources::Param::CreateDepthStencilState()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	/// <summary>
	/// ラスタライザステートを作成
	/// </summary>
	/// <param name="cullMode"></param>
	/// <param name="fillMode"></param>
	/// <param name="ppResult"></param>
	/// <param name="scissor"></param>
	void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** ppResult, bool scissor = false)
	{
		try
		{
			auto dev = APP->GetDeviceResources();
			auto pDX11Device = dev->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.CullMode = cullMode;
			desc.FillMode = fillMode;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = true;
			desc.ScissorEnable = scissor;

			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, ppResult);
			if (FAILED(hr))
			{
				// 初期化失敗
				throw StandardException(
					"ラスタライザステート作成失敗",
					"if(FAILED(pDX11Device->CreateRasterizerState()))",
					"DeviceResources::Param::CreateRasterizerState()"
				);
			}
		}
		catch (...)
		{
			throw;
		}
	}

	Microsoft::WRL::ComPtr<ID3D11BlendState>	pOpaque;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>	pLinearClamp;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	pDepthDefault;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pCullBack;
	std::mutex mutex;	// ミューテックス

	Param(HWND hWnd, bool fullScreen, UINT width, UINT height);
	~Param() {}

	/// <summary>
	/// リソースの構築
	/// </summary>
	/// <param name="hWnd"></param>
	/// <param name="fullScreen"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void CreateDeviceResouces(HWND hWnd, bool fullScreen, UINT width, UINT height);

	ID3D11SamplerState* GetLinearClampSampler()
	{
		return DemandCreate(pLinearClamp, mutex,
			[&](ID3D11SamplerState** ppResult)
		{
			CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, ppResult);
		});
	}

	ID3D11BlendState* GetOpaqueBlend()
	{
		return DemandCreate(pOpaque, mutex,
			[&](ID3D11BlendState** ppResult)
		{
			CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, ppResult);
		});
	}

	ID3D11RasterizerState* GetCullBackRasterizer()
	{
		return DemandCreate(pCullBack, mutex,
			[&](ID3D11RasterizerState** ppResult)
		{
			CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, ppResult);
		});
	}

	ID3D11DepthStencilState* GetDefaultDepthStencil()
	{
		return DemandCreate(pDepthDefault, mutex,
			[&](ID3D11DepthStencilState** ppResult)
		{
			CreateDepthStencilState(true, true, ppResult);
		});
	}
};

DeviceResources::Param::Param(HWND hWnd, bool fullScreen, UINT width, UINT height)
	: dpi(96.0f)
{
	try
	{
		// リソースの構築
		CreateDeviceResouces(hWnd, fullScreen, width, height);
	}
	catch (...)
	{
		throw;
	}
}

void DeviceResources::Param::CreateDeviceResouces(HWND hWnd, bool fullScreen, UINT width, UINT height)
{
	// Direct2Dリソースを初期化
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#ifdef _DEBUG
	// プロジェクトがデバッグビルドに含まれている場合は、Direct2DデバッグをSDKレイヤーを介して有効にする
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif // _DEBUG
	// Direct2Dファクトリを初期化
	ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory2),
			&options,
			&pD2DFactory
		),
		"Factory作成に失敗",
		"D2D1CreateFactory()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// DirectWriteファクトリを初期化
	ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory2),
			&pDwriteFactory
		),
		"DirectWriteファクトリ作成に失敗",
		"DwriteCreateFactory()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	ThrowIfFailed(
		CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pWicFactory)
		),
		"(WIC)ファクトリ作成に失敗",
		"CoCreateInstance()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	HRESULT hr;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// デバイス作成用フラグ
	// ドライバの種類
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,		// GPUで描画
		D3D_DRIVER_TYPE_WARP,			// 高精度(低速
		D3D_DRIVER_TYPE_REFERENCE,		// CPUで描画
	};
	// 選択できるドライバの数
	UINT driverTypesNum = ARRAYSIZE(driverTypes);

	// 機能セット
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1対応GPUレベル
		D3D_FEATURE_LEVEL_11_0,		// DirectX11対応GPUレベル
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1対応GPUレベル
		D3D_FEATURE_LEVEL_10_0,		// DirectX10対応GPUレベル
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3対応GPUレベル
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2対応GPUレベル
		D3D_FEATURE_LEVEL_9_1		// Direct9.1対応GPUレベル
	};
	// 選択できる機能レベルの数
	UINT featureLevelsNum = ARRAYSIZE(featureLevels);

	// スワップチェーンのデスクプリタの設定
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;		// バッファ数
	sd.BufferDesc.Width = width;	// バッファの幅
	sd.BufferDesc.Height = height;	// バッファの高さ
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// バックバッファフォーマット(R,G,B,A)16ビット符号なし正規化整数フォーマット
	sd.BufferDesc.RefreshRate.Numerator = 1000;	// リフレッシュレート最大値
	sd.BufferDesc.RefreshRate.Denominator = 1;	// リフレッシュレート最小値
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// バックバッファの使用方法(出力用として定義
	sd.OutputWindow = hWnd;		// 関連付けるウィンドウのハンドル
	sd.SampleDesc.Count = 1;	// マルチサンプリング数
	sd.SampleDesc.Quality = 0;	// マルチサンプリングクオリティ(最大)
	//sd.Windowed = TRUE;		//ウィンドウモードで作成して後からフルスクリーンにする
	sd.Windowed = fullScreen ? FALSE : TRUE;

	Microsoft::WRL::ComPtr<ID3D11Device> deviceTemp;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> contextTemp;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChainTemp;

	// デバイスとスワップチェインの作成
	for (UINT driverTypeIdx = 0; driverTypeIdx < driverTypesNum; ++driverTypeIdx)
	{
		// ドライバタイプを配列から取得
		D3DDriverType = driverTypes[driverTypeIdx];
		// デバイスとスワップチェインの作成
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,			// ディスプレイデバイスのアダプタ(nullの場合最初に見つかったアダプタ
			D3DDriverType,		// デバイスドライバのタイプ
			nullptr,			// ソフトウェアラスタライザを使用する場合に指定
			createDeviceFlags,	// デバイスフラグ
			featureLevels,		// 機能レベル
			featureLevelsNum,	// 機能レベル数
			D3D11_SDK_VERSION,	//
			&sd,				// スワップチェインの設定
			&swapChainTemp,		// IDXGISwapChainインターフェイス
			&deviceTemp,		// ID3D11Deviceインターフェイス
			&D3DFeatureLevel,	// サポートされている機能レベル
			&contextTemp		// デバイスコンテキスト
		);
		// 成功したらそのドライバを使用する
		if (SUCCEEDED(hr))
			break;
	}
	ThrowIfFailed(
		hr,
		"DX11デバイスとスワップチェインの作成に失敗",
		"D3D11CreateDeviceAndSwapChain()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// リソースをバージョンアップする
	ThrowIfFailed(
		deviceTemp.As(&pD3D11Device),
		"DX11デバイスのバージョンアップに失敗",
		"deviceTemp.As(pD3D11Device)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		contextTemp.As(&pD3D11Context),
		"DX11コンテキストのバージョンアップに失敗",
		"contextTemp.As(&pD3D11Context)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		swapChainTemp.As(&pD3D11SwapChain),
		"DX11スワップチェインのバージョンアップに失敗",
		"swapChainTemp.As(&pD3D11SwapChain)",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// Direct2Dデバイスオブジェクトと対応するコンテキストを作成
	Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
	ThrowIfFailed(
		pD3D11Device.As(&dxgiDevice),
		"dxgiDeviceのバージョンアップに失敗",
		"pD3D11Device.As(&dxgiDevice)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		pD2DFactory->CreateDevice(dxgiDevice.Get(), &pD2DDevice),
		"dxgiDeviceのバージョンアップに失敗",
		"pD2DFactory->CreateDevice()",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&pD2DContext),
		"2dデバイスコンテキスト作成に失敗",
		"pD2DDevice->CreateDeviceContext()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	if (fullScreen)
	{
		ThrowIfFailed(
			pD3D11SwapChain->SetFullscreenState(true, nullptr),
			"フルスクリーン移行に失敗",
			"pD3D11SwapChain->SetFullscreenState(true,nullptr)",
			"DeviceResources::Param::CreateDeviceResources()"
		);
	}
}


DeviceResources::DeviceResources(HWND hWnd, bool fullScreen, UINT screenWidth, UINT screenHeight)
	: m_param(new Param(hWnd, fullScreen, screenWidth, screenHeight))
{
	// レンダリングステートの作成
	m_param->pRenderState = std::make_shared<RenderState>();
}

DeviceResources::~DeviceResources()
{
}

ID3D11Device2 * DeviceResources::GetD3DDevice() const
{
	return m_param->pD3D11Device.Get();
}

ID3D11DeviceContext2 * DeviceResources::GetD3DDeviceContext() const
{
	return m_param->pD3D11Context.Get();
}

IDXGISwapChain1 * DeviceResources::GetSwapChain() const
{
	return m_param->pD3D11SwapChain.Get();
}

D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const
{
	return m_param->D3DFeatureLevel;
}

ID2D1Factory2 * DeviceResources::GetD2DFactory() const
{
	return m_param->pD2DFactory.Get();
}

ID2D1Device1 * DeviceResources::GetD2DDevice() const
{
	return m_param->pD2DDevice.Get();
}

ID2D1DeviceContext1 * DeviceResources::GetD2DDeviceContext() const
{
	return m_param->pD2DContext.Get();
}

IDWriteFactory2 * DeviceResources::GetDWriteFactory() const
{
	return m_param->pDwriteFactory.Get();
}

IWICImagingFactory2 * DeviceResources::GetWicImagingFactory() const
{
	return m_param->pWicFactory.Get();
}

void DeviceResources::ClearDefaultView(const Float4 & color)
{
	auto defaultTarget = GetDefaultRenderTarget();
	defaultTarget->ClearViews(color);
}

void DeviceResources::StartDefaultDraw()
{
	auto defaultTarget = GetDefaultRenderTarget();
	defaultTarget->StartRenderTarget();
}

void DeviceResources::EndDefaultDraw()
{
	auto defaultTarget = GetDefaultRenderTarget();
	defaultTarget->EndRenderTarget();
}

void DeviceResources::InitailizeStates()
{
	ID3D11ShaderResourceView* pNullResouceView[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
	ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
	ID3D11Buffer* pNullConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };

	auto pID3D11DeviceContext = GetD3DDeviceContext();

	// 後始末
	// コンスタントバッファクリア
	pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	// シェーダーリソースクリア
	pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullResouceView);
	// サンプラークリア
	pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
	// シェーダクリア
	pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
	// インプットレイアウトクリア
	pID3D11DeviceContext->IASetInputLayout(nullptr);

	// デフォルト設定
	// リニアサンプラーを設定
	ID3D11SamplerState* pSamplerState = m_param->GetLinearClampSampler();
	pID3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	// ブレンド塗りつぶし
	pID3D11DeviceContext->OMSetBlendState(m_param->GetOpaqueBlend(), nullptr, 0xffffffff);
	// レンダーステート前面のみ描画
	pID3D11DeviceContext->RSSetState(m_param->GetCullBackRasterizer());
	// デプスステンシル使用
	pID3D11DeviceContext->OMSetDepthStencilState(m_param->GetDefaultDepthStencil(), 0);
}

void DeviceResources::Present(unsigned int syncInterval, unsigned int flags)
{
	// バックバッファからフロントバッファへ転送
	GetSwapChain()->Present(syncInterval, flags);
}

std::shared_ptr<DefaultRenderTarget> DeviceResources::GetDefaultRenderTarget()
{
	if (!m_param->pDefaultRenderTarget)
	{
		// デフォルトレンダーターゲットを作成
		m_param->pDefaultRenderTarget = std::make_shared<DefaultRenderTarget>();
	}
	return m_param->pDefaultRenderTarget;
}

std::shared_ptr<RenderState> DeviceResources::GetRenderState() const
{
	return m_param->pRenderState;
}

#pragma endregion

//==========================================
// レンダリングステート
//==========================================
#pragma region RenderState

struct RenderState::Param
{
	// ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> pOpaque;			// 塗りつぶしのブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAlphaBlend;		// 単純な透明処理ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAlphaBlendEx;		// 半透明ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAdditive;			// 加算処理ブレンドステート
	Microsoft::WRL::ComPtr<ID3D11BlendState> pNonPremultiplied;	// アルファを使用した元のデータと対象データをブレンド
	// デプスステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthNone;		// 深度処理無しのデプスステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthDefault;	// 通常深度のデプスステンシルステート
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthRead;		// 読み取り専用のデプスステンシルステート
	// ラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullNone;			// カリング無しのラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullNoneScissor;		// カリング無しのラスタライザステート(シーサー処理)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullFront;			// フロントカリング(背面描画のみ)のラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullFrontScissor;	// フロントカリング(背面描画のみ)のラスタライザステート(シーサー処理)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullBack;			// バックカリング(前面面描画のみ)のラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullBackScissor;		// バックカリング(前面描画のみ)のラスタライザステート(シーサー処理)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pWireframe;			// ワイヤフレームのラスタライザステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pWireframeScissor;	// ワイヤフレームのラスタライザステート(シーサー処理)
	// サンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pPointWrap;			// 点フィルタリングとテクスチャ座標ラップのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pPointClamp;			// 点フィルタリングとテクスチャ座標クランプのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pLinearWrap;			// 線形フィルタリングとテクスチャ座標ラップのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pLinearClamp;		// 線形フィルタリングテクスチャ座標クランプのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pAnisotropicWrap;	// 異方性フィルタリングとテクスチャ座標ラップのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pAnisotropicClamp;	// 異方性フィルタリングとテクスチャ座標クランプのサンプラーステート
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pComparisonLinear;	// 線形フィルタリングを使った比較サンプラーステート

	// ミューテックス
	std::mutex mutex;

	Param() {}
	~Param() {}

	/// <summary>
	/// ブレンドステートを作成する
	/// </summary>
	/// <param name="srcBlend"></param>
	/// <param name="destBlend"></param>
	/// <param name="ppResult">受け取るインターフェイス</param>
	void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** ppResult);
	/// <summary>
	/// デプスステンシルステートを作成する
	/// </summary>
	/// <param name="enable"></param>
	/// <param name="writeEnable"></param>
	/// <param name="ppResult"></param>
	void CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState** ppResult);
	/// <summary>
	/// ラスタライザステートを作成
	/// </summary>
	/// <param name="cullMode">カリング</param>
	/// <param name="fillMode">塗り</param>
	/// <param name="ppResult">受け取るインターフェイス</param>
	/// <param name="scissor">画面分割するかどうか</param>
	void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** ppResult, bool scissor = false);
	/// <summary>
	/// サンプラーステートを作成する
	/// </summary>
	/// <param name="filter">フィルター</param>
	/// <param name="addressMode">アドレスモード</param>
	/// <param name="ppResult">受け取るインターフェイス</param>
	void CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** ppResult);
};

void RenderState::Param::CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** ppResult)
{
	try
	{
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();

		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) || (destBlend != D3D11_BLEND_ZERO);
		desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
		desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
		desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = pDX11Device->CreateBlendState(&desc, ppResult);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw StandardException(
				"ブレンドステート作成失敗",
				"if(FAILED(pDX11Device->CreateBlendState()))",
				"RenderState::Param::CreateBlendState()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

void RenderState::Param::CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState** ppResult)
{
	try
	{
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();

		D3D11_DEPTH_STENCIL_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.DepthEnable = enable;
		desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
		desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		desc.StencilEnable = false;
		desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		desc.BackFace = desc.FrontFace;

		HRESULT hr = pDX11Device->CreateDepthStencilState(&desc, ppResult);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw StandardException(
				"デプスステンシルステート作成失敗",
				"if(FAILED(pDX11Device->CreateDepthStencilState()))",
				"RenderState::Param::CreateDepthStencilState()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

void RenderState::Param::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** ppResult, bool scissor)
{
	try
	{
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();

		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.CullMode = cullMode;
		desc.FillMode = fillMode;
		desc.DepthClipEnable = true;
		desc.MultisampleEnable = true;
		desc.ScissorEnable = scissor;

		HRESULT hr = pDX11Device->CreateRasterizerState(&desc, ppResult);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw StandardException(
				"ラスタライザステート作成失敗",
				"if(FAILED(pDX11Device->CreateRasterizerState()))",
				"RenderState::Param::CreateRasterizerState()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

void RenderState::Param::CreateSamplerState(D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode, ID3D11SamplerState** ppResult)
{
	try
	{
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();

		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Filter = filter;

		desc.AddressU = addressMode;
		desc.AddressV = addressMode;
		desc.AddressW = addressMode;

		desc.MaxAnisotropy = (pDX11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

		desc.MaxLOD = FLT_MAX;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

		HRESULT hr = pDX11Device->CreateSamplerState(&desc, ppResult);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw StandardException(
				"サンプラーステート作成失敗",
				"if(FAILED(pDX11Device->CreateSamplerState()))",
				"RenderState::Param::CreateSamplerState()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

RenderState::RenderState()
	: m_pParam(new Param())
{
}

RenderState::~RenderState()
{
}

ID3D11BlendState * RenderState::GetOpaque() const
{
	return DemandCreate(m_pParam->pOpaque, m_pParam->mutex, [&](ID3D11BlendState** ppResult)
	{
		m_pParam->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, ppResult);
	});
}

ID3D11BlendState * RenderState::GetAlphaBlend() const
{
	return DemandCreate(m_pParam->pAlphaBlend, m_pParam->mutex, [&](ID3D11BlendState** ppResult)
	{
		m_pParam->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, ppResult);
	});
}

ID3D11BlendState * RenderState::GetAlphaBlendEx() const
{
	if (!m_pParam->pAlphaBlendEx)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		HRESULT hr = pDX11Device->CreateBlendState(&desc, &m_pParam->pAlphaBlendEx);
		if (FAILED(hr))
		{
			// 初期化失敗
			throw StandardException(
				"ブレンドステート作成に失敗",
				"if(FAILED(pDX11Device->CreateBlendState()))",
				"BasicState::GetAlphaBlendEx()"
			);
		}
	}

	return m_pParam->pAlphaBlendEx.Get();
}

ID3D11BlendState * RenderState::GetAdditive() const
{
	return DemandCreate(m_pParam->pAdditive, m_pParam->mutex, [&](ID3D11BlendState** ppResult)
	{
		m_pParam->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, ppResult);
	});
}

ID3D11BlendState * RenderState::GetNonPremultiplied() const
{
	return DemandCreate(m_pParam->pNonPremultiplied, m_pParam->mutex, [&](ID3D11BlendState** ppResult)
	{
		m_pParam->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, ppResult);
	});
}

ID3D11DepthStencilState * RenderState::GetDepthNone() const
{
	return DemandCreate(m_pParam->pDepthNone, m_pParam->mutex, [&](ID3D11DepthStencilState** ppResult)
	{
		m_pParam->CreateDepthStencilState(false, false, ppResult);
	});
}

ID3D11DepthStencilState * RenderState::GetDepthDefault() const
{
	return DemandCreate(m_pParam->pDepthDefault, m_pParam->mutex, [&](ID3D11DepthStencilState** ppResult)
	{
		m_pParam->CreateDepthStencilState(true, true, ppResult);
	});
}

ID3D11DepthStencilState * RenderState::GetDepthRead() const
{
	return DemandCreate(m_pParam->pDepthRead, m_pParam->mutex, [&](ID3D11DepthStencilState** ppResult)
	{
		m_pParam->CreateDepthStencilState(true, false, ppResult);
	});
}

ID3D11RasterizerState * RenderState::GetCullNone() const
{
	return DemandCreate(m_pParam->pCullNone, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, ppResult);
	});
}

ID3D11RasterizerState * RenderState::GetCullNoneScissor() const
{
	return DemandCreate(m_pParam->pCullNoneScissor, m_pParam->mutex, [&](ID3D11RasterizerState** ppRsult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, ppRsult, true);
	});
}

ID3D11RasterizerState * RenderState::GetCullFront() const
{
	return DemandCreate(m_pParam->pCullFront, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, ppResult);
	});
}

ID3D11RasterizerState * RenderState::GetCullFrontScissor() const
{
	return DemandCreate(m_pParam->pCullFrontScissor, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, ppResult, true);
	});
}

ID3D11RasterizerState * RenderState::GetCullBack() const
{
	return DemandCreate(m_pParam->pCullBack, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, ppResult);
	});
}

ID3D11RasterizerState * RenderState::GetCullBackScissor() const
{
	return DemandCreate(m_pParam->pCullBackScissor, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, ppResult, true);
	});
}

ID3D11RasterizerState * RenderState::GetWireframe() const
{
	return DemandCreate(m_pParam->pWireframe, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, ppResult);
	});
}

ID3D11RasterizerState * RenderState::GetWireframeScissor() const
{
	return DemandCreate(m_pParam->pWireframeScissor, m_pParam->mutex, [&](ID3D11RasterizerState** ppResult)
	{
		m_pParam->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, ppResult, true);
	});
}

ID3D11SamplerState * RenderState::GetPointWrap() const
{
	return DemandCreate(m_pParam->pPointWrap, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetPointClamp() const
{
	return DemandCreate(m_pParam->pPointClamp, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetLinearWrap() const
{
	return DemandCreate(m_pParam->pLinearWrap, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetLinearClamp() const
{
	return DemandCreate(m_pParam->pLinearClamp, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetAnisotropicWrap() const
{
	return DemandCreate(m_pParam->pAnisotropicClamp, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetAnisotropicClamp() const
{
	return DemandCreate(m_pParam->pAnisotropicClamp, m_pParam->mutex, [&](ID3D11SamplerState** ppResult)
	{
		m_pParam->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, ppResult);
	});
}

ID3D11SamplerState * RenderState::GetComparisonLinear() const
{
	if (!m_pParam->pComparisonLinear)
	{
		auto* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 1.0f;
		desc.BorderColor[3] = 1.0f;
		desc.MinLOD = 0.f;
		desc.MaxLOD = D3D11_FLOAT32_MAX;
		desc.MipLODBias = 0.f;
		desc.MaxAnisotropy = 0;
		desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
		desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
		ThrowIfFailed(
			pDX11Device->CreateSamplerState(&desc, &m_pParam->pComparisonLinear),
			"比較用サンプラーの作成失敗",
			"pDX11Device->CreateSamplerState(&desc, &m_pParam->pComparisonLinear)",
			"RenderState::GetComparisonLinear()"
		);
	}

	return m_pParam->pComparisonLinear.Get();
}

void RenderState::SetBlendState(ID3D11DeviceContext2 * pContext, BlendState state)
{
	switch (state)
	{
	case BlendState::Opaque:
		pContext->OMSetBlendState(GetOpaque(), nullptr, 0xffffffff);
		break;
	case BlendState::AlphaBlend:
		pContext->OMSetBlendState(GetAlphaBlendEx(), nullptr, 0xffffffff);
		break;
	case BlendState::Additive:
		pContext->OMSetBlendState(GetAdditive(), nullptr, 0xffffffff);
		break;
	case BlendState::NonPremultiplied:
		pContext->OMSetBlendState(GetNonPremultiplied(), nullptr, 0xffffffff);
		break;
	default:
		pContext->OMSetBlendState(GetOpaque(), nullptr, 0xffffffff);
		break;
	}
}

void RenderState::SetDepthStencilState(ID3D11DeviceContext2 * pContext, DepthStencilState state)
{
	switch (state)
	{
	case DepthStencilState::None:
		pContext->OMSetDepthStencilState(GetDepthNone(), 0);
		break;
	case DepthStencilState::Default:
		pContext->OMSetDepthStencilState(GetDepthDefault(), 0);
		break;
	case DepthStencilState::Read:
		pContext->OMSetDepthStencilState(GetDepthRead(), 0);
		break;
	}
}

void RenderState::SetRasterizerState(ID3D11DeviceContext2 * pContext, RasterizerState state)
{
	switch (state)
	{
	case RasterizerState::CullNone:
		pContext->RSSetState(GetCullNone());
		break;
	case RasterizerState::CullFront:
		pContext->RSSetState(GetCullFront());
		break;
	case RasterizerState::CullBack:
		pContext->RSSetState(GetCullBack());
		break;
	case RasterizerState::Wireframe:
		pContext->RSSetState(GetWireframe());
		break;
	case RasterizerState::DoubleDraw:
		pContext->RSSetState(GetCullFront());
		break;
	default:
		break;
	}
}

void RenderState::SetSamplerState(ID3D11DeviceContext2 * pContext, SamplerState state, UINT slot)
{
	ID3D11SamplerState* pSampler = nullptr;
	ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };

	switch (state)
	{
	case SamplerState::SamplerNone:
		pContext->PSSetSamplers(slot, 1, pNullSR);
		break;
	case SamplerState::PointWrap:
		pSampler = GetPointWrap();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::PointClamp:
		pSampler = GetPointClamp();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::LinearWrap:
		pSampler = GetLinearWrap();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::LinearClamp:
		pSampler = GetLinearClamp();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::AnisotropicWrap:
		pSampler = GetAnisotropicWrap();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::AnisotropicClamp:
		pSampler = GetAnisotropicClamp();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	case SamplerState::ComparisonLinear:
		pSampler = GetComparisonLinear();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	default:
		pSampler = GetLinearClamp();
		pContext->PSSetSamplers(slot, 1, &pSampler);
		break;
	}
}

void RenderState::SetSamplerAllClear(ID3D11DeviceContext2 * pContext)
{
	ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
	pContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
}


#pragma endregion


//=========================================
// レンダーターゲット基底クラス
//=========================================
#pragma region RenderTarget
struct RenderTarget::Param
{
	D3D11_VIEWPORT viewPort;	// ビューポート
	Param()
	{
		ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));
	}
};

RenderTarget::RenderTarget()
	: m_pParam(new Param())
{
}

RenderTarget::~RenderTarget()
{
}

#pragma endregion

const D3D11_VIEWPORT & RenderTarget::GetViewPort() const
{
	return m_pParam->viewPort;
}

void RenderTarget::SetViewPort(const D3D11_VIEWPORT & viewPort)
{
	m_pParam->viewPort = viewPort;
}

//=========================================
// デフォルトレンダリングターゲット
//=========================================
#pragma region DefaultRenderTarget
struct DefaultRenderTarget::Param
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pD3D11RenderTargetView;	// レンダリングターゲットビュー
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthStencil;			// 深度ステンシルバッファ
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthStencilView;		// 深度ステンシルビュー

	Microsoft::WRL::ComPtr<ID2D1Bitmap1>	pD2DTargetBitmap;

	Param() {}
	~Param() {}
};

DefaultRenderTarget::DefaultRenderTarget()
	: m_pParam(new Param())
{
	try
	{
		auto dev = APP->GetApp()->GetDeviceResources();
		auto pD3D11Device = dev->GetD3DDevice();
		auto pSwapChain = dev->GetSwapChain();
		//auto pD3D11DeviceContext = dev->GetD3DDeviceContext();
		auto pD2D11DeviceContext = dev->GetD2DDeviceContext();

		// レンダリングターゲットビューの作成
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
		// バックバッファのポインタを得る
		ThrowIfFailed(
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
			"スワップチェインからバックバッファの取得に失敗",
			"pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&pBackBuffer)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);
		// バックバッファからレンダリングターゲットのビューを作成
		ThrowIfFailed(
			pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pParam->pD3D11RenderTargetView),
			"DX11バックバッファからのレンダリングターゲットビューの作成に失敗",
			"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(),nullptr,&m_pParam->pD3D11RenderTargetView",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		// 深度テクスチャの作成
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = APP->GetScreenWidth();
		descDepth.Height = APP->GetScreenHeight();
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;

		ThrowIfFailed(
			pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_pParam->pDepthStencil),
			"DX11深度テクスチャの作成に失敗",
			"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_pParam->pDepthStencil)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		// 深度ステンシルビューの作成
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			pD3D11Device->CreateDepthStencilView(m_pParam->pDepthStencil.Get(), &descDSV, &m_pParam->pDepthStencilView),
			"DX11深度ステンシルビューの作成に失敗",
			"pD3D11Device->CreateDepthStencilView(m_pParam->pDepthStencil.Get(), &descDSV, &m_pParam->pDepthStencilView)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		Microsoft::WRL::ComPtr<IDXGISurface2> dxgiBackBuffer;
		ThrowIfFailed(
			pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
			"2dデバイスコンテキスト作成に失敗",
			"pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		ThrowIfFailed(
			pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				nullptr,
				&m_pParam->pD2DTargetBitmap
			),
			"2dビットマップ作成に失敗",
			"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		pD2D11DeviceContext->SetTarget(m_pParam->pD2DTargetBitmap.Get());
		// グレースケールテキストのアンチエイリアシング
		pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

		// デフォルトビューポートのセット
		D3D11_VIEWPORT viewPort;
		ZeroMemory(&viewPort, sizeof(viewPort));
		viewPort.Width = (float)APP->GetScreenWidth();
		viewPort.Height = (float)APP->GetScreenHeight();
		viewPort.MinDepth = 0.0f;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0.0f;
		viewPort.TopLeftY = 0.0f;
		SetViewPort(viewPort);
	}
	catch (...)
	{
		throw;
	}
}

DefaultRenderTarget::~DefaultRenderTarget()
{
}

void DefaultRenderTarget::ClearViews(const Float4 & color)
{
	auto dev = APP->GetDeviceResources();
	//auto pD3D11Device = dev->GetD3DDevice();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();
	// バッファのクリア
	float _color[4] = { color.x,color.y,color.z,color.w };
	D3D11_VIEWPORT viewPort;
	// ビューポートのセットアップ
	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.Width = (float)APP->GetScreenWidth();
	viewPort.Height = (float)APP->GetScreenHeight();
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	pD3D11DeviceContext->RSSetViewports(1, &viewPort);
	// レンダリングターゲットのクリア
	pD3D11DeviceContext->ClearRenderTargetView(m_pParam->pD3D11RenderTargetView.Get(), _color);
	// 通常の深度バッファとステンシルバッファのクリア
	pD3D11DeviceContext->ClearDepthStencilView(m_pParam->pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DefaultRenderTarget::StartRenderTarget()
{
	auto dev = APP->GetDeviceResources();
	//auto pD3D11Device = dev->GetD3DDevice();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();

	ID3D11RenderTargetView* pV = m_pParam->pD3D11RenderTargetView.Get();
	// レンダリングターゲットとステンシルを設定
	pD3D11DeviceContext->OMSetRenderTargets(1, &pV, m_pParam->pDepthStencilView.Get());
	// ビューポートの設定
	auto viewPort = GetViewPort();
	pD3D11DeviceContext->RSSetViewports(1, &viewPort);

	D3D11_RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (LONG)viewPort.Width;
	rect.bottom = (LONG)viewPort.Height;
	pD3D11DeviceContext->RSSetScissorRects(1, &rect);

	// シェーダーリソースビューのクリア
	ID3D11ShaderResourceView* pNull[1] = { nullptr };
	pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
	pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
	// シェーダーは指定しない
	pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	// ブレンドは指定しない
	pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void DefaultRenderTarget::EndRenderTarget()
{
	auto dev = APP->GetDeviceResources();
	auto pD3D11Device = dev->GetD3DDevice();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();
	// シェーダーリソースビューのクリア
	ID3D11ShaderResourceView* pNull[1] = { nullptr };
	pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
	pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
	// シェーダーは指定しない
	pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	// ブレンドは指定しない
	pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

ID3D11RenderTargetView * DefaultRenderTarget::GetRenderTargetView() const
{
	return m_pParam->pD3D11RenderTargetView.Get();
}

ID3D11Texture2D * DefaultRenderTarget::GetDepthStencil() const
{
	return m_pParam->pDepthStencil.Get();
}

ID3D11DepthStencilView * DefaultRenderTarget::GetDepthStencilView() const
{
	return m_pParam->pDepthStencilView.Get();
}

ID2D1Bitmap1 * DefaultRenderTarget::GetD2DTargetBitmap() const
{
	return m_pParam->pD2DTargetBitmap.Get();
}

#pragma endregion

//==========================================================
// ShaderResource
//==========================================================
#pragma region ShaderResource

ShaderResource::ShaderResource()
{
}

ShaderResource::~ShaderResource()
{
}

void ShaderResource::ReadBinaryFile(const std::string & fileName, std::unique_ptr<uint8_t[]>& data, size_t & csoSize)
{
	try
	{
		DWORD retCode;
		retCode = GetFileAttributes(fileName.c_str());
		if (retCode == -1)
		{
			throw StandardException(
				"ファイルが存在しない",
				fileName,
				"ShaderResource::ReadBinariFile"
			);
		}

		ThrowIfFailed(
			BinaryReader::ReadEntireFile(fileName, data, &csoSize),
			"ファイル読み込み失敗",
			fileName,
			"ShaderResource::ReadBinaryFile()"
		);
	}
	catch (...)
	{
		throw;
	}
}

void ShaderResource::CreateVertexShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11VertexShader ** ppResult)
{
	try
	{
		ID3D11Device* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		// 頂点シェーダ作成
		ThrowIfFailed(
			pDX11Device->CreateVertexShader(data.get(), csoSize, nullptr, ppResult),
			"頂点シェーダ作成失敗",
			"if(FAILED(pDX11Device->CreateVertexShader())",
			"ShaderResource::CreateVertexShader()"
		);
	}
	catch (...)
	{
		throw;
	}
}

void ShaderResource::CreateInputLayout(std::unique_ptr<uint8_t[]>& data, size_t csoSize, const D3D11_INPUT_ELEMENT_DESC * pElement, UINT elementsNum, ID3D11InputLayout ** ppResult)
{
	try
	{
		ID3D11Device* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();

		// インプットレイアウト作成
		HRESULT hr = pDX11Device->CreateInputLayout(
			pElement,
			elementsNum,
			data.get(),
			csoSize,
			ppResult
		);
		if (FAILED(hr))
		{
			throw StandardException(
				"頂点シェーダのインプットレイアウト作成失敗",
				"if(FAILED(pDX11Device->CreateInputLayout()))",
				"ShaderResource::CreateInputLayout()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

void ShaderResource::CreatePixelShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11PixelShader ** ppResult)
{
	try
	{
		ID3D11Device* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		
		// ピクセルシェーダ作成
		HRESULT hr = pDX11Device->CreatePixelShader(
			data.get(),
			csoSize,
			nullptr,
			ppResult
		);
		if (FAILED(hr))
		{
			throw StandardException(
				"ピクセルシェーダ作成失敗",
				"if(FAILED(pDX11Device->CreatePixelShader()))",
				"ShaderResource::CreatePixelShader()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

void ShaderResource::CreateConstantBuffer(UINT buffSize, ID3D11Buffer ** ppResult)
{
	try
	{
		ID3D11Device* pDX11Device = APP->GetDeviceResources()->GetD3DDevice();

		// 定数バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = buffSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;
		HRESULT hr = pDX11Device->CreateBuffer(&bd, nullptr, ppResult);
		if (FAILED(hr))
		{
			throw StandardException(
				"定数バッファ作成失敗",
				"if(FAILED(pDX11Device->CreateBuffer()))",
				"ShaderResource::CreateConstantBuffer()"
			);
		}
	}
	catch (...)
	{
		throw;
	}
}

ID3D11VertexShader * ShaderResource::GetVertexShaderBase(const std::string & fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& pVS)
{
	return DemandCreate(pVS, mutexBase, [&](ID3D11VertexShader** ppResult)
	{
		std::unique_ptr<uint8_t[]> data;
		// シェーダ読み込み
		size_t csoSize = 0;
		ReadBinaryFile(fileName, data, csoSize);
		CreateVertexShader(data, csoSize, ppResult);
	});
}

ID3D11PixelShader * ShaderResource::GetPixelShaderBase(const std::string & fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pPS)
{
	return DemandCreate(pPS, mutexBase, [&](ID3D11PixelShader** ppResult)
	{
		std::unique_ptr<uint8_t[]> data;
		// シェーダ読み込み
		size_t csoSize = 0;
		ReadBinaryFile(fileName, data, csoSize);
		CreatePixelShader(data, csoSize, ppResult);
	});
}

ID3D11InputLayout * ShaderResource::GetInputLayoutBase(const std::string & fileName, Microsoft::WRL::ComPtr<ID3D11InputLayout>& pIL, const D3D11_INPUT_ELEMENT_DESC * pElem, UINT elementsNum)
{
	return DemandCreate(pIL, mutexBase, [&](ID3D11InputLayout** ppResult)
	{
		std::unique_ptr<uint8_t[]> data;
		// シェーダ読み込み
		size_t csoSize = 0;
		ReadBinaryFile(fileName, data, csoSize);
		CreateInputLayout(data, csoSize, pElem, elementsNum, ppResult);
	});
}

ID3D11Buffer * ShaderResource::GetConstantBufferBase(UINT buffSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer)
{
	return DemandCreate(buffer, mutexBase, [&](ID3D11Buffer** ppResult)
	{
		CreateConstantBuffer(buffSize, ppResult);
	});
}

#pragma endregion
