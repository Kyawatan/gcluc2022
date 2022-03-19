#include "SceneResult.h"
#include "TaskWindowEffect.h"
#include "TaskNumber.h"
#include "TaskBackGround.h"


SceneResult::SceneResult(int iTotalScore)
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_fWaitTime(0)
	, m_iCount(0)
	, m_iTotalScore(iTotalScore)
	, m_isOnce(true)
{
	// ウィンドウエフェクト生成
	m_pWindowEffect = new TaskWindowEffect(0);
	SetBackGroundColor(0, 0, 0);
	// 数字生成
	m_pNumber = new TaskNumber();
	// 背景生成
	TaskBase* back = new TaskBackGround(E_SceneName::Result);
	// メインカメラ位置
	m_CameraController.SetDefault();
}

SceneResult::~SceneResult()
{

}

void SceneResult::Update()
{
	if (m_isOnce)
	{
		m_fWaitTime = 5.0f;
		m_isOnce = false;
	}
	m_fWaitTime -= GetDeltaTime();
	if (m_fWaitTime <= 0 && m_iCount == 0)
	{
		// SPACEキー押下でシーン終了
		if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
		{
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.8f);
			m_fWaitTime = 1.5f;
			m_iCount++;
		}
	}
	else if (m_fWaitTime <= 0 && m_iCount == 1)
	{
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Title));
		DeleteAllTask();
	}
}
