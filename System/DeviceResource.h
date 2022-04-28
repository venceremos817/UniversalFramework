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
/// DirectX11�f�o�C�X�N���X
/// </summary>
class DeviceResources
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="hWnd">�E�B���h�E�n���h��</param>
	/// <param name="fullScreen">�t���X�N���[�����ǂ���</param>
	/// <param name="screenWidth">�X�N���[������</param>
	/// <param name="screenHeight">�X�N���[���c��</param>
	DeviceResources(HWND hWnd, bool fullScreen, UINT screenWidth, UINT screenHeight);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DeviceResources();

	/// <summary>
	/// D3D11Device2�C���^�[�t�F�C�X���擾
	/// </summary>
	ID3D11Device2* GetD3DDevice()const;
	/// <summary>
	/// D3D11DeviceContext2�C���^�[�t�F�C�X���擾
	/// </summary>
	ID3D11DeviceContext2* GetD3DDeviceContext()const;
	/// <summary>
	/// DXGISwapChain1�C���^�[�t�F�C�X���擾
	/// </summary>
	IDXGISwapChain1* GetSwapChain()const;
	/// <summary>
	/// D3D_FEATURE_LEVEL���擾
	/// </summary>
	D3D_FEATURE_LEVEL GetFeatureLevel()const;

	/// <summary>
	/// D2D1Factory2�C���^�[�t�F�C�X���擾
	/// </summary>
	ID2D1Factory2* GetD2DFactory()const;
	/// <summary>
	/// D2DDevice1�C���^�[�t�F�C�X���擾
	/// </summary>
	ID2D1Device1* GetD2DDevice()const;
	/// <summary>
	/// D2D1DeviceContex1�C���^�[�t�F�C�X���擾
	/// </summary>
	ID2D1DeviceContext1* GetD2DDeviceContext()const;
	/// <summary>
	/// DWriteFactory2�C���^�[�t�F�C�X���擾
	/// </summary>
	IDWriteFactory2* GetDWriteFactory()const;
	/// <summary>
	/// WICImagingFactory2�C���^�[�t�F�C�X���擾
	/// </summary>
	IWICImagingFactory2* GetWicImagingFactory()const;

	/// <summary>
	/// �f�t�H���g�����_�����O�^�[�Q�b�g���擾
	/// </summary>
	std::shared_ptr<DefaultRenderTarget> GetDefaultRenderTarget();
	/// <summary>
	/// �����_�����O�X�e�[�g���擾
	/// </summary>
	std::shared_ptr<RenderState> GetRenderState()const;
	/// <summary>
	/// �ʏ�`����N���A
	/// </summary>
	/// <param name="color">�N���A�F</param>
	virtual void ClearDefaultView(const Float4& color = Float4(0.2f, 0.2f, 0.2f, 1.0f));
	/// <summary>
	/// �ʏ�`��J�n
	/// </summary>
	virtual void StartDefaultDraw();
	/// <summary>
	/// �ʏ�`��I��
	/// </summary>
	virtual void EndDefaultDraw();
	/// <summary>
	/// �`��㏈��
	/// </summary>
	void InitailizeStates();

	/// <summary>
	/// �t�����g�o�b�t�@�ɓ]��
	/// </summary>
	/// <param name="syncInterval">���������^�C�~���O(0�Ȃ瑦���Ή�/��0�Ȃ琔�l�̎��Ԍo�ߌ�ɑΉ�)</param>
	/// <param name="flags">�o�̓I�v�V����(��{0(DXGI_PRESENT)</param>
	void Present(unsigned int syncInterval, unsigned int flags);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};

