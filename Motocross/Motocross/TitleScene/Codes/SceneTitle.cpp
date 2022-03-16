#include "SceneTitle.h"
#include "TexQuad.h"
#include "TaskWindowEffect.h"
#include "TaskBackGround.h"


SceneTitle::SceneTitle()
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_isOnce(true)
{
	// ウィンドウエフェクト生成
	//m_pWindowEffect = new TaskWindowEffect();
	// メインカメラ位置
	m_CameraController.SetDefault();

	// 背景生成
	TaskBase* back = new TaskBackGround(E_SceneName::Title);
	// メニューポインタ生成
	
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

	// SPACEキー押下でシーン終了
	if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Game));
		DeleteAllTask();
	}

	// メニュー操作
	if (GetpKeyState()->Down(E_KEY_NAME::W))
	{

	}
	else if (GetpKeyState()->Down(E_KEY_NAME::S))
	{

	}
}