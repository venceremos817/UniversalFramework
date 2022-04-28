#pragma once

#include <GameObject.h>


class DebugSprite
	:public GameObject
{
public:
	explicit DebugSprite(const std::shared_ptr<Stage>& pStage);
	virtual ~DebugSprite();

	virtual void OnCreate()override;
	virtual void Update()override;

	virtual void Destroy()override;

private:
	struct Param;
	std::unique_ptr<Param> m_param;
};

