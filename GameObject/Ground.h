#pragma once
#include <GameObject.h>


class Ground
	:public GameObject
{
public:
	Ground(const std::shared_ptr<Stage>& pStage) :GameObject(pStage, ObjectType::Ground) {}
	~Ground() {}

	/// <summary>
	/// 初期化処理2
	/// </summary>
	virtual void OnCreate()override;

	virtual void Destroy()override;


};
