#include "DeviceResource.h"
#include <App.h>
#include <BinaryReader.h>
#include <ThrowException.h>


//===========================================
// DirectX11�f�o�C�X�N���X
//===========================================
#pragma region DeviceResources
struct DeviceResources::Param
{
	D3D_DRIVER_TYPE		D3DDriverType;	// �h���C�o�̎��
	D3D_FEATURE_LEVEL	D3DFeatureLevel;	// �@�\���x��
	// Direct3D
	Microsoft::WRL::ComPtr<ID3D11Device2>			pD3D11Device;		// ID3D11Device�C���^�[�t�F�C�X
	Microsoft::WRL::ComPtr<ID3D11DeviceContext2>	pD3D11Context;		// �f�o�C�X�R���e�L�X�g
	Microsoft::WRL::ComPtr<IDXGISwapChain1>			pD3D11SwapChain;	// IDXGIDrapChain�C���^�[�t�F�C�X
	// Direct2D
	Microsoft::WRL::ComPtr<ID2D1Factory2>			pD2DFactory;	// ID2D1Factory�C���^�[�t�F�C�X
	Microsoft::WRL::ComPtr<ID2D1Device1>			pD2DDevice;		// ID2D1Device�C���^�[�t�F�C�X
	Microsoft::WRL::ComPtr<ID2D1DeviceContext1>		pD2DContext;	// ID2D1�f�o�C�X�R���e�L�X�g
	// DirectWrite
	Microsoft::WRL::ComPtr<IDWriteFactory2>		pDwriteFactory;
	Microsoft::WRL::ComPtr<IWICImagingFactory2>	pWicFactory;

	float dpi;	//DPI

	std::shared_ptr<DefaultRenderTarget>	pDefaultRenderTarget;	// �f�t�H���g�����_�����O�^�[�Q�b�g
	std::shared_ptr<RenderState>			pRenderState;			// �����_�����O�X�e�[�g

	/// <summary>
	/// �T���v���[�X�e�[�g���쐬
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
				// ���������s
				throw StandardException(
					"�T���v���[�X�e�[�g�쐬���s",
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
	/// �u�����h�X�e�[�g���쐬
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
				// ���������s
				throw StandardException(
					"�u�����h�X�e�[�g�쐬�Ɏ��s",
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
	/// �f�v�X�X�e���V���X�e�[�g���쐬
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
				// ���������s
				throw StandardException(
					"�f�v�X�X�e���V���X�e�[�g�쐬���s",
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
	/// ���X�^���C�U�X�e�[�g���쐬
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
				// ���������s
				throw StandardException(
					"���X�^���C�U�X�e�[�g�쐬���s",
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
	std::mutex mutex;	// �~���[�e�b�N�X

	Param(HWND hWnd, bool fullScreen, UINT width, UINT height);
	~Param() {}

	/// <summary>
	/// ���\�[�X�̍\�z
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
		// ���\�[�X�̍\�z
		CreateDeviceResouces(hWnd, fullScreen, width, height);
	}
	catch (...)
	{
		throw;
	}
}

void DeviceResources::Param::CreateDeviceResouces(HWND hWnd, bool fullScreen, UINT width, UINT height)
{
	// Direct2D���\�[�X��������
	D2D1_FACTORY_OPTIONS options;
	ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#ifdef _DEBUG
	// �v���W�F�N�g���f�o�b�O�r���h�Ɋ܂܂�Ă���ꍇ�́ADirect2D�f�o�b�O��SDK���C���[����ėL���ɂ���
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif // _DEBUG
	// Direct2D�t�@�N�g����������
	ThrowIfFailed(
		D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory2),
			&options,
			&pD2DFactory
		),
		"Factory�쐬�Ɏ��s",
		"D2D1CreateFactory()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// DirectWrite�t�@�N�g����������
	ThrowIfFailed(
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory2),
			&pDwriteFactory
		),
		"DirectWrite�t�@�N�g���쐬�Ɏ��s",
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
		"(WIC)�t�@�N�g���쐬�Ɏ��s",
		"CoCreateInstance()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	HRESULT hr;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;	// �f�o�C�X�쐬�p�t���O
	// �h���C�o�̎��
	D3D_DRIVER_TYPE driverTypes[] = {
		D3D_DRIVER_TYPE_HARDWARE,		// GPU�ŕ`��
		D3D_DRIVER_TYPE_WARP,			// �����x(�ᑬ
		D3D_DRIVER_TYPE_REFERENCE,		// CPU�ŕ`��
	};
	// �I���ł���h���C�o�̐�
	UINT driverTypesNum = ARRAYSIZE(driverTypes);

	// �@�\�Z�b�g
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_11_1,		// DirectX11.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_11_0,		// DirectX11�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_1,		// DirectX10.1�Ή�GPU���x��
		D3D_FEATURE_LEVEL_10_0,		// DirectX10�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_3,		// DirectX9.3�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_2,		// DirectX9.2�Ή�GPU���x��
		D3D_FEATURE_LEVEL_9_1		// Direct9.1�Ή�GPU���x��
	};
	// �I���ł���@�\���x���̐�
	UINT featureLevelsNum = ARRAYSIZE(featureLevels);

