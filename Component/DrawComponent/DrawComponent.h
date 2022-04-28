#pragma once
#include <Component.h>
#include <App.h>
#include <System/Resource/Shader.h>



class MeshResource;

/// <summary>
/// �`��R���|�[�l���g�C���^�[�t�F�C�X�N���X
/// </summary>
class DrawComponentBase
	: public Component
{
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	explicit DrawComponentBase(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~DrawComponentBase();

public:

	virtual void Draw() = 0;

	/// <summary>
	/// �u�����h�X�e�[�g���擾
	/// </summary>
	virtual BlendState GetBlendState()const;
	/// <summary>
	/// �f�v�X�X�e���V���X�e�[�g���擾
	/// </summary>
	virtual DepthStencilState GetDepthStencilState()const;

	/// <summary>
	/// ���X�^���C�U�X�e�[�g���擾
	/// </summary>
	virtual RasterizerState GetRasterizerState()const;
	/// <summary>
	/// �T���v���[�X�e�[�g���擾
	/// </summary>
	virtual SamplerState GetSamplerState()const;

	/// <summary>
	/// �u�����h�X�e�[�g��ݒ�
	/// </summary>
	/// <param name="state"></param>
	virtual void SetBlendState(const BlendState& blendState);
	/// <summary>
	/// �f�v�X�X�e���V���X�e�[�g��ݒ�
	/// </summary>
	virtual void SetDepthStencilState(const DepthStencilState& depthStencilState);
	/// <summary>
	/// ���X�^���C�U�X�e�[�g��ݒ�
	/// </summary>
	/// <param name="rasterizerState"></param>
	virtual void SetRasterizerState(const RasterizerState& rasterizerState);
	/// <summary>
	/// �T���v���[�X�e�[�g��ݒ�
	/// </summary>
	/// <param name="samplerState"></param>
	virtual void SetSamplerState(const SamplerState& samplerState);

	/// <summary>
	/// �u�����h�X�e�[�g���f�o�C�X�ɐݒ�
	/// </summary>
	virtual void BindDeviceBlendState();
	/// <summary>
	/// �f�v�X�X�e���V���X�e�[�g���f�o�C�X�ɐݒ�
	/// </summary>
	virtual void BindDeviceDepthStencilState();
	/// <summary>
	/// ���X�^���C�U�X�e�[�g���f�o�C�X�ɐݒ�
	/// </summary>
	virtual void BindDeviceRasterizerState();
	/// <summary>
	/// �T���v���[�X�e�[�g���f�o�C�X�ɐݒ�
	/// </summary>
	virtual void BindDeviceSamplerState();

	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮����s����擾
	/// </summary>
	virtual const Float4x4& GetMeshOffsetTransform()const;
	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮����s���ݒ�
	/// </summary>
	virtual void SetMeshOffsetTransform(const Float4x4& offsetTransform);

	/// <summary>
	/// ���b�V�����\�[�X���擾
	/// </summary>
	virtual std::shared_ptr<MeshResource> GetMeshResource()const;
	/// <summary>
	/// ���b�V�����\�[�X��ݒ�
	/// </summary>
	virtual void SetMeshResource(const std::shared_ptr<MeshResource>& meshRes);

	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�
	/// </summary>
	virtual void SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, UINT slot);
	virtual void SetTextureResource(const std::string textureKey, UINT slot);
	/// <summary>
	/// �e�N�X�`�����\�[�X���擾
	/// </summary>
	virtual std::shared_ptr<TextureResouce> GetTextureResource(UINT slot)const;

	/// <summary>
	/// �G�~�b�V�u���擾
	/// </summary>
	virtual Float4 GetEmissive()const;
	/// <summary>
	/// �G�~�b�V�u��ݒ�
	/// </summary>
	virtual void SetEmissive(const Float4& emissive);

	/// <summary>
	/// �f�B�t���[�Y���擾
	/// </summary>
	virtual Float4 GetDiffuse()const;
	/// <summary>
	/// �f�B�t���[�Y��ݒ�
	/// </summary>
	/// <param name="diffuse"></param>
	virtual void SetDiffuse(const Float4& diffuse);

	virtual Float4 GetAmbientLightColor()const;
	virtual void SetAmbientLightColor(const Float4& ambientLightColor);

	/// <summary>
	/// �X�y�L�����[���擾
	/// </summary>
	virtual Float4 GetSpecular()const;
	/// <summary>
	/// �X�y�L�����[��ݒ�
	/// </summary>
	virtual void SetSpecular(const Float4& specular);
	/// <summary>
	/// �X�y�L�����[�p���[���擾
	/// </summary>
	virtual float GetSpecularPower()const;
	/// <summary>
	/// �X�y�L�����[�p���[��ݒ�
	/// </summary>
	virtual void SetSpecularPower(float power);
	/// <summary>
	/// �X�y�L�����[���Ȃ���
	/// </summary>
	virtual void DisableSpecular();

	/// <summary>
	/// �A���t�@�l���擾
	/// </summary>
	virtual float GetAlpha()const;
	/// <summary>
	/// �A���t�@�l��ݒ�
	/// </summary>
	virtual void SetAlpha(float a);

	/// <summary>
	/// �J���[�ƃA���t�@���擾
	/// </summary>
	/// <returns>Float4(diffuse.xyz,alpha)</returns>
	virtual Float4 GetColorAndAlpha()const;
	/// <summary>
	/// �J���[�ƃA���t�@�l��ݒ�
	/// </summary>
	virtual void SetColorAlpha(const Float4& color);

	/// <summary>
	/// �V�F�[�_�[��ݒ�
	/// </summary>
	template<class T_VShader, class T_PShader>
	void SetShader()
	{
		SetVertexShader(T_VShader::GetInstance()->GetShader(),
			T_VShader::GetInstance()->GetInputLayout());
		SetPixelShader(T_PShader::GetInstance()->GetShader());
	}

	/// <summary>
	/// ���_�V�F�[�_��ݒ�
	/// </summary>
	void SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pIL);
	/// <summary>
	/// ���_�V�F�[�_���擾
	/// </summary>
	/// <returns></returns>
	ID3D11VertexShader* GetVertexShader();
	/// <summary>
	/// �C���v�b�g���C�A�E�g���擾
	/// </summary>
	/// <returns></returns>
	ID3D11InputLayout* GetInputLayout();
	/// <summary>
	/// �s�N�Z���V�F�[�_��ݒ�
	/// </summary>
	void SetPixelShader(ID3D11PixelShader* pPS);
	ID3D11PixelShader* GetPixelShader();

	/// <summary>
	/// UV�X�P�[����ݒ�
	/// </summary>
	/// <param name="scale"></param>
	void SetUVScale(const Float2& scale);
	/// <summary>
	/// UV�X�P�[�����擾
	/// </summary>
	/// <returns></returns>
	const Float2& GetUVScale();

	/// <summary>
	/// ���C�e�B���O��L���ɂ��邩�ݒ�
	/// </summary>
	/// <param name="b"></param>
	void SetLightingEnable(bool b);
	/// <summary>
	/// ���C�e�B���O���L�����ǂ����擾
	/// </summary>
	/// <returns></returns>
	bool IsLightingEnable();

	/// <summary>
	/// �e��`�悷�邩�ǂ�����ݒ�
	/// </summary>
	/// <param name="b"></param>
	void SetOwnShadowActive(bool b);
	/// <summary>
	/// �e��`�悷�邩�ǂ������擾
	/// </summary>
	/// <returns></returns>
	bool IsOwnShadowActive();

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};


