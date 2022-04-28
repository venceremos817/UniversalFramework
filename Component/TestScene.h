#pragma once

#include <SceneInterface.h>


class TestScene
	: public SceneInterface
{
public:
	TestScene();
	virtual ~TestScene() {}

	void OnPreCreate()override;
	void OnCreate()override;
	void Update()override;
	void Draw()override;
	void OnDestroy()override;

private:
};
