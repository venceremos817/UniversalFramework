#include "SceneInterface.h"
#include <App.h>
#include <Stage.h>
#include <DrawComponent/DrawComponent.h>

struct SceneInterface::Param
{
	bool m_bCreated;	// 作成済みかどうか
	std::shared_ptr<Stage> m_pStage;

	Param() :m_bCreated(false), m_pStage() {}
};


SceneInterface::SceneInterface()
	: m_param(new Param())
{
	//--- 基本リソース作成 ---
	// メッシュ
	std::vector<uint16_t> indexes;
	std::vector<VertexPositionNormalTexture> pntV;
	std::vector<VertexPositionColor> pcV;
	std::vector<VertexPositionTexture> ptV;
	std::vector<VertexPositionNormalTextureTangent> pnttgV;
	MeshResource::CreateCubeVertices(1.0f, pntV, indexes);
	MeshResource::ConvertVertex(pntV, pcV, ptV, pnttgV);
	APP->RegisterResource("DEFAULT_CUBE", MeshResource::CreateMeshResource<VertexPositionNormalTexture>(pntV, indexes));
	APP->RegisterResource("DEFAULT_PNTTG_CUBE", MeshResource::CreateMeshResource<VertexPositionNormalTextureTangent>(pnttgV, indexes));
	APP->RegisterResource("DEFAULT_SQUARE", MeshResource::CreateSquareMeshResource(1.0f));
	APP->RegisterResource("DEFAULT_SPHERE", MeshResource::CreateSphereMeshResource(1.0F, 20));
	MeshResource::CreateCapsuleVertices(1.0f, 1.0f, 20, pntV, indexes);
	MeshResource::ConvertVertex(pntV, pcV, ptV, pnttgV);
	//APP->RegisterResource("DEFAULT_CAPSULE", MeshResource::CreateCapsuleMeshResource(1.0f, 1.0f, 20));
	APP->RegisterResource("DEFAULT_CAPSULE", MeshResource::CreateMeshResource<VertexPositionNormalTexture>(pntV, indexes));
	APP->RegisterResource("DEFAULT_PNTTG_CAPSULE", MeshResource::CreateMeshResource<VertexPositionNormalTextureTangent>(pnttgV, indexes));
	// テクスチャ
	APP->RegisterTexture("NUM256_TEXTURE", "Assets/num256.png");
	APP->RegisterTexture("CHARACTER_TEXTURE", "Assets/Textures/ScifiPanel02.tga");
	APP->RegisterTexture("CHARACTER_NORMAL_TEXTURE", "Assets/Textures/ScifiPanel02_n.tga");
	APP->RegisterTexture("RETICLE_TEXTURE", "Assets/reticle.png");
	APP->RegisterTexture("ASPHALT_ALBEDO_TEXTURE", "Assets/Asphalt_texture14/Asphalt_texture_14.png");
	APP->RegisterTexture("ASPHALT_NORMAL_TEXTURE", "Assets/Asphalt_texture14/NormalMap-14.png");
	APP->RegisterTexture("SKYDOME_TEXTURE", "Assets/sky-4.png");
	APP->RegisterTexture("RAMP_TEXTURE", "Assets/RampTex.bmp");
	// エフェクト
	APP->RegisterEffect("HIT_EFFECT", u"Assets/01_AndrewFM01/hit_eff.efk");
	APP->RegisterEffect("BOMB_EFFECT", u"Assets/bomb.efk");
	APP->RegisterEffect("HEAT_BULLET_EFFECT", u"Assets/TouhouStrategy/remilia_heart_bullet.efk");
	APP->RegisterEffect("DRILL_BULLET_EFFECT", u"Assets/01_AndrewFM01/drill.efk");
	APP->RegisterEffect("STEP_WAVE_EFFECT", u"Assets/tktk01/Blow10.efk");
}

SceneInterface::~SceneInterface()
{
}

void SceneInterface::Draw()
{
	if (m_param->m_pStage)
	{
		auto dev = APP->GetDeviceResources();
		dev->ClearDefaultView();
		m_param->m_pStage->RenderStage();
	}
}

std::shared_ptr<Stage> SceneInterface::GetStage() const
{
	return m_param->m_pStage;
}

void SceneInterface::SetCreated(bool created)
{
	m_param->m_bCreated = created;
}

bool SceneInterface::IsCreated()
{
	return m_param->m_bCreated;
}

void SceneInterface::SetStage(const std::shared_ptr<Stage>& stage)
{
	m_param->m_pStage = stage;
}