	// �X���b�v�`�F�[���̃f�X�N�v���^�̐ݒ�
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 2;		// �o�b�t�@��
	sd.BufferDesc.Width = width;	// �o�b�t�@�̕�
	sd.BufferDesc.Height = height;	// �o�b�t�@�̍���
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// �o�b�N�o�b�t�@�t�H�[�}�b�g(R,G,B,A)16�r�b�g�����Ȃ����K�������t�H�[�}�b�g
	sd.BufferDesc.RefreshRate.Numerator = 1000;	// ���t���b�V�����[�g�ő�l
	sd.BufferDesc.RefreshRate.Denominator = 1;	// ���t���b�V�����[�g�ŏ��l
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// �o�b�N�o�b�t�@�̎g�p���@(�o�͗p�Ƃ��Ē�`
	sd.OutputWindow = hWnd;		// �֘A�t����E�B���h�E�̃n���h��
	sd.SampleDesc.Count = 1;	// �}���`�T���v�����O��
	sd.SampleDesc.Quality = 0;	// �}���`�T���v�����O�N�I���e�B(�ő�)
	//sd.Windowed = TRUE;		//�E�B���h�E���[�h�ō쐬���Čォ��t���X�N���[���ɂ���
	sd.Windowed = fullScreen ? FALSE : TRUE;

	Microsoft::WRL::ComPtr<ID3D11Device> deviceTemp;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> contextTemp;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChainTemp;

	// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
	for (UINT driverTypeIdx = 0; driverTypeIdx < driverTypesNum; ++driverTypeIdx)
	{
		// �h���C�o�^�C�v��z�񂩂�擾
		D3DDriverType = driverTypes[driverTypeIdx];
		// �f�o�C�X�ƃX���b�v�`�F�C���̍쐬
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,			// �f�B�X�v���C�f�o�C�X�̃A�_�v�^(null�̏ꍇ�ŏ��Ɍ��������A�_�v�^
			D3DDriverType,		// �f�o�C�X�h���C�o�̃^�C�v
			nullptr,			// �\�t�g�E�F�A���X�^���C�U���g�p����ꍇ�Ɏw��
			createDeviceFlags,	// �f�o�C�X�t���O
			featureLevels,		// �@�\���x��
			featureLevelsNum,	// �@�\���x����
			D3D11_SDK_VERSION,	//
			&sd,				// �X���b�v�`�F�C���̐ݒ�
			&swapChainTemp,		// IDXGISwapChain�C���^�[�t�F�C�X
			&deviceTemp,		// ID3D11Device�C���^�[�t�F�C�X
			&D3DFeatureLevel,	// �T�|�[�g����Ă���@�\���x��
			&contextTemp		// �f�o�C�X�R���e�L�X�g
		);
		// ���������炻�̃h���C�o���g�p����
		if (SUCCEEDED(hr))
			break;
	}
	ThrowIfFailed(
		hr,
		"DX11�f�o�C�X�ƃX���b�v�`�F�C���̍쐬�Ɏ��s",
		"D3D11CreateDeviceAndSwapChain()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// ���\�[�X���o�[�W�����A�b�v����
	ThrowIfFailed(
		deviceTemp.As(&pD3D11Device),
		"DX11�f�o�C�X�̃o�[�W�����A�b�v�Ɏ��s",
		"deviceTemp.As(pD3D11Device)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		contextTemp.As(&pD3D11Context),
		"DX11�R���e�L�X�g�̃o�[�W�����A�b�v�Ɏ��s",
		"contextTemp.As(&pD3D11Context)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		swapChainTemp.As(&pD3D11SwapChain),
		"DX11�X���b�v�`�F�C���̃o�[�W�����A�b�v�Ɏ��s",
		"swapChainTemp.As(&pD3D11SwapChain)",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	// Direct2D�f�o�C�X�I�u�W�F�N�g�ƑΉ�����R���e�L�X�g���쐬
	Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
	ThrowIfFailed(
		pD3D11Device.As(&dxgiDevice),
		"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s",
		"pD3D11Device.As(&dxgiDevice)",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		pD2DFactory->CreateDevice(dxgiDevice.Get(), &pD2DDevice),
		"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s",
		"pD2DFactory->CreateDevice()",
		"DeviceResources::Param::CreateDeviceResources()"
	);
	ThrowIfFailed(
		pD2DDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&pD2DContext),
		"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s",
		"pD2DDevice->CreateDeviceContext()",
		"DeviceResources::Param::CreateDeviceResources()"
	);

	if (fullScreen)
	{
		ThrowIfFailed(
			pD3D11SwapChain->SetFullscreenState(true, nullptr),
			"�t���X�N���[���ڍs�Ɏ��s",
			"pD3D11SwapChain->SetFullscreenState(true,nullptr)",
			"DeviceResources::Param::CreateDeviceResources()"
		);
	}
}


