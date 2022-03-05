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
	// �v���C���[����
	m_pPlayer = new TaskPlayer(&m_LaneManager);
	// ���C���J�����ʒu
	m_MainCamera.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	// ���C���J����
	m_MainCamera.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCameraMovement());
}