/// <summary>
/// �u�����h�X�e�[�g
/// </summary>
enum class BlendState
{
	Opaque,
	AlphaBlend,
	Additive,
	NonPremultiplied,
};
/// <summary>
/// �f�v�X�X�e���V���X�e�[�g
/// </summary>
enum class DepthStencilState
{
	None,
	Default,
	Read,
};
/// <summary>
///  ���X�^���C�U�X�e�[�g
/// </summary>
enum class RasterizerState
{
	CullNone,		// ���ʕ`��
	CullFront,		// ���ʕ`��
	CullBack,		// �O�ʕ`��
	Wireframe,		// ���C���t���[��
	DoubleDraw,		// �w�ʕ`��̌�A�O�ʕ`��
};
/// <summary>
/// �T���v���[�X�e�[�g
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
/// �����_�����O�X�e�[�g
/// </summary>
class RenderState
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	RenderState();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~RenderState();

	/// <summary>
	/// �h��Ԃ��̃u�����h�X�e�[�g���擾
	/// </summary>
	ID3D11BlendState* GetOpaque()const;
	/// <summary>
	/// �P���ȓ����������擾
	/// </summary>
	/// <returns></returns>
	ID3D11BlendState* GetAlphaBlend()const;
	/// <summary>
	/// �������������擾
	/// </summary>
	ID3D11BlendState* GetAlphaBlendEx()const;
	/// <summary>
	/// ���Z�������擾
	/// </summary>
	ID3D11BlendState* GetAdditive()const;
	/// <summary>
	/// �A���t�@���g�p�������̃f�[�^�ƑΏۃf�[�^���u�����h�������擾
	/// </summary>
	ID3D11BlendState* GetNonPremultiplied()const;

	/// <summary>
	/// �[�x���������̃f�v�X�X�e���V���X�e�[�g���擾
	/// </summary>
	ID3D11DepthStencilState* GetDepthNone()const;
	/// <summary>
	/// �ʏ�[�x�̃f�v�X�X�e���V���X�e�[�g���擾
	/// </summary>
	ID3D11DepthStencilState* GetDepthDefault()const;
	/// <summary>
	/// �ǂݎ���p�̃f�v�X�X�e���V���X�e�[�g���擾
	/// </summary>
	ID3D11DepthStencilState* GetDepthRead()const;

	/// <summary>
	/// �J�����O�����̃��X�^���C�U�X�e�[�g���擾
	/// </summary>
	ID3D11RasterizerState* GetCullNone()const;
	/// <summary>
	/// �J�����O�����̃��X�^���C�U�X�e�[�g(�V�[�T�[����)���擾
	/// </summary>
	ID3D11RasterizerState* GetCullNoneScissor()const;
	/// <summary>
	/// �t�����g�J�����O(�w�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g���擾
	/// </summary>
	ID3D11RasterizerState* GetCullFront()const;
	/// <summary>
	/// �t�����g�J�����O(�w�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g(�V�[�T�[����)���擾
	/// </summary>
	ID3D11RasterizerState* GetCullFrontScissor()const;
	/// <summary>
	/// �o�b�N�J�����O(�O�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g���擾
	/// </summary>
	ID3D11RasterizerState* GetCullBack()const;
	/// <summary>
	/// �o�b�N�J�����O(�O�ʕ`��̂�)�̃��X�^���C�U�X�e�[�g(�V�[�T�[����)���擾
	/// </summary>
	ID3D11RasterizerState* GetCullBackScissor()const;
	/// <summary>
	/// ���C���t���[���̃��X�^���C�U�X�e�[�g���擾
	/// </summary>
	ID3D11RasterizerState* GetWireframe()const;
	/// <summary>
	/// ���C���t���[���̃��X�^���C�U�X�e�[�g(�V�[�T�[����)���擾
	/// </summary>
	/// <returns></returns>
	ID3D11RasterizerState* GetWireframeScissor()const;

	/// <summary>
	/// �_�t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetPointWrap()const;
	/// <summary>
	/// �_�t�B���^�����O�ƃe�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetPointClamp()const;
	/// <summary>
	/// ���`�t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetLinearWrap()const;
	/// <summary>
	/// ���`�t�B���^�����O�ƃe�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetLinearClamp()const;
	/// <summary>
	/// �ٕ����t�B���^�����O�ƃe�N�X�`�����W���b�v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetAnisotropicWrap()const;
	/// <summary>
	/// �ٕ����t�B���^�����O�ƃe�N�X�`�����W�N�����v�̃T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetAnisotropicClamp()const;
	/// <summary>
	/// ���`�t�B���^�����O���g������r�T���v���[�X�e�[�g���擾
	/// </summary>
	ID3D11SamplerState* GetComparisonLinear()const;

	/// <summary>
	/// �u�����h�X�e�[�g�̐ݒ�
	/// </summary>
	void SetBlendState(ID3D11DeviceContext2* pContext, BlendState state);
	/// <summary>
	/// �f�v�X�X�e���V���̐ݒ�
	/// </summary>
	void SetDepthStencilState(ID3D11DeviceContext2* pContext, DepthStencilState state);
	/// <summary>
	/// ���X�^���C�U�X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="pContext"></param>
	/// <param name="state"></param>
	void SetRasterizerState(ID3D11DeviceContext2* pContext, RasterizerState state);
	/// <summary>
	/// 1�̃T���v���[�X�e�[�g�̐ݒ�
	/// </summary>
	/// <param name="pContext"></param>
	/// <param name="state"></param>
	/// <param name="slot">�X���b�g�ԍ�</param>
	void SetSamplerState(ID3D11DeviceContext2* pContext, SamplerState state, UINT slot);
	/// <summary>
	/// �T���v���[�X�e�[�g�����ׂăN���A����
	/// </summary>
	void SetSamplerAllClear(ID3D11DeviceContext2* pContext);

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

	// �R�s�[�֎~
	RenderState(const RenderState&) = delete;
	RenderState& operator=(const RenderState&) = delete;
	// ���[�u�֎~
	RenderState(const RenderState&&) = delete;
	RenderState& operator=(const RenderState&&) = delete;
};

