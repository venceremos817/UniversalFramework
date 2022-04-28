#pragma once
#include <DeviceResource.h>
#include <System/Resource/VertexHelper.h>

// �V�F�[�_�錾�p�}�N��
#define DECLARE_DX11_VERTEX_SHADER(ShaderName,VertexType) class ShaderName : public VertexShader<ShaderName,VertexType>{public: ShaderName(); };
#define DECLARE_DX11_PIXEL_SHADER(ShaderName) class ShaderName : public PixelShader<ShaderName>{public: ShaderName(); };
// �萔�o�b�t�@�錾�p�}�N��
#define DECLARE_DX11_CONSTANT_BUFFER(CBName, CBStruct) class CBName : public ConstantBuffer<CBName, CBStruct>{ public: };


//=======================================================
// VertexShader
//=======================================================
#pragma region VertexShader
// �X�v���C�g�p
DECLARE_DX11_VERTEX_SHADER(VS_PNTSprite, VertexPositionNormalTexture)

DECLARE_DX11_VERTEX_SHADER(VS_PNTStatic, VertexPositionNormalTexture)

DECLARE_DX11_VERTEX_SHADER(VS_PNTTg_BUMP, VertexPositionNormalTextureTangent)

#pragma endregion

//=======================================================
// PixelShader
//=======================================================
#pragma region PixelShader
// �X�v���C�g�p
DECLARE_DX11_PIXEL_SHADER(PS_PNTSprite)

DECLARE_DX11_PIXEL_SHADER(PS_PNTStatic)

// �g�D�[���V�F�[�_�[
DECLARE_DX11_PIXEL_SHADER(PS_PNT_Toon)

DECLARE_DX11_PIXEL_SHADER(PS_PNTTg_BUMP)

#pragma endregion


//=======================================================
// ConstantBuffer
//=======================================================
#pragma region ConstantBuffer

/// <summary>
/// �X�v���C�g�p�萔�o�b�t�@�\����
/// </summary>
struct SpriteConstants
{
	Float4x4 world;		// ���[���h�}�g���N�X
	Float4 emissive;	// �G�~�b�V�u
	Float4 diffuse;		// �f�B�t���[�Y

	SpriteConstants()
		: emissive(1.0f)
		, diffuse(1.0f)
	{
		memset(this, 0, sizeof(SpriteConstants));
	}
};

struct SimpleConstants
{
	Float4x4 world;
	Float4x4 view;
	Float4x4 projection;
	Float4 emissive;
	Float4 diffuse;
	Float4 specular;
	DirectX::XMUINT4 activeFlg;
	Float4 lightDir;
	Float4 eyePos;
	Float4x4 lightView;
	Float4x4 lightProjection;
	Float4 uvScale;

	SimpleConstants()
	{
		memset(this, 0, sizeof(SimpleConstants));
		diffuse = Float4(1.0f);
		uvScale = Float4(1.0f);
	}
};

/// <summary>
/// BasicShader�p�萔�o�b�t�@�\����
/// </summary>
struct BasicConstants
{
	Float4x4 world;
	Float4 worldInversTranspose[3];
	Float4x4 worldViewProj;

	Float4 diffuse;
	Float4 emissive;
	Float4 specular;

	// �ėp�t���O
	DirectX::XMUINT4 activeFlg;

	Float4 eyePosition;

	Float4 lightPos;
	Float4 lightDirection;
	Float4 lightDiffuse;
	Float4 lightSpecular;
	Float4x4 lightView;
	Float4x4 lightProjection;
	Float4 uvScale;

	BasicConstants()
	{
		memset(this, 0, sizeof(BasicConstants));
		diffuse = Float4(1.0f);
		uvScale = Float4(1.0f);
	}
};
/// <summary>
/// �X�v���C�g�萔�o�b�t�@
/// </summary>
DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstants)

DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
/// <summary>
/// BasicShader�p�萔�o�b�t�@
/// </summary>
DECLARE_DX11_CONSTANT_BUFFER(CBBasic, BasicConstants)

#pragma endregion
