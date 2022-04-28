#include "Ground.h"
#include <App.h>
#include <CollisionRect.h>
#include <DrawComponent/DrawComponent.h>
#include <Stage.h>

void Ground::OnCreate()
{
	AddTag("ground");
	m_pTransform->SetScale(100.0f, 100.0f, 0.1f);
	m_pTransform->SetQuaternion(Quaternion(Float3(1.0f, 0.0f, 0.0f), DirectX::XM_PIDIV2));
	auto col = AddComponent<CollisionRect>();
	col->SetFixed(true);
	col->SetHitAction(Collision::HitAction::None);

	auto draw = AddComponent<BasicCBDrawComponent>();
	draw->SetShader<VS_PNTTg_BUMP, PS_PNTTg_BUMP>();
	draw->SetMeshResource(APP->GetResource<MeshResource>("DEFAULT_PNTTG_CUBE"));
	draw->SetTextureResource("ASPHALT_ALBEDO_TEXTURE", 0);
	draw->SetTextureResource("ASPHALT_NORMAL_TEXTURE", 1);
	draw->SetSamplerState(SamplerState::AnisotropicWrap);
	draw->SetUVScale(Float2(10.0f));
	draw->SetSpecularPower(0.0f);
}

void Ground::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<Ground>());
}
