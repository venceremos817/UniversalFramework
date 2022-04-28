#include "EffekseerComponent.h"
#include <App.h>

struct EffekseerComponent::Param
{
	std::shared_ptr<EffekseerEffectResource> effect;	// エフェクトリソース
	Effekseer::Handle handle;	// ハンドル
	bool follow;	// オブジェクトに追従するかどうか
	Float3 offsetLocation;
	bool isPlaying;	// 再生中かどうか
	bool isPausing;	// 一時停止中
	//Float3 offsetScale;

	Param()
		: effect(nullptr), handle(-1), follow(true)
		, offsetLocation(), isPlaying(false)
	{}
};

EffekseerComponent::EffekseerComponent(std::shared_ptr<GameObject>& parent)
	: Component(parent)
	, m_param(new Param)
{
}

EffekseerComponent::~EffekseerComponent()
{
}

void EffekseerComponent::Update()
{
	auto manager = APP->GetEffekseerManagerRef();
	m_param->isPlaying = manager->Exists(m_param->handle);
	m_param->isPausing = manager->GetPaused(m_param->handle);

	// 再生中でなければ更新なし
	//if (!(m_param->isPlaying && m_param->isPausing))
	//	return;

	if (m_param->follow)
	{
		Float3 worldPos = GetGameObject()->GetTransform()->GetWorldPos();
		auto transform = GetGameObject()->GetTransform();
		worldPos += m_param->offsetLocation;
		::Effekseer::Vector3D pos(worldPos.x, worldPos.y, worldPos.z);
		APP->GetEffekseerManagerRef()->SetLocation(m_param->handle, pos);
		auto quat = transform->GetQuaternion();
		//quat.Normalize();
		DirectX::XMVECTOR axisV;
		Float4 axis;
		float angle;
		DirectX::XMQuaternionToAxisAngle(&axisV, &angle, quat);
		axis = DirectX::XMVector3Normalize(axisV);
		::Effekseer::Vector3D efAxis(axis.x, axis.y, axis.z);
		APP->GetEffekseerManagerRef()->SetRotation(m_param->handle, efAxis, angle);
	}
}

void EffekseerComponent::OnDestroy()
{
}

void EffekseerComponent::Play(bool follow, bool mirror, const Float3 & pos, const Float3& scale)
{
	m_param->isPlaying = true;
	m_param->follow = follow;
	auto manager = APP->GetEffekseerManagerRef();
	m_param->handle = manager->Play(m_param->effect->GetEffectResource(), pos.x, pos.y, pos.z);
	if (mirror)
	{
		manager->SetScale(m_param->handle, scale.x, scale.y, scale.z);
	}
	else
	{
		manager->SetScale(m_param->handle, -scale.x, -scale.y, -scale.z);
	}
}

void EffekseerComponent::Stop()
{
	m_param->isPlaying = false;
	APP->GetEffekseerManagerRef()->StopEffect(m_param->handle);
}

bool EffekseerComponent::IsPlaying()
{
	return m_param->isPlaying;
}

void EffekseerComponent::SetPause(bool pause)
{
	m_param->isPausing = pause;
	APP->GetEffekseerManagerRef()->SetPaused(m_param->handle, pause);
}

bool EffekseerComponent::GetPaused()
{
	return m_param->isPausing;
}

void EffekseerComponent::SetEffect(const std::shared_ptr<EffekseerEffectResource>& effect)
{
	m_param->effect = effect;
}

const std::shared_ptr<EffekseerEffectResource>& EffekseerComponent::GetEffect() const
{
	return m_param->effect;
}

void EffekseerComponent::SetRotation(const Float3 & axis, float angle)
{
	::Effekseer::Vector3D efAxis(axis.x, axis.y, axis.z);
	APP->GetEffekseerManagerRef()->SetRotation(m_param->handle, efAxis, angle);
}