DeviceResources::DeviceResources(HWND hWnd, bool fullScreen, UINT screenWidth, UINT screenHeight)
	: m_param(new Param(hWnd, fullScreen, screenWidth, screenHeight))
{
	// �����_�����O�X�e�[�g�̍쐬
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

	// ��n��
	// �R���X�^���g�o�b�t�@�N���A
	pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
	// �V�F�[�_�[���\�[�X�N���A
	pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNullResouceView);
	// �T���v���[�N���A
	pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
	// �V�F�[�_�N���A
	pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
	// �C���v�b�g���C�A�E�g�N���A
	pID3D11DeviceContext->IASetInputLayout(nullptr);

	// �f�t�H���g�ݒ�
	// ���j�A�T���v���[��ݒ�
	ID3D11SamplerState* pSamplerState = m_param->GetLinearClampSampler();
	pID3D11DeviceContext->PSSetSamplers(0, 1, &pSamplerState);
	// �u�����h�h��Ԃ�
	pID3D11DeviceContext->OMSetBlendState(m_param->GetOpaqueBlend(), nullptr, 0xffffffff);
	// �����_�[�X�e�[�g�O�ʂ̂ݕ`��
	pID3D11DeviceContext->RSSetState(m_param->GetCullBackRasterizer());
	// �f�v�X�X�e���V���g�p
	pID3D11DeviceContext->OMSetDepthStencilState(m_param->GetDefaultDepthStencil(), 0);
}

void DeviceResources::Present(unsigned int syncInterval, unsigned int flags)
{
	// �o�b�N�o�b�t�@����t�����g�o�b�t�@�֓]��
	GetSwapChain()->Present(syncInterval, flags);
}

