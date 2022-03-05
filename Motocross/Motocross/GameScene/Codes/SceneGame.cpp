#include "SceneGame.h"
#include "TaskPlayer.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_MainCamera()
	, m_LaneManager()
	, m_CourseGenerator(&m_LaneManager)
	, m_pPlayer(NULL)
{
	// プレイヤー生成
	m_pPlayer = new TaskPlayer(&m_LaneManager);
	// メインカメラ位置
	m_MainCamera.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	// メインカメラ
	m_MainCamera.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCameraMovement());
}