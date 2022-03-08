#include "SceneGame.h"
#include "TaskPlayer.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_MainCamera()
	, m_LaneManager()
	, m_CollisionDetector()
	, m_CourseGenerator(&m_LaneManager, &m_CollisionDetector) // �R�[�X����
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
	
	// �v���C���[�ƃW�����v��iQTE�J�n�ƏI���j�̓����蔻��
	KVector2 vPlayerPoint = dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCollisionPoint();
	int eCollisionNum = m_CollisionDetector.CollisionDetection(vPlayerPoint);

	switch (eCollisionNum)
	{
	case static_cast<int>(E_CollisionName::StartQTE):
		dynamic_cast<TaskPlayer*>(m_pPlayer)->SetEvent();
		break;

	case static_cast<int>(E_CollisionName::EndQTE):
		dynamic_cast<TaskPlayer*>(m_pPlayer)->Jump();
		break;

	default:
		break;
	}
}