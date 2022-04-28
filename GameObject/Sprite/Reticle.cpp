#include "Reticle.h"
#include <DrawComponent/DrawComponent.h>
#include <Stage.h>
#include <Ray.h>
#include <Enemy/Enemy.h>
#include <Player/Player.h>

struct Reticle::Param
{
	std::shared_ptr<SpriteBase> sprite;	// スプライトコンポーネント
	std::shared_ptr<Camera> camera;		// カメラ
};

Reticle::Reticle(std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
	, m_param(new Param)
{
#ifdef _DEBUG
	AddTag("reticle");
#endif // _DEBUG


	// 透過処理を行う
	SetIsAlpha(true);
	// デフォルトとしてカメラを初期化
	m_param->camera = GetStage()->GetView()->GetTargetCamera();
}

Reticle::~Reticle()
{
}

void Reticle::OnCreate()
{
	auto transform = GetTransform();
	transform->SetWorldPos(0, 0, 0);
	transform->SetScale(50, 50, 1);

	// スプライトコンポーネント
	m_param->sprite = AddComponent<SpriteBase>();
	m_param->sprite->SetMeshResource(APP->GetResource<MeshResource>("DEFAULT_SQUARE"));
	m_param->sprite->SetTextureResource("RETICLE_TEXTURE");
	m_param->sprite->SetBlendState(BlendState::AlphaBlend);
}

void Reticle::Update()
{
	// レティクルの色を元に戻す
	m_param->sprite->SetDiffuse(Float4(1.0f));

	//--- レティクルをカメラの注視点に移動 ---
	auto transform = GetTransform();
	// 注視点をスクリーン座標に変換
	const Float4x4 VPMatrix = m_param->camera->GetViewMatrix() * m_param->camera->GetProjectionMatrix();
	Float3 cameraLook = m_param->camera->GetLook();
	Float4x4 cameraLookScreenMatrix =
		(Float4x4)DirectX::XMMatrixTranslation(cameraLook.x, cameraLook.y, cameraLook.z) * VPMatrix;
	Float3 pos = cameraLookScreenMatrix.ExtractTransform();
	pos.x *= (APP->GetScreenWidth()*0.5f);
	pos.y *= (APP->GetScreenHeight()*0.5f);
	pos /= pos.z;
	pos.z = 0.0f;
	transform->SetWorldPos(pos);


	Float3 cameraToLook = m_param->camera->GetLookDirection().normalized();
	Float3 cameraPos = m_param->camera->GetPos();
	auto enemyVec = EnemyManager::GetInstance()->GetEnemyVectorByDistance(cameraPos);
	for (auto enemy : enemyVec)
	{
		// レイとエネミースフィアがぶつかっていればレティクルの色を変える
		Ray ray(m_param->camera->GetPos(), cameraToLook);
		auto enemySp = enemy->GetCollision()->GetWrappedSphereVol();
		enemySp.m_radius *= GetStage()->GetPlayer()->GetAimAssist();
		float distance;
		Float3 aimPos;
		if (ray.IsHitSphere(enemySp, distance, aimPos))
		{
			// レティクルの色を変える
			m_param->sprite->SetDiffuse(Float4(1.0f, 0.0f, 0.0f, 1.0f));

			// エネミーをスクリーン座標に変換
			auto enemyPos = enemy->GetTransform()->GetWorldPos();
			Float4x4 enemyScreenMatrix =
				(Float4x4)DirectX::XMMatrixTranslation(enemyPos.x, enemyPos.y, enemyPos.z) * VPMatrix;
			pos = enemyScreenMatrix.ExtractTransform();
			pos.x *= (APP->GetScreenWidth()*0.5f);
			pos.y *= (APP->GetScreenHeight()*0.5f);
			pos /= pos.z;
			pos.z = 0.0f;
			transform->SetWorldPos(pos);

			break;
		}
	}

}

void Reticle::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<Reticle>());
}

void Reticle::SetCamera(std::shared_ptr<Camera>& camera)
{
	m_param->camera = camera;
}
