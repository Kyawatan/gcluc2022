#include "SceneTitle.h"
#include "TexQuad.h"
#include "TaskWindowEffect.h"
#include "TaskBackGround.h"
#include "TaskMenuPointer.h"

#define FINISH_WAIT_TIME 1.0f


SceneTitle::SceneTitle()
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_pMenuPointer(NULL)
	, m_isOnce(true)
	, m_fWaitTime(0)
{
	// ウィンドウエフェクト生成
	m_pWindowEffect = new TaskWindowEffect(0);
	// メインカメラ位置
	m_CameraController.SetDefault();
	// メニューポインタ生成
	m_pMenuPointer = new TaskMenuPointer();
	// 背景生成
	TaskBase* back = new TaskBackGround(E_SceneName::Title);
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Update()
{
	if (m_isOnce)
	{
		m_isOnce = false;
	}

	// シーン終了処理
	if (0 < m_fWaitTime)
	{
		Finish();
		return;
	}
	// SPACEキー押下でシーン終了
	if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		m_fWaitTime = FINISH_WAIT_TIME;
		dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.8f);
		return;
	}

	// メニュー操作
	if (GetpKeyState()->Down(E_KEY_NAME::W))
	{
		dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->MoveUp();
	}
	else if (GetpKeyState()->Down(E_KEY_NAME::S))
	{
		dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->MoveDown();
	}
}

void SceneTitle::Finish()
{
	m_fWaitTime -= GetDeltaTime();
	if (m_fWaitTime <= 0)
	{
		switch (dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->GetCurrentPos())
		{
		case E_Menu::Play:
			// シーン終了
			SetIsFinish();
			SetNextSceneNum(static_cast<int>(E_SceneName::Game));
			DeleteAllTask();
			break;

		case E_Menu::Book:
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn(0.8f);
			break;

		case E_Menu::Howto:
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn(0.8f);
			break;
		}
	}
}