/// <summary>
/// �}�e���A��
/// </summary>
struct Material
{
	UINT m_startIndex;	// �J�n�C���f�b�N�X
	UINT m_indexCount;	// �C���f�b�N�X�J�E���g
	Float4 m_diffuse;	// �f�B�t���[�Y
	Float4 m_specular;	// �X�y�L�����[
	Float4 m_ambient;	// �A���r�G���g
	Float4 m_emissive;	// �G�~�b�V�u
	std::shared_ptr<TextureResouce> m_textureResource;	// �e�N�X�`�����\�[�X
};

/// <summary>
/// ���_���Ƃ̃X�L�j���O���
/// </summary>
struct SkinPrimData
{
	uint32_t indexes[4];
	float weights[4];
};

/// <summary>
/// ���b�V���f�[�^
/// </summary>
struct MeshPrimData
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuff;	// ���_�o�b�t�@
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuff;	// �C���f�b�N�X�o�b�t�@
	UINT m_verticesNum;	// ���_��
	UINT m_indexesNum;	// �C���f�b�N�X��
	std::type_index m_meshTypeIndex;	// ���b�V���̌`�̃C���f�b�N�X
	UINT m_strideNum;	// �X�g���C�h��
	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology;	// �`��g�|���W�[
	std::shared_ptr<BackupDataBase> m_backupData;	// �o�b�N�A�b�v�f�[�^
	Float4x4 m_meshOffsetTransform;			// ���b�V���ƃI�u�W�F�N�g�̍���⊮����s��(���b�V���P�ʂŐݒ肷��ꍇ���p)
	bool m_useMeshOffsetTransform;			// ���b�V���ƃI�u�W�F�N�g�̍���ۊǂ���s��𗘗p���邩
	std::weak_ptr<TextureResouce> m_textureResource;	// �e�N�X�`�����\�[�X(���b�V���P�ʂŐݒ肷��ꍇ���p)
	std::vector<Material> m_materialVec;	// �}�e���A���z��(���f���Ŏg�p)

	bool m_isSkining;		// �{�[�����ǂ���
	UINT m_boneNum;			// �{�[���̐�
	UINT m_sampleNum;		// �T���v�����O��
	std::vector<Float4x4> m_sampleMatrixVec;	// �T���v�����O���ꂽ�{�[���s��
	UINT m_multiMeshIndex;	// �}���`���b�V���̏ꍇ�̃��b�V���C���f�b�N�X
	std::vector<VertexPosition> m_vertices;	// �ėp�Ɏg���郍�[�J�����_�z��
	std::vector<SkinPrimData> m_skins;	// �X�L�j���O���

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
/// ���b�V�����\�[�X
/// </summary>
class MeshResource
	: public BaseResource
{
	friend class ObjectFactory;
public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~MeshResource();

	/// <summary>
	/// ���b�V���f�[�^���擾
	/// </summary>
	/// <returns></returns>
	const MeshPrimData& GetMeshData()const;
	/// <summary>
	/// ���b�V���f�[�^��ݒ�
	/// </summary>
	/// <param name="meshData"></param>
	void SetMeshData(const MeshPrimData& meshData);

	/// <summary>
	/// ���_�o�b�t�@���擾
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer()const;
	/// <summary>
	/// ���_�����擾
	/// </summary>	
	UINT GetVerticesNum()const;
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���擾
	/// </summary>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer()const;
	/// <summary>
	/// �C���f�b�N�X�����擾
	/// </summary>	
	UINT GetindexesNum()const;
	/// <summary>
	/// �X�g���C�h�����擾
	/// </summary>	
	UINT GetStridesNum()const;

	/// <summary>
	/// �`��g�|���W�[���擾
	/// </summary>
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology()const;
	/// <summary>
	/// �`��g�|���W�[��ݒ�
	/// </summary>
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
	/// <summary>
	/// type_index���擾
	/// </summary>
	std::type_index GetTypeIndex()const;
	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮����s����擾
	/// </summary>
	const Float4x4& GetMeshOffsetTransform()const;
	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮����s���ݒ�
	/// </summary>
	void SetMeshOffsetTransform(const Float4x4& offsetTransform);
	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮���邩
	/// </summary>
	bool IsUseMeshOffsetTransform();
	/// <summary>
	/// ���b�V���ƃQ�[���I�u�W�F�N�g�̍���⊮���邩�ǂ����ݒ�
	/// </summary>
	void SetUseMeshOffsetTransform(bool useOffsetTransform);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�(���b�V�����̏ꍇ)
	/// </summary>
	/// <param name="textureResource"></param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�(���b�V�����̏ꍇ)
	/// </summary>
	/// <param name="textureKey"></param>
	void SetTextureResource(const std::string& textureKey);
	/// <summary>
	/// �e�N�X�`�����\�[�X���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> GetTextureResource()const;
	/// <summary>
	/// �}�e���A���z����擾
	/// </summary>
	/// <returns></returns>
	const std::vector<Material>& GetMaterialVec()const;


	/// <summary>
	/// �o�b�t�@���쐬����
	/// </summary>
	/// <param name="pDX11Device">�f�o�C�X</param>
	/// <param name="pBufferDesc">�o�b�t�@�f�X�N</param>
	/// <param name="pDataDesc">�f�[�^�f�X�N</param>
	/// <param name="ppBuffer">���_�o�b�t�@</param>
	static void CreatePrimitiveBuffer(ID3D11Device* pDX11Device, const D3D11_BUFFER_DESC* pBufferDesc, const D3D11_SUBRESOURCE_DATA* pDataDesc, ID3D11Buffer** ppBuffer);
	/// <summary>
	/// �o�b�t�@���쐬
	/// </summary>
	/// <param name="pDX11Device">�f�o�C�X</param>
	/// <param name="data">�f�[�^</param>
	/// <param name="bindFlag">�o�C���h�t���O</param>
	/// <param name="ppBuffer">�o�b�t�@</param>
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
				"�o�b�t�@�쐬���s",
				"if(FAILED(pDX11Device->CreateBuffer()))",
				"MeshResource::CreatePrimitiveBuffer()"
			);
		}
	}

	/// <summary>
	/// ���_�o�b�t�@���쐬
	/// </summary>
	/// <param name="vertexBuffer">�Ԃ����o�b�t�@</param>
	/// <param name="data">�f�[�^</param>
	template<class T>
	static void CreateVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(vertexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// ���_�o�b�t�@�쐬
			CreatePrimitiveBuffer(pDX11Device, data, D3D11_BIND_VERTEX_BUFFER, ppResult);
		});
	}

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���쐬
	/// </summary>
	/// <param name="indexBuffer">�Ԃ����o�b�t�@</param>
	/// <param name="data">�f�[�^</param>
	template<class T>
	static void CreateIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(indexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// �C���f�b�N�X�o�b�t�@�쐬
			CreatePrimitiveBuffer(pDX11Device, data, D3D11_BIND_INDEX_BUFFER, ppResult);
		});
	}

	/// <summary>
	/// ���_�f�[�^�������������钸�_�o�b�t�@���쐬
	/// </summary>
	/// <param name="pDX11Device">�f�o�C�X</param>
	/// <param name="data">�f�[�^</param>
	/// <param name="ppBuffer">�Ԃ����o�b�t�@</param>
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
				"�o�b�t�@�쐬���s",
				"if(FAILED(pDX11Device->CreateBuffer()))",
				"MeshResource::CreateDynamicPrimitiveVertexBuffer()"
			);
		}
	}

	/// <summary>
	/// ���_�f�[�^�������������钸�_�o�b�t�@���쐬
	/// </summary>
	/// <param name="vertexBuffer">�Ԃ����o�b�t�@</param>
	/// <param name="data">�f�[�^</param>
	template<class T>
	static void CreateDynamicVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, const T& data)
	{
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		std::mutex mutex;
		DemandCreate(vertexBuffer, mutex, [&](ID3D11Buffer** ppResult)
		{
			// ���_�o�b�t�@���쐬
			CreateDynamicPrimitiveVertexBuffer(pDX11Device, data, ppResult);
		});
	}

	/// <summary>
	/// ��`���_���쐬
	/// </summary>
	/// <param name="size"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateSquareVertices(float size,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �����̒��_���쐬
	/// </summary>
	/// <param name="size"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCubeVertices(float size,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �����_���쐬
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateSphereVertices(float diameter, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �J�v�Z�����_���쐬
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCapsuleVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �V�����_�[���_���쐬
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateCylinderVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �R�[�����_���쐬
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="height"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateConeVertices(float diameter, float height, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);
	/// <summary>
	/// �g�[���X���_���쐬
	/// </summary>
	/// <param name="diameter"></param>
	/// <param name="thickness"></param>
	/// <param name="tessellation"></param>
	/// <param name="vertices"></param>
	/// <param name="indexes"></param>
	static void CreateTorusVertices(float diameter, float thickness, size_t tessellation,
		std::vector<VertexPositionNormalTexture>& vertices, std::vector<uint16_t>& indexes);


	/// <summary>
	/// ��`���b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="size">1�ӂ̒���</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateSquareMeshResource(float size, bool accessWrite = false);
	/// <summary>
	/// �����̃��b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="size">1�ӂ̒���</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCubeMeshResource(float size, bool accessWrite = false);
	/// <summary>
	/// �����b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="diameter">���a</param>
	/// <param name="tessellation">������</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateSphereMeshResource(float diameter, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// �J�v�Z�����b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="diameter">���a</param>
	/// <param name="height">����</param>
	/// <param name="tessellation">������</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCapsuleMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// �V�����_�[���b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="diameter">���a</param>
	/// <param name="height">����</param>
	/// <param name="tessellation">������</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateCylinderMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// �R�[�����b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="diameter">���a</param>
	/// <param name="height">����</param>
	/// <param name="tessellation">������</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateConeMeshResource(float diameter, float height, size_t tessellation, bool accessWrite = false);
	/// <summary>
	/// �g�[���X���b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="diameter">���a</param>
	/// <param name="thickness">����</param>
	/// <param name="tessellation">������</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns></returns>
	static std::shared_ptr<MeshResource> CreateTorusMeshResource(float diameter, float thickness, size_t tessellation, bool accessWrite = false);


	/// <summary>
	/// ���b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="veritices">���_�z��</param>
	/// <param name="accessWrite">���������\���ǂ���</param>
	/// <returns></returns>
	template<class T>
	static std::shared_ptr<MeshResource> CreateMeshResource(const std::vector<T>& vertices, bool accessWrite = false)
	{
		std::mutex mutex;
		auto pDX11Device = APP->GetDeviceResources()->GetD3DDevice();
		auto pMeshResource = ObjectFactory::Create<MeshResource>();
		// �ėp���_���쐬
		for (auto& v : vertices)
		{
			VertexPosition vertex;
			vertex.position = v.position;
			pMeshResource->m_meshPrimData.m_vertices.push_back(vertex);
		}
		// �o�b�t�@�쐬
		if (accessWrite)
		{
			DemandCreate(pMeshResource->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				auto pBackup = std::shared_ptr<BackupData<T>>(new BackupData<T>());
				for (auto& v : vertices)
					pBackup->m_vertices.push_back(v);
				pMeshResource->m_meshPrimData.m_backupData = pBackup;
				// ���_�o�b�t�@�쐬
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
		// ���_���ݒ�
		pMeshResource->m_meshPrimData.m_verticesNum = static_cast<UINT>(vertices.size());
		pMeshResource->m_meshPrimData.m_meshTypeIndex = typeid(T);
		pMeshResource->m_strideNum = sizeof(T);

		return pMeshResource;
	}

	/// <summary>
	/// ���b�V�����\�[�X���쐬
	/// </summary>
	/// <param name="vertices">���_�z��</param>
	/// <param name="indexes">�C���f�b�N�X�z��</param>
	/// <param name="accessWrite">���������\���ǂ���</param>
	/// <returns></returns>
	template<class T>
	static std::shared_ptr<MeshResource> CreateMeshResource(const std::vector<T>& vertices, const std::vector<uint16_t>& indexes, bool accessWrite = false)
	{
		std::mutex mutex;
		auto dev = APP->GetDeviceResources();
		auto pDX11Device = dev->GetD3DDevice();
		auto pID3D11DeviceContext = dev->GetD3DDeviceContext();
		auto pMeshResouce = ObjectFactory::Create<MeshResource>();
		// �ėp���_�쐬
		// �C���f�b�N�X�����Ƃɍ쐬
		for (auto i : indexes)
		{
			VertexPosition vertex;
			vertex.position = vertices[i].position;
			pMeshResouce->m_meshPrimData.m_vertices.push_back(vertex);
			// �X�L����������
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
		// �o�b�t�@�쐬
		if (accessWrite)
		{
			DemandCreate(pMeshResouce->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				auto pBackup = std::shared_ptr<BackupData<T>>(new BackupData<T>());
				for (auto& v : vertices)
					pBackup->m_vertices.push_back(v);
				pMeshResouce->m_meshPrimData.m_backupData = pBackup;
				// ���_�o�b�t�@�쐬
				CreateDynamicPrimitiveVertexBuffer(pDX11Device, vertices, ppResult);
			});
		}
		else
		{
			DemandCreate(pMeshResouce->m_meshPrimData.m_vertexBuff, mutex, [&](ID3D11Buffer** ppResult)
			{
				// ���_�o�b�t�@�쐬
				CreatePrimitiveBuffer(pDX11Device, vertices, D3D11_BIND_VERTEX_BUFFER, ppResult);
			});
		}
		// ���_���ݒ�
		pMeshResouce->m_meshPrimData.m_verticesNum = static_cast<UINT>(vertices.size());
		// �C���f�b�N�X�쐬
		DemandCreate(pMeshResouce->m_meshPrimData.m_indexBuff, mutex, [&](ID3D11Buffer** ppResult)
		{
			if (pMeshResouce->m_meshPrimData.m_backupData)
			{
				auto pBackup = std::dynamic_pointer_cast<BackupData<T>>(pMeshResouce->m_meshPrimData.m_backupData);
				for (auto& v : indexes)
					pBackup->m_indexes.push_back(v);
			}
			// �C���f�b�N�X�o�b�t�@�쐬
			CreatePrimitiveBuffer(pDX11Device, indexes, D3D11_BIND_INDEX_BUFFER, ppResult);
		});
		// �C���f�b�N�X���ݒ�
		pMeshResouce->m_meshPrimData.m_indexesNum = static_cast<UINT>(indexes.size());
		pMeshResouce->m_meshPrimData.m_meshTypeIndex = typeid(T);
		pMeshResouce->m_meshPrimData.m_strideNum = sizeof(T);
		return pMeshResouce;
	}

	/// <summary>
	/// VertexPositionNormalTexture���_�����Ƃ�
	/// ���ꂼ��̒��_���ɕϊ�
	/// </summary>
	/// <param name="originVertices">���ƂȂ钸�_</param>
	/// <param name="pcVertices">���W�A�F�������_</param>
	/// <param name="ptVertices">���W�AUV�������_</param>
	/// <param name="pnttnVertices">���W�A�@���AUV�A�ڃx�N�g���������_</param>
	static void ConvertVertex(const std::vector<VertexPositionNormalTexture>& originVertices,
		std::vector<VertexPositionColor>& pcVertices,
		std::vector<VertexPositionTexture>& ptVertices,
		std::vector<VertexPositionNormalTextureTangent>& pnttgVertices);

protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	MeshResource();

	/// <summary>
	/// ���_�o�b�t�@��ݒ�
	/// </summary>
	/// <param name="vertexBuffer"></param>
	void SetVertexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer);
	/// <summary>
	/// ���_����ݒ�
	/// </summary>
	/// <param name="verticesNum"></param>
	void SetVerticiesNum(UINT verticesNum);
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@��ݒ�
	/// </summary>
	/// <param name="indexBuffer"></param>
	void SetIndexBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer);
	/// <summary>
	/// �C���f�b�N�X����ݒ�
	/// </summary>
	/// <param name="indexesNum"></param>
	void SetindexesNum(UINT indexesNum);

	/// <summary>
	/// ���_�̌^�ƃX�g���C�h��ݒ�
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
/// �}���`���b�V�����\�[�X
/// </summary>
class MultiMeshResource
	: public BaseResource
{
	friend class ObjectFactory;
protected:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	MultiMeshResource();

public:
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~MultiMeshResource();

	/// <summary>
	/// ���b�V���f�[�^�����擾
	/// </summary>
	size_t GetMeshVecNum()const;
	/// <summary>
	/// ���b�V���f�[�^�z����擾
	/// </summary>
	const std::vector<MeshPrimData>& GetMeshVec()const;

	/// <summary>
	/// ���b�V���f�[�^��ǉ�
	/// </summary>
	/// <param name="data">���b�V���f�[�^</param>
	/// <returns>�ǉ����ꂽ�C���f�b�N�X</returns>
	size_t AddMesh(const MeshPrimData& data);
	/// <summary>
	/// ���b�V���f�[�^��ǉ�
	/// </summary>
	/// <param name="meshRes">���b�V�����\�[�X</param>
	/// <returns>�ǉ����ꂽ�C���f�b�N�X</returns>
	size_t AddMesh(const std::shared_ptr<MeshResource>& meshRes);
	/// <summary>
	/// ���b�V���f�[�^��ǉ�
	/// </summary>
	/// <param name="meshKey">���b�V���L�[</param>
	/// <returns>�ǉ����ꂽ�C���f�b�N�X</returns>
	size_t AddMesh(const std::string& meshKey);
	/// <summary>
	/// ���b�V�����\�[�X��ǉ�
	/// </summary>
	/// <param name="vertices">���_�z��</param>
	/// <param name="AccessWrite">���������\���ǂ���</param>
	/// <returns>�ǉ����ꂽ�C���f�b�N�X</returns>
	template<class T>	// ���_�̌^
	size_t AddMesh(const std::vector<T>& vertices, bool accessWrite = false)
	{
		auto meshRes = MeshResource::CreateMeshResource<T>(vertices, accessWrite);
		return AddMesh(meshRes);
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="vertices">���_�z��</param>
	/// <param name="indexes">�C���f�b�N�X�z��</param>
	/// <param name="accessWrite">���������\���ǂ���</param>
	/// <returns></returns>
	template<class T>	// ���_�̌^
	size_t AddMesh(const std::vector<T>& vertices, const std::vector<uint16_t>& indexes, bool accessWrite = false)
	{
		auto pMeshRes = MeshResource::CreateMeshResource<T>(vertices, indexes, accessWrite);
		return AddMesh(pMeshRes);
	}
	/// <summary>
	/// ���b�V���f�[�^���폜����
	/// </summary>
	/// <param name="index">�폜���郁�b�V���f�[�^�z��̃C���f�b�N�X</param>
	void RemoveMesh(size_t index);

	/// <summary>
	/// ���b�V���f�[�^���擾
	/// </summary>
	/// <param name="outData">�擾����f�[�^�Q��</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void GetMeshData(MeshPrimData& outData, size_t index)const;
	/// <summary>
	/// ���_�o�b�t�@���擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer(size_t index)const;
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer(size_t index)const;
	/// <summary>
	/// ���_�����擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	UINT GetVerticesNum(size_t index)const;
	/// <summary>
	/// �C���f�b�N�X�����擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	UINT GetIndexesNum(size_t index)const;
	/// <summary>
	/// �X�g���C�h�����擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	UINT GetStrideNum(size_t index)const;
	/// <summary>
	/// �`��g�|���W�[���擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology(size_t index)const;
	/// <summary>
	/// �`��g�|���W�[��ݒ�
	/// </summary>
	/// <param name="topology">�`��g�|���W�[</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology, size_t index);
	/// <summary>
	/// type_index���擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	/// <returns></returns>
	std::type_index GetTypeIndex(size_t index)const;
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�ƃ��b�V���̂����⊮����s����擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	const Float4x4& GetMeshOffsetTransform(size_t index)const;
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�ƃ��b�V���̂����⊮����s���ݒ�
	/// </summary>
	/// <param name="offsetTransform">����s��</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void SetMeshOffsetTransform(const Float4x4& offsetTransform, size_t index);
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�Ƃ̂����⊮����s����g�p���邩�ǂ���
	/// </summary>
	/// <param name="index"></param>
	bool IsUseMeshOffsetTransform(size_t index)const;
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�Ƃ̂����⊮����s����g�p���邩�ǂ����ݒ肷��
	/// </summary>
	/// <param name="b"></param>
	/// <param name="index"></param>
	void SetIsUseMeshOffsetTransform(bool b, size_t index);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�(���b�V�����ɈقȂ�ꍇ�g�p
	/// </summary>
	/// <param name="textureRes">�e�N�X�`�����\�[�X</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureRes, size_t index);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�(���b�V�����ɈقȂ�ꍇ�g�p
	/// </summary>
	/// <param name="textureKey">���b�V�����\�[�X�L�[</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void SetTextureResource(const std::string& textureKey, size_t index);
	/// <summary>
	/// �e�N�X�`�����\�[�X���擾(���b�V�����ɈقȂ�ꍇ�g�p
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	std::shared_ptr<TextureResouce> GetTextureResource(size_t index)const;
	/// <summary>
	/// �}�e���A���z����擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z����擾</param>
	const std::vector<Material>& GetMaterialVec(size_t index)const;

	/// <summary>
	/// �X�L�j���O���邩�ǂ���
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	virtual bool IsSkining(size_t index)const;
	/// <summary>
	/// �{�[�������擾
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	UINT GetBoneNum(size_t index)const;
	/// <summary>
	/// �T���v�����O�����擾
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	UINT GetSampleNum(size_t index)const;
	/// <summary>
	/// �T���v�����O���ꂽ�s��̔z����擾
	/// </summary>
	/// <param name="index"></param>
	/// <returns></returns>
	const std::vector<Float4x4>& GetSampleMatrixVec(size_t index)const;

	/// <summary>
	/// �o�b�N�A�b�v�f�[�^���擾
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	template<class T>
	std::vector<T>& GetBackupVertices(size_t index)const
	{
		auto pBackup = std::dynamic_pointer_cast<BackupData<T>>(m_meshPrimDataVec[index].m_backupData);
		return pBackup->m_vertices;
	}

	/// <summary>
	/// ���_��ύX
	/// </summary>
	/// <param name="newBuffer">���_�z��</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	template<class T>
	void UpdateVertices(const std::vector<T>& newBuffer, size_t index)
	{
		auto pBakcup = std::dynamic_pointer_cast<BackupData<T>>(m_meshPrimDataVec[index].m_backupData);
		auto dev = APP->GetDeviceResources();
		auto pID3D11DeviceContext = dev->GetD2DDeviceContext();
		// ���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = GetVertexBuffer(index).Get();

		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;	// GPU�Ɏז�����Ȃ�
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		// ���_��ύX
		T* vertices = (T*)mappedBuffer.pData;
		for (size_t i = 0; i < newBuffer.size(); ++i)
		{
			vertices[i] = newBuffer[i];
		}
		// �A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);
	}



public:
	/// <summary>
	/// �}���`���b�V�����\�[�X���쐬
	/// </summary>
	/// <returns></returns>
	static std::shared_ptr<MultiMeshResource> CreateMultiMeshResource();
	/// <summary>
	/// �I���W�i���}���`���b�V�����쐬(�X�^�e�B�b�N���b�V��
	/// </summary>
	/// <param name="binDataDir">��f�B���N�g��</param>
	/// <param name="binDataFile">�t�@�C����</param>
	/// <param name="accessWrite">���_��ύX�ł��邩�ǂ���</param>
	/// <returns>�쐬�������\�[�X</returns>
	static std::shared_ptr<MultiMeshResource> CreateStaticModelMultiMesh(const std::string& binDataDir, const std::string& binDataFile, bool accessWrite = false);
	/// <summary>
	/// �I���W�i���}���`���b�V�����쐬(�{�[�����b�V��
	/// </summary>
	/// <param name="binDataDir"></param>
	/// <param name="binDataFile"></param>
	/// <param name="accessWrite"></param>
	/// <returns></returns>
	static std::shared_ptr<MultiMeshResource> CreateBoneModelMultiMesh(const std::string& binDataDir, const std::string& binDataFile, bool accessWrite = false);




protected:
	/// <summary>
	/// ���_�o�b�t�@��ݒ�
	/// </summary>
	/// <param name="vertexBuffer">���_�o�b�t�@</param>
	/// <param name="index">���b�V���f�[�^�z��̃C���f�b�N�X</param>
	void SetVertexBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& vertexBuffer, size_t index);
	/// <summary>
	/// ���_����ݒ�
	/// </summary>
	/// <param name="verticesNum">���_��</param>
	/// <param name="index">���b�V���f�[�^�z��C���f�b�N�X</param>
	void SetVerticesNum(UINT verticesNum, size_t index);

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@��ݒ�
	/// </summary>
	/// <param name="indexBuffer">�C���f�b�N�X�o�b�t�@</param>
	/// <param name="index">���b�V���f�[�^�z��C���f�b�N�X</param>
	void SetIndexBuffer(const Microsoft::WRL::ComPtr<ID3D11Buffer>& indexBuffer, size_t index);
	/// <summary>
	/// �C���f�b�N�X����ݒ�
	/// </summary>
	/// <param name="indexesNum">�C���f�b�N�X��</param>
	/// <param name="index">���b�V���f�[�^�z��C���f�b�N�X</param>
	void SetIndexesNum(UINT indexesNum, size_t index);

	/// <summary>
	/// ���_�̌^�ƃX�g���C�h��ݒ�
	/// </summary>
	/// <param name="index">���b�V���f�[�^�z��C���f�b�N�X</param>
	template<class T>
	void SetVertexType(size_t index)
	{
		m_meshPrimDataVec[index].m_meshTypeIndex = typeid(T);
		m_meshPrimDataVec[index].m_strideNum = sizeof(T);
	}

private:
	std::vector<MeshPrimData> m_meshPrimDataVec;	// ���b�V���f�[�^�z��
};



/// <summary>
/// �X�v���C�g�x�[�X
/// </summary>
class SpriteBase
	: public DrawComponentBase
{
public:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	explicit SpriteBase(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~SpriteBase();

public:
	void OnCreate()override;
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���_�ύX�ł���X�v���C�g���b�V�����쐬
	/// ���łɍ쐬�ς݂̏ꍇ�͍����ւ���
	/// </summary>
	/// <param name="vertices">���_�z��</param>
	/// <param name="indexes">���_�C���f�b�N�X�z��</param>
	template<class T>
	void CreateMesh(std::vector<T>& vertices, std::vector<uint16_t>& indexes)
	{
		try
		{
			// ���b�V���쐬(���_�ύX��)
			auto mesh = MeshResource::CreateMeshResouce(vertices, indexes, true);
			SetMeshResource(mesh);
		}
		catch (...)
		{
			throw;
		}
	}
	/// <summary>
	/// ���_�o�b�t�@���X�V
	/// </summary>
	/// <param name="verticies">���_�z��</param>
	template<class T>
	void UpdateVertices(const std::vector<T>& verticiesVec)
	{
		auto mesh = GetMeshResource();
		if (!mesh)
		{
			throw StandardException(
				"���b�V�����쐬����Ă��Ȃ�",
				"if(!GetMeshResource())",
				"SpriteBase::UpdateVertices()"
			);
		}
		if (verticiesVec.size() > mesh->GetVerticesNum())
		{
			throw StandardException(
				"�X�V���钸�_�����傫��",
				"if(Vertices.size() > mesh->GetVerticesNum()",
				"SpriteBase::UpdateVertices()"
			);
		}

		auto dev = APP->GetDeviceResources();
		auto pD3D11DeviceContext = dev->GetD3DDeviceContext();

		// ���_�ύX
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;	// GPU�Ɏז�����Ȃ��悤�ɂ���
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		// ���_�}�b�v
		if (FAILED(pD3D11DeviceContext->Map(mesh->GetVertexBuffer().Get(), 0, mapType, 0, &mappedBuffer)))
		{
			throw StandardException(
				"���_��Map�Ɏ��s",
				"if(FAILED(pD3D11DeviceContext->Map()))",
				"SpriteBase::UpdateVertices()"
			);
		}
		// ���_�ύX
		T* vertices = (T*)mappedBuffer.pData;
		for (size_t i = 0; i < mesh->GetVerticesNum(); ++i)
		{
			vertices[i] = verticiesVec[i];
		}
		// �A���}�b�v
		pD3D11DeviceContext->Unmap(mesh->GetVertexBuffer().Get(), 0);
	}

	/// <summary>
	/// ���b�V�����\�[�X���擾
	/// </summary>
	/// <returns></returns>
	std::shared_ptr <MeshResource> GetMeshResource()const;
	/// <summary>
	/// ���b�V�����\�[�X��ݒ�
	/// </summary>
	/// <param name="meshResource"></param>
	void SetMeshResource(const std::shared_ptr<MeshResource>& meshResource);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�
	/// </summary>
	/// <param name="textureResource"></param>
	void SetTextureResource(const std::shared_ptr<TextureResouce>& textureResource);
	/// <summary>
	/// �e�N�X�`�����\�[�X��ݒ�
	/// </summary>
	/// <param name="textureKey">�o�^����Ă���L�[</param>
	void SetTextureResource(const std::string& textureKey);
	/// <summary>
	/// �e�N�X�`�����\�[�X���擾
	/// </summary>
	std::shared_ptr<TextureResouce> GetTextureResource()const;

	/// <summary>
	/// �G�~�b�V�u���擾
	/// </summary>
	Float4 GetEmissive()const;
	/// <summary>
	/// �G�~�b�V�u��ݒ�
	/// </summary>
	void SetEmissive(const Float4& emissive);
	/// <summary>
	/// �f�B�t���[�Y���擾
	/// </summary>
	Float4 GetDiffese()const;
	/// <summary>
	/// �f�B�t���[�Y��ݒ�
	/// </summary>
	void SetDiffuse(const Float4& diffuse);

	/// <summary>
	/// �V�F�[�_�[��ݒ�
	/// </summary>
	template<class T_VShader, class T_PShader>
	void SetShader()
	{
		SetVertexShader(T_VShader::GetInstance()->GetShader(),
			T_VShader::GetInstance()->GetInputLayout());
		SetPixelShader(T_PShader::GetInstance()->GetShader());
	}

	/// <summary>
	/// ���_�V�F�[�_��ݒ�
	/// </summary>
	void SetVertexShader(ID3D11VertexShader* pVS, ID3D11InputLayout* pIL);
	/// <summary>
	/// �s�N�Z���V�F�[�_��ݒ�
	/// </summary>
	void SetPixelShader(ID3D11PixelShader* pPS);

protected:
	/// <summary>
	/// �萔�o�b�t�@��ݒ�
	/// </summary>
	/// <param name="spriteConstant"></param>
	void WriteConstantBuffer(SpriteConstants& spriteConstant);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};




/// <summary>
/// SimpleConstantBuffer���g���`��R���|�[�l���g
/// </summary>
class SimpleCBDrawComponent
	: public DrawComponentBase
{
public:
	/// <summary>
	/// �v���e�N�g�R���X�g���N�^
	/// </summary>
	explicit SimpleCBDrawComponent(const std::shared_ptr<GameObject>& pGameObject);
	/// <summary>
	/// �v���e�N�g�f�X�g���N�^
	/// </summary>
	virtual ~SimpleCBDrawComponent();

public:
	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw()override;


protected:
	/// <summary>
	/// �萔�o�b�t�@���X�V
	/// </summary>
	/// <param name="SCB"></param>
	/// <param name="data"></param>
	void WriteConstants(SimpleConstants& SCB, const MeshPrimData& data);


private:
};

/// <summary>
/// BasicConstantBuffer���g���`��R���|�[�l���g
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
	/// �萔�o�b�t�@���X�V
	/// </summary>
	/// <param name="BCB"></param>
	void WriteConstants(BasicConstants& BCB, const MeshPrimData& data);
};

///// <summary>
///// ���W�E�@���EUV������3DDrawComponent
///// </summary>
//class PNTStaticDraw
//	: public DrawSolidBase
//{
//public:
//	PNTStaticDraw();
//	virtual ~PNTStaticDraw();
//
//	/// <summary>
//	/// ����������
//	/// </summary>
//	virtual void OnCreate()override;
//	/// <summary>
//	/// �`�揈��
//	/// </summary>
//	virtual void Draw()override;
//};


#if 0
/// <summary>
/// �`��Ɏg�p����\����(�x�[�X)
/// </summary>
struct DrawObjectBase
{
	std::weak_ptr<MeshResource> m_meshResource;	// ���b�V�����\�[�X
	std::weak_ptr<MultiMeshResource> m_multiMeshResource; // �}���`���b�V�����\�[�X

	//std::vector<Float4x4> m_localBoneMatrix;	// ���[�J���{�[���s��
	//std::vector<std::vector<Float4x4>> m_multiLocalBonesMatrix;	// ���[�J���{�[���s��(�}���`���b�V���p
	//
	//std::map<std::string, animationdata> m_animationMap;	// �A�j���[�V�����}�b�v
	//std::string m_currentAnimationName;	// ���݂̃A�j���[�V������
	//float m_currentAnimationTime;	// ���݂̃A�j���[�V�����^�C��

	DrawObjectBase()
		//:m_currentAnimationName(""), m_currentAnimationTime(0.0f)
	{}


};
#endif // 0
