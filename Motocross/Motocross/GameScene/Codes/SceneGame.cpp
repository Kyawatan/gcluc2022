#include "SceneGame.h"
#include "TaskPlayer.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_CameraController()
	, m_GameDirector()
	, m_CourseGenerator(&m_GameDirector)
	, m_QTEController(&m_GameDirector)
	, m_pPlayer(NULL)
{
	// コース生成
	m_CourseGenerator.Init();
	// プレイヤー生成
	m_pPlayer = new TaskPlayer(&m_GameDirector);
	m_GameDirector.SetPlayerInstance(dynamic_cast<TaskPlayer*>(m_pPlayer));
	// メインカメラ位置
	m_CameraController.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	m_CameraController.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetMovement());
	m_GameDirector.Update();
	m_QTEController.Update();
}