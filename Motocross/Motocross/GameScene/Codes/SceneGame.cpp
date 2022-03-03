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
	m_pPlayer->m_TaskTransform.SetTransform
	(
		KVector3{ -2600, 0, 0 },		// pos
		KVector3{ 0, 0, 0 },			// rot
		KVector3{ 1, 1, 1 }				// scale
	);

	// ���C���J����
	m_pMainCamera = new CameraController();
	m_pMainCamera->FollowPlayer(m_pPlayer->m_TaskTransform.GetPosition());
}

SceneGame::~SceneGame()
{
	delete m_pMainCamera;
}

void SceneGame::Update()
{
	m_pMainCamera->FollowPlayer(m_pPlayer->m_TaskTransform.GetPosition());
}