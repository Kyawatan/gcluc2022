#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskBackGround.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_pMainCamera(NULL)
	, m_pPlayer(NULL)
{
	// �w�i����
	TaskBackGround* back = new TaskBackGround();

	// �v���C���[����
	m_pPlayer = new TaskPlayer();
	// ���C���J����
	m_pMainCamera = new CameraController();
	m_pMainCamera->SetCourseStart();
}

SceneGame::~SceneGame()
{
	delete m_pMainCamera;
}

void SceneGame::Update()
{
	// ���C���J����
	if (dynamic_cast<TaskPlayer*>(m_pPlayer)->IsGoal())
	{
		m_pMainCamera->SetCourseGoal();
	}
	else
	{
		m_pMainCamera->FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCameraMovement());
	}
}