/// <summary>
/// �����_�[�^�[�Q�b�g���N���X
/// </summary>
class RenderTarget
{
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	RenderTarget();
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~RenderTarget();

public:
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���N���A
	/// </summary>
	/// <param name="color">�N���A�F</param>
	virtual void ClearViews(const Float4& color) = 0;
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���J�n
	/// </summary>
	virtual void StartRenderTarget() = 0;
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���I��
	/// </summary>
	virtual void EndRenderTarget() = 0;

	/// <summary>
	/// �r���[�|�[�g���擾
	/// </summary>
	const D3D11_VIEWPORT& GetViewPort()const;
	/// <summary>
	/// �r���[�|�[�g��ݒ�
	/// </summary>
	/// <param name="viewPort"></param>
	void SetViewPort(const D3D11_VIEWPORT& viewPort);

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

	// �R�s�[�֎~
	RenderTarget(const RenderTarget&) = delete;
	RenderTarget& operator=(const RenderTarget&) = delete;
	// ���[�u�֎~
	RenderTarget(const RenderTarget&&) = delete;
	RenderTarget& operator=(const RenderTarget&&) = delete;
};


/// <summary>
/// �f�t�H���g�����_�����O�^�[�Q�b�g
/// </summary>
class DefaultRenderTarget
	: public RenderTarget
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	explicit DefaultRenderTarget();
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DefaultRenderTarget();

	/// <summary>
	/// �����_�����O�^�[�Q�b�g���N���A
	/// </summary>
	/// <param name="color">�N���A�F</param>
	virtual void ClearViews(const Float4& color)override;
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���J�n
	/// </summary>
	virtual void StartRenderTarget()override;
	/// <summary>
	/// �����_�����O�^�[�Q�b�g���I��
	/// </summary>
	virtual void EndRenderTarget()override;

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�r���[���擾
	/// </summary>
	ID3D11RenderTargetView* GetRenderTargetView()const;
	/// <summary>
	/// �f�v�X�X�e���V���e�N�X�`�����擾
	/// </summary>
	ID3D11Texture2D* GetDepthStencil()const;
	/// <summary>
	/// �f�v�X�X�e���V���r���[���擾
	/// </summary>
	ID3D11DepthStencilView* GetDepthStencilView()const;
	/// <summary>
	/// 2�����^�[�Q�b�g�̃r�b�g�}�b�v���擾
	/// </summary>
	ID2D1Bitmap1* GetD2DTargetBitmap()const;

private:
	struct Param;
	std::unique_ptr<Param> m_pParam;

};



