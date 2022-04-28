#include "SkyDome.h"
#include <DrawComponent/DrawComponent.h>
#include <Stage.h>

struct SkyDome::Param
{
	std::shared_ptr<Transform> centerTarget;	// 追従するトランスフォーム
};

SkyDome::SkyDome(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
	, m_param(new Param)
{
}

SkyDome::~SkyDome()
{
}

void SkyDome::OnCreate()
{
	auto transform = GetTransform();
	auto stageCamera = GetStage()->GetView()->GetTargetCamera();
	float farClip = stageCamera->GetFarClip();
	farClip *= 1.85f;
	transform->SetScale(Float3(farClip));
	SetCenterTransform(stageCamera->GetTransform());

	auto drawCom = AddComponent<SimpleCBDrawComponent>();
	drawCom->SetShader<VS_PNTStatic, PS_PNTStatic>();
	drawCom->SetMeshResource(APP->GetResource<MeshResource>("DEFAULT_SPHERE"));
	drawCom->SetTextureResource("SKYDOME_TEXTURE", 0);
	drawCom->SetRasterizerState(RasterizerState::CullFront);
	drawCom->DisableSpecular();
}

void SkyDome::LateUpdate()
{
	if (!m_param->centerTarget)
		return;

	auto transform = GetTransform();
	transform->SetWorldPos(m_param->centerTarget->GetWorldPos());
}

void SkyDome::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<SkyDome>());
}

void SkyDome::SetCenterTransform(const std::shared_ptr<Transform>& transform)
{
	m_param->centerTarget = transform;
}

