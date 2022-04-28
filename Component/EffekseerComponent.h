#pragma once

#include <System/Resource/Resource.h>

#include <Component.h>
#include <Utility/XMFLOAT_EX.h>
class Camera;

class EffekseerComponent
	: public Component
{
public:
	explicit EffekseerComponent(std::shared_ptr<GameObject>& parent);
	virtual ~EffekseerComponent();

	void Update()override;
	void OnDestroy()override;

	/// <summary>
	/// �Đ�
	/// </summary>
	/// <param name="pos"></param>
	void Play(bool follow = true, bool mirror = false, const Float3& pos = Float3(0.0f, 0.0f, 0.0f), const Float3& scale = Float3(1.0f, 1.0f, 1.0f));
	/// <summary>
	/// ��~
	/// </summary>
	void Stop();

	bool IsPlaying();

	void SetPause(bool pause);
	/// <summary>
	/// �ꎞ��~����Ă��邩�擾
	/// </summary>
	bool GetPaused();

	/// <summary>
	/// �G�t�F�N�g��ݒ�
	/// </summary>
	/// <param name="effect"></param>
	void SetEffect(const std::shared_ptr<EffekseerEffectResource>& effect);

	const std::shared_ptr<EffekseerEffectResource>& GetEffect()const;

	void SetRotation(const Float3& axis, float angle);

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};
