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
	// �R�[�X����
	m_CourseGenerator.Init();
	// �v���C���[����
	m_pPlayer = new TaskPlayer(&m_GameDirector);
	m_GameDirector.SetPlayerInstance(dynamic_cast<TaskPlayer*>(m_pPlayer));
	// ���C���J�����ʒu
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