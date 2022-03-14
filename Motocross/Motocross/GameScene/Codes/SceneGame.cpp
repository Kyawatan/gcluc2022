#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskWindowEffect.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_CameraController()
	, m_GameDirector()
	, m_CourseGenerator(&m_GameDirector)
	, m_QTEController(&m_GameDirector)
	, m_ScoreController()
	, m_pWindowEffect(NULL)
	, m_pPlayer(NULL)
	, m_isOnce(true)
{
	// コース生成
	m_CourseGenerator.Init();
	// ウィンドウエフェクト生成
	m_pWindowEffect = new TaskWindowEffect();
	// プレイヤー生成
	m_pPlayer = new TaskPlayer(&m_GameDirector, &m_ScoreController);
	m_GameDirector.SetInstance(dynamic_cast<TaskPlayer*>(m_pPlayer), dynamic_cast<TaskWindowEffect*>(m_pWindowEffect));
	// メインカメラ位置
	m_CameraController.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	// ゲーム進行
	switch (m_GameDirector.GetCurrentGameState())
	{
	case E_GameState::BeforeStart:
		if (m_isOnce)
		{
			// 画面フェードイン
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn();
			m_isOnce = false;
		}
		// SPACEキー押下でプレイ中に
		if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
		{
			m_GameDirector.m_eCurrentGameState = E_GameState::Playing;
		}
		break;

	case E_GameState::Playing:
		m_CameraController.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetMovement());
		m_GameDirector.Update();
		m_QTEController.Update();
		m_ScoreController.Update();
		break;

	case E_GameState::AfterGoal:

		break;
	}

}