/// <summary>
/// ���S��COM�C���^�[�t�F�C�X�̃|�C���^���擾����
/// </summary>
/// <param name="pCom">COM�|�C���^</param>
/// <param name="mutex">�~���[�e�b�N�X</param>
/// <param name="createFunc">�쐬�֐�</param>
/// <returns>COM�������̏ꍇ�͐V�����쐬���āA����ȊO��COM���瓾��COM�C���^�[�t�F�C�X�̃|�C���^</returns>
template<class T, class TCreateFunc>
static inline T* DemandCreate(Microsoft::WRL::ComPtr<T>& pCom, std::mutex& mutex, TCreateFunc createFunc)
{
	T* result = pCom.Get();
	// ���b�N��Ԃ��`�F�b�N
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
/// �V�F�[�_�[�֘A���\�[�X�̃C���^�[�t�F�C�X
/// </summary>
class ShaderResource
{
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	ShaderResource();
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~ShaderResource();

public:
	/// <summary>
	/// �o�C�i���t�@�C����ǂݍ���
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="data">�o�C�i���f�[�^�̖߂�</param>
	/// <param name="csoSize">�T�C�Y�̖߂�</param>
	static void ReadBinaryFile(const std::string& fileName, std::unique_ptr<uint8_t[]>& data, size_t& csoSize);

	/// <summary>
	/// �o�C�i���f�[�^���璸�_�V�F�[�_���쐬����
	/// </summary>
	/// <param name="data">�o�C�i���f�[�^</param>
	/// <param name="csoSize">�T�C�Y</param>
	/// <param name="ppResult">�󂯎��V�F�[�_</param>
	static void CreateVertexShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11VertexShader** ppResult);
	/// <summary>
	/// �o�C�i���f�[�^����C���v�b�g���C�A�E�g���쐬����
	/// </summary>
	/// <param name="data">�o�C�i���f�[�^</param>
	/// <param name="csoSize">�T�C�Y</param>
	/// <param name="pElement">���_��`</param>
	/// <param name="elementsNum">���_��`��</param>
	/// <param name="ppResult">�󂯎�郌�C�A�E�g</param>
	static void CreateInputLayout(std::unique_ptr<uint8_t[]>& data, size_t csoSize, const D3D11_INPUT_ELEMENT_DESC* pElement, UINT elementsNum, ID3D11InputLayout** ppResult);
	/// <summary>
	/// �o�C�i���f�[�^����s�N�Z���V�F�[�_���쐬����
	/// </summary>
	/// <param name="data">�o�C�i���f�[�^</param>
	/// <param name="csoSize">�T�C�Y</param>
	/// <param name="ppResult">�󂯎��V�F�[�_</param>
	static void CreatePixelShader(std::unique_ptr<uint8_t[]>& data, size_t csoSize, ID3D11PixelShader** ppResult);

	/// <summary>
	/// �萔�o�b�t�@���쐬����
	/// </summary>
	/// <param name="buffSize">�T�C�Y</param>
	/// <param name="ppResult">�󂯎��萔�o�b�t�@</param>
	static void CreateConstantBuffer(UINT buffSize, ID3D11Buffer** ppResult);

protected:
	/// <summary>
	/// ���_�V�F�[�_�A�N�Z�T
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="pVS">���؂���V�F�[�_COM</param>
	ID3D11VertexShader* GetVertexShaderBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11VertexShader>& pVS);
	/// <summary>
	/// �s�N�Z���V�F�[�_�A�N�Z�T
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="pPS">���؂���V�F�[�_COM</param>
	/// <returns></returns>
	ID3D11PixelShader* GetPixelShaderBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pPS);
	/// <summary>
	/// �C���v�b�g���C�A�E�g�A�N�Z�T
	/// </summary>
	/// <param name="fileName">�t�@�C����</param>
	/// <param name="pIL">���؂���C���v�b�g���C�A�E�gCOM</param>
	/// <param name="pElem">���_��`</param>
	/// <param name="elementsNum">���_��`��</param>
	/// <returns></returns>
	ID3D11InputLayout* GetInputLayoutBase(const std::string& fileName, Microsoft::WRL::ComPtr<ID3D11InputLayout>& pIL, const D3D11_INPUT_ELEMENT_DESC* pElem, UINT elementsNum);
	/// <summary>
	/// �萔�o�b�t�@�A�N�Z�T
	/// </summary>
	/// <param name="buffSize">�T�C�Y</param>
	/// <param name="buffer">���؂���o�b�t�@COM</param>
	/// <returns></returns>
	ID3D11Buffer* GetConstantBufferBase(UINT buffSize, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer);

public:
	static const int m_lightMax{ 3 };	// ���C�g�ő吔

protected:
	std::mutex mutexBase;	// �~���[�e�b�N�X

private:
	//struct Param;
	//std::unique_ptr<Param> m_param;

	// �R�s�[�֎~
	ShaderResource(const ShaderResource&) = delete;
	ShaderResource& operator=(const ShaderResource&) = delete;
	// ���[�u�֎~
	ShaderResource(const ShaderResource&&) = delete;
	ShaderResource&& operator=(const ShaderResource&&) = delete;
};

/// <summary>
/// ���_�V�F�[�_�[(�V���O���g��)
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
	/// ���_�V�F�[�_���擾
	/// </summary>
	ID3D11VertexShader* GetShader() { return GetVertexShaderBase(m_fileName, m_pVS); }
	/// <summary>
	/// �C���v�b�g���C�A�E�g���擾
	/// </summary>
	ID3D11InputLayout* GetInputLayout() { return GetInputLayoutBase(m_fileName, m_pIL, VertexType::GetVertexElement(), VertexType::GetElementsNum()); }

	/// <summary>
	/// �C���X�^���X���擾
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
	// �f���[�^�[
	struct Deleter
	{
		void operator()(ShaderType* p) { delete p; }
	};

	PixelShader<ShaderType>(const std::string& fileName) : ShaderResource(), m_fileName(fileName) {}
	virtual ~PixelShader() {}

public:
	/// <summary>
	/// �s�N�Z���V�F�[�_�[���擾
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
/// �萔�o�b�t�@(�V���O���g��)
/// </summary>
template<class ShaderType, class ConstantType>
class ConstantBuffer : public ShaderResource
{
protected:
	// �f���[�^
	struct Deleter
	{
		void operator()(ShaderType* p) { delete p; }
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	std::mutex m_mutex;		// �~���[�e�b�N�X
	// �\�z�Ɣj��
	ConstantBuffer<ShaderType, ConstantType>() : ShaderResource() {}
	virtual ~ConstantBuffer() {}
	static std::unique_ptr<ShaderType, Deleter> m_pInstance;


public:
	/// <summary>
	/// �萔�o�b�t�@���擾
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
	// �R�s�[�֎~
	ConstantBuffer<ShaderType, ConstantType>(ConstantBuffer const&) = delete;
	ConstantBuffer<ShaderType, ConstantType>& operator=(ConstantBuffer<ShaderType, ConstantType> const&) = delete;
	// ���[�u�֎~
	ConstantBuffer(const ConstantBuffer&&) = delete;
	ConstantBuffer& operator=(const ConstantBuffer&&) = delete;
};



