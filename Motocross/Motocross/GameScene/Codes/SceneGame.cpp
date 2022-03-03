#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskBackGround.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_pMainCamera(NULL)
	, m_pPlayer(NULL)
{
	// 背景生成
	TaskBackGround* back = new TaskBackGround();

	// プレイヤー生成
	m_pPlayer = new TaskPlayer();
	// メインカメラ
	m_pMainCamera = new CameraController();
	m_pMainCamera->SetCourseStart();
}

SceneGame::~SceneGame()
{
	delete m_pMainCamera;
}

void SceneGame::Update()
{
	m_pMainCamera->FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCameraMovement());
}