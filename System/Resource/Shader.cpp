#include "Shader.h"

const std::string ShaderDirectory = "Assets/Shader/";

// 頂点シェーダ定義用マクロ
#define IMPLEMENT_DX11_VERTEX_SHADER(ShaderName,CsoFilename) std::unique_ptr<ShaderName,ShaderName::Deleter> ShaderName::m_pInstance; ShaderName::ShaderName() : VertexShader(ShaderDirectory + CsoFilename){}
// ピクセルシェーダ定義用マクロ
#define IMPLEMENT_DX11_PIXEL_SHADER(ShaderName,CsoFilename) std::unique_ptr<ShaderName,ShaderName::Deleter> ShaderName::m_pInstance; ShaderName::ShaderName() : PixelShader(ShaderDirectory + CsoFilename){}
// 定数バッファ定義用マクロ
#define IMPLEMENT_DX11_CONSTANT_BUFFER(CBName) std::unique_ptr<CBName, CBName::Deleter> CBName::m_pInstance;

//====================================================
// マクロによる実装
//====================================================
//==============================
// VertexShader
//==============================
IMPLEMENT_DX11_VERTEX_SHADER(VS_PNTSprite, "VS_PNTSprite.cso")

IMPLEMENT_DX11_VERTEX_SHADER(VS_PNTStatic, "VS_PNTStatic.cso")

IMPLEMENT_DX11_VERTEX_SHADER(VS_PNTTg_BUMP, "VS_Bump.cso")

//=============================
// PixelShader
//=============================
IMPLEMENT_DX11_PIXEL_SHADER(PS_PNTSprite, "PS_PNTSprite.cso")

IMPLEMENT_DX11_PIXEL_SHADER(PS_PNTStatic, "PS_PNTStatic.cso")

IMPLEMENT_DX11_PIXEL_SHADER(PS_PNT_Toon, "PS_PNT_Toon.cso")

IMPLEMENT_DX11_PIXEL_SHADER(PS_PNTTg_BUMP, "PS_Bump.cso")

//===============================
// ConstantBuffer
//===============================
/// <summary>
/// スプライト定数バッファ
/// </summary>
IMPLEMENT_DX11_CONSTANT_BUFFER(CBSprite)
IMPLEMENT_DX11_CONSTANT_BUFFER(CBSimple)
IMPLEMENT_DX11_CONSTANT_BUFFER(CBBasic)