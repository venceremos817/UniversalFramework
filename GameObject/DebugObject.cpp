#include "DebugObject.h"
#include <App.h>
#include <StringSprite.h>
#include <Stage.h>

struct DebugSprite::Param
{
	std::shared_ptr<StringSprite> fpsSprite;
};

DebugSprite::DebugSprite(const std::shared_ptr<Stage>& pStage)
	: GameObject(pStage)
	, m_param(new Param())
{
}

DebugSprite::~DebugSprite()
{
}

void DebugSprite::OnCreate()
{
	SetSpriteDraw(true);
	GetTransform()->SetWorldPos(Float3(0.0f, 0.0f, 1000.0f));	// ˆê”Ô‰œ
	m_param->fpsSprite = AddComponent<StringSprite>();
	m_param->fpsSprite->SetTextRect(16.0f, 16.0f, 320.0f, 240.0f);
	m_param->fpsSprite->SetBackColor(Float4(0.0f, 0.0f, 0.0f, 0.2f));
}

void DebugSprite::Update()
{
	std::wstring strFPS(L"FPS: ");
	auto fps = APP->GetStepTimer().GetFPS();
	float elapsedTime = APP->GetElapsedTime();
	strFPS += StringSprite::UintToWstr(fps);
	strFPS += L"\nElapsedTime: ";
	strFPS += StringSprite::FloatToWstr(elapsedTime);

	std::wstring str = strFPS;
	m_param->fpsSprite->SetText(str);
}

void DebugSprite::Destroy()
{
	GetStage()->RemoveGameObject(GetSharedPtr<DebugSprite>());
}
