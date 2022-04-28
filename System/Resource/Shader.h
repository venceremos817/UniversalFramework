#pragma once
#include <DeviceResource.h>
#include <System/Resource/VertexHelper.h>

// シェーダ宣言用マクロ
#define DECLARE_DX11_VERTEX_SHADER(ShaderName,VertexType) class ShaderName : public VertexShader<ShaderName,VertexType>{public: ShaderName(); };
#define DECLARE_DX11_PIXEL_SHADER(ShaderName) class ShaderName : public PixelShader<ShaderName>{public: ShaderName(); };
// 定数バッファ宣言用マクロ
#define DECLARE_DX11_CONSTANT_BUFFER(CBName, CBStruct) class CBName : public ConstantBuffer<CBName, CBStruct>{ public: };


//=======================================================
// VertexShader
//=======================================================
#pragma region VertexShader
// スプライト用
DECLARE_DX11_VERTEX_SHADER(VS_PNTSprite, VertexPositionNormalTexture)

DECLARE_DX11_VERTEX_SHADER(VS_PNTStatic, VertexPositionNormalTexture)

DECLARE_DX11_VERTEX_SHADER(VS_PNTTg_BUMP, VertexPositionNormalTextureTangent)

#pragma endregion

//=======================================================
// PixelShader
//=======================================================
#pragma region PixelShader
// スプライト用
DECLARE_DX11_PIXEL_SHADER(PS_PNTSprite)

DECLARE_DX11_PIXEL_SHADER(PS_PNTStatic)

// トゥーンシェーダー
DECLARE_DX11_PIXEL_SHADER(PS_PNT_Toon)

DECLARE_DX11_PIXEL_SHADER(PS_PNTTg_BUMP)

#pragma endregion


//=======================================================
// ConstantBuffer
//=======================================================
#pragma region ConstantBuffer

/// <summary>
/// スプライト用定数バッファ構造体
/// </summary>
struct SpriteConstants
{
	Float4x4 world;		// ワールドマトリクス
	Float4 emissive;	// エミッシブ
	Float4 diffuse;		// ディフューズ

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
/// BasicShader用定数バッファ構造体
/// </summary>
struct BasicConstants
{
	Float4x4 world;
	Float4 worldInversTranspose[3];
	Float4x4 worldViewProj;

	Float4 diffuse;
	Float4 emissive;
	Float4 specular;

	// 汎用フラグ
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
/// スプライト定数バッファ
/// </summary>
DECLARE_DX11_CONSTANT_BUFFER(CBSprite, SpriteConstants)

DECLARE_DX11_CONSTANT_BUFFER(CBSimple, SimpleConstants)
/// <summary>
/// BasicShader用定数バッファ
/// </summary>
DECLARE_DX11_CONSTANT_BUFFER(CBBasic, BasicConstants)

#pragma endregion
