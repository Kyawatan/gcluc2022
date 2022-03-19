#include "SceneResult.h"
#include "TaskWindowEffect.h"
#include "TaskNumber.h"
#include "TaskBackGround.h"


SceneResult::SceneResult(int iTotalScore)
	: SceneBase()
	, m_CameraController()
	, m_ScoreController()
	, m_pWindowEffect(NULL)
	, m_pNumber(NULL)
	, m_fWaitTime(1.5f)
	, m_iCount(0)
	, m_iTotalScore(iTotalScore)
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
	m_fWaitTime -= GetDeltaTime();

	if (m_fWaitTime <= 0)
	{
		switch (m_iCount)
		{
		case 0:
			// スコア表示
			m_ScoreController.SetTotalScore(m_iTotalScore);
			m_fWaitTime = 2.0f;
			m_iCount++;
			break;

		case 1:
			// SPACEキー押下でフェードアウト
			if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
			{
				dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.8f);
				m_fWaitTime = 1.5f;
				m_iCount++;
			}
			break;

		case 2:
			// シーン終了
			SetIsFinish();
			SetNextSceneNum(static_cast<int>(E_SceneName::Title));
			DeleteAllTask();
			break;
		}
	}
}