std::shared_ptr<DefaultRenderTarget> DeviceResources::GetDefaultRenderTarget()
{
	if (!m_param->pDefaultRenderTarget)
	{
		// �f�t�H���g�����_�[�^�[�Q�b�g���쐬
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
// �����_�����O�X�e�[�g
//==========================================
#pragma region RenderState

struct RenderState::Param
{
	// �u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> pOpaque;			// �h��Ԃ��̃u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAlphaBlend;		// �P���ȓ��������u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAlphaBlendEx;		// �������u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> pAdditive;			// ���Z�����u�����h�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11BlendState> pNonPremultiplied;	// �A���t�@���g�p�������̃f�[�^�ƑΏۃf�[�^���u�����h
	// �f�v�X�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthNone;		// �[�x���������̃f�v�X�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthDefault;	// �ʏ�[�x�̃f�v�X�X�e���V���X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDepthRead;		// �ǂݎ���p�̃f�v�X�X�e���V���X�e�[�g
	// ���X�^���C�U�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullNone;			// �J�����O�����̃��X�^���C�U�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullNoneScissor;		// �J�����O�����̃��X�^���C�U�X�e�[�g(�V�[�T�[����)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullFront;			// �t�����g�J�����O(�w�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullFrontScissor;	// �t�����g�J�����O(�w�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g(�V�[�T�[����)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullBack;			// �o�b�N�J�����O(�O�ʖʕ`��̂�)�̃��X�^���C�U�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pCullBackScissor;		// �o�b�N�J�����O(�O�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g(�V�[�T�[����)
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pWireframe;			// ���C���t���[���̃��X�^���C�U�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> pWireframeScissor;	// ���C���t���[���̃��X�^���C�U�X�e�[�g(�V�[�T�[����)
	// �T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pPointWrap;			// �_�t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pPointClamp;			// �_�t�B���^�����O�ƃe�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pLinearWrap;			// ���`�t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pLinearClamp;		// ���`�t�B���^�����O�e�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pAnisotropicWrap;	// �ٕ����t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pAnisotropicClamp;	// �ٕ����t�B���^�����O�ƃe�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pComparisonLinear;	// ���`�t�B���^�����O���g������r�T���v���[�X�e�[�g

	// �~���[�e�b�N�X
	std::mutex mutex;

	Param() {}
	~Param() {}

	/// <summary>
	/// �u�����h�X�e�[�g���쐬����
	/// </summary>
	/// <param name="srcBlend"></param>
	/// <param name="destBlend"></param>
	/// <param name="ppResult">�󂯎��C���^�[�t�F�C�X</param>
	void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend, ID3D11BlendState** ppResult);
	/// <summary>
	/// �f�v�X�X�e���V���X�e�[�g���쐬����
	/// </summary>
	/// <param name="enable"></param>
	/// <param name="writeEnable"></param>
	/// <param name="ppResult"></param>
	void CreateDepthStencilState(bool enable, bool writeEnable, ID3D11DepthStencilState** ppResult);
	/// <summary>
	/// ���X�^���C�U�X�e�[�g���쐬
	/// </summary>
	/// <param name="cullMode">�J�����O</param>
	/// <param name="fillMode">�h��</param>
	/// <param name="ppResult">�󂯎��C���^�[�t�F�C�X</param>
	/// <param name="scissor">��ʕ������邩�ǂ���</param>
	void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode, ID3D11RasterizerState** ppResult, bool scissor = false);
	/// <summary>
	/// �T���v���[�X�e�[�g���쐬����
	/// </summary>
	/// <param name="filter">�t�B���^�[</param>
	/// <param name="addressMode">�A�h���X���[�h</param>
	/// <param name="ppResult">�󂯎��C���^�[�t�F�C�X</param>
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
			// ���������s
			throw StandardException(
				"�u�����h�X�e�[�g�쐬���s",
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
			// ���������s
			throw StandardException(
				"�f�v�X�X�e���V���X�e�[�g�쐬���s",
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
			// ���������s
			throw StandardException(
				"���X�^���C�U�X�e�[�g�쐬���s",
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
			// ���������s
			throw StandardException(
				"�T���v���[�X�e�[�g�쐬���s",
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
			// ���������s
			throw StandardException(
				"�u�����h�X�e�[�g�쐬�Ɏ��s",
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
			"��r�p�T���v���[�̍쐬���s",
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
// �����_�[�^�[�Q�b�g���N���X
//=========================================
#pragma region RenderTarget
struct RenderTarget::Param
{
	D3D11_VIEWPORT viewPort;	// �r���[�|�[�g
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
// �f�t�H���g�����_�����O�^�[�Q�b�g
//=========================================
#pragma region DefaultRenderTarget
struct DefaultRenderTarget::Param
{
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pD3D11RenderTargetView;	// �����_�����O�^�[�Q�b�g�r���[
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthStencil;			// �[�x�X�e���V���o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthStencilView;		// �[�x�X�e���V���r���[

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

		// �����_�����O�^�[�Q�b�g�r���[�̍쐬
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
		// �o�b�N�o�b�t�@�̃|�C���^�𓾂�
		ThrowIfFailed(
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
			"�X���b�v�`�F�C������o�b�N�o�b�t�@�̎擾�Ɏ��s",
			"pSwapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&pBackBuffer)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);
		// �o�b�N�o�b�t�@���烌���_�����O�^�[�Q�b�g�̃r���[���쐬
		ThrowIfFailed(
			pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_pParam->pD3D11RenderTargetView),
			"DX11�o�b�N�o�b�t�@����̃����_�����O�^�[�Q�b�g�r���[�̍쐬�Ɏ��s",
			"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(),nullptr,&m_pParam->pD3D11RenderTargetView",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		// �[�x�e�N�X�`���̍쐬
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
			"DX11�[�x�e�N�X�`���̍쐬�Ɏ��s",
			"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_pParam->pDepthStencil)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		// �[�x�X�e���V���r���[�̍쐬
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		ThrowIfFailed(
			pD3D11Device->CreateDepthStencilView(m_pParam->pDepthStencil.Get(), &descDSV, &m_pParam->pDepthStencilView),
			"DX11�[�x�X�e���V���r���[�̍쐬�Ɏ��s",
			"pD3D11Device->CreateDepthStencilView(m_pParam->pDepthStencil.Get(), &descDSV, &m_pParam->pDepthStencilView)",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		Microsoft::WRL::ComPtr<IDXGISurface2> dxgiBackBuffer;
		ThrowIfFailed(
			pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
			"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s",
			"pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer))",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		ThrowIfFailed(
			pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				nullptr,
				&m_pParam->pD2DTargetBitmap
			),
			"2d�r�b�g�}�b�v�쐬�Ɏ��s",
			"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
			"DefaultRenderTarget::DefaultRenderTarget()"
		);

		pD2D11DeviceContext->SetTarget(m_pParam->pD2DTargetBitmap.Get());
		// �O���[�X�P�[���e�L�X�g�̃A���`�G�C���A�V���O
		pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

		// �f�t�H���g�r���[�|�[�g�̃Z�b�g
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
	// �o�b�t�@�̃N���A
	float _color[4] = { color.x,color.y,color.z,color.w };
	D3D11_VIEWPORT viewPort;
	// �r���[�|�[�g�̃Z�b�g�A�b�v
	ZeroMemory(&viewPort, sizeof(viewPort));
	viewPort.Width = (float)APP->GetScreenWidth();
	viewPort.Height = (float)APP->GetScreenHeight();
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0.0f;
	viewPort.TopLeftY = 0.0f;
	pD3D11DeviceContext->RSSetViewports(1, &viewPort);
	// �����_�����O�^�[�Q�b�g�̃N���A
	pD3D11DeviceContext->ClearRenderTargetView(m_pParam->pD3D11RenderTargetView.Get(), _color);
	// �ʏ�̐[�x�o�b�t�@�ƃX�e���V���o�b�t�@�̃N���A
	pD3D11DeviceContext->ClearDepthStencilView(m_pParam->pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DefaultRenderTarget::StartRenderTarget()
{
	auto dev = APP->GetDeviceResources();
	//auto pD3D11Device = dev->GetD3DDevice();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();

	ID3D11RenderTargetView* pV = m_pParam->pD3D11RenderTargetView.Get();
	// �����_�����O�^�[�Q�b�g�ƃX�e���V����ݒ�
	pD3D11DeviceContext->OMSetRenderTargets(1, &pV, m_pParam->pDepthStencilView.Get());
	// �r���[�|�[�g�̐ݒ�
	auto viewPort = GetViewPort();
	pD3D11DeviceContext->RSSetViewports(1, &viewPort);

	D3D11_RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = (LONG)viewPort.Width;
	rect.bottom = (LONG)viewPort.Height;
	pD3D11DeviceContext->RSSetScissorRects(1, &rect);

	// �V�F�[�_�[���\�[�X�r���[�̃N���A
	ID3D11ShaderResourceView* pNull[1] = { nullptr };
	pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
	pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
	// �V�F�[�_�[�͎w�肵�Ȃ�
	pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	// �u�����h�͎w�肵�Ȃ�
	pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}

void DefaultRenderTarget::EndRenderTarget()
{
	auto dev = APP->GetDeviceResources();
	auto pD3D11Device = dev->GetD3DDevice();
	auto pD3D11DeviceContext = dev->GetD3DDeviceContext();
	// �V�F�[�_�[���\�[�X�r���[�̃N���A
	ID3D11ShaderResourceView* pNull[1] = { nullptr };
	pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
	pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
	// �V�F�[�_�[�͎w�肵�Ȃ�
	pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	// �u�����h�͎w�肵�Ȃ�
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
				"�t�@�C�������݂��Ȃ�",
				fileName,
				"ShaderResource::ReadBinariFile"
			);
		}

		ThrowIfFailed(
			BinaryReader::ReadEntireFile(fileName, data, &csoSize),
			"�t�@�C���ǂݍ��ݎ��s",
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
		// ���_�V�F�[�_�쐬
		ThrowIfFailed(
			pDX11Device->CreateVertexShader(data.get(), csoSize, nullptr, ppResult),
			"���_�V�F�[�_�쐬���s",
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

		// �C���v�b�g���C�A�E�g�쐬
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
				"���_�V�F�[�_�̃C���v�b�g���C�A�E�g�쐬���s",
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
		
		// �s�N�Z���V�F�[�_�쐬
		HRESULT hr = pDX11Device->CreatePixelShader(
			data.get(),
			csoSize,
			nullptr,
			ppResult
		);
		if (FAILED(hr))
		{
			throw StandardException(
				"�s�N�Z���V�F�[�_�쐬���s",
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

		// �萔�o�b�t�@�쐬
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
				"�萔�o�b�t�@�쐬���s",
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
		// �V�F�[�_�ǂݍ���
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
		// �V�F�[�_�ǂݍ���
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
		// �V�F�[�_�ǂݍ���
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
