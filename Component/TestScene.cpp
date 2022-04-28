#include "TestScene.h"

#include <App.h>
#include <Stage.h>


TestScene::TestScene()
	: SceneInterface()
{
}

void TestScene::OnPreCreate()
{
}

void TestScene::OnCreate()
{
	// �X�e�[�W����
	auto stage = ObjectFactory::Create<Stage>();
	SetStage(stage);
}

void TestScene::Update()
{
	GetStage()->Update();
}

void TestScene::Draw()
{
	auto dev = APP->GetDeviceResources();
	dev->ClearDefaultView();
	GetStage()->RenderStage();

}

void TestScene::OnDestroy()
{
	if (GetStage())
	{
		//delete m_pStage;
		SetStage(nullptr);
	}
}
