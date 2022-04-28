#pragma once
#include <GameObject.h>


class SkyDome
	: public GameObject
{
public:
	SkyDome(const std::shared_ptr<Stage>& pStage);
	~SkyDome();

	void OnCreate()override;
	void LateUpdate()override;

	virtual void Destroy() override;
	
	/// <summary>
	/// 追従するトランスフォームを設定
	/// </summary>
	/// <param name="transform"></param>
	void SetCenterTransform(const std::shared_ptr<Transform>& transform);

private:
	struct Param;
	std::unique_ptr<Param> m_param;

};
