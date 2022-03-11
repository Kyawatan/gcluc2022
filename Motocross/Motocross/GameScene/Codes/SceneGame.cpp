#include "SceneGame.h"
#include "TaskPlayer.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_MainCamera()
	, m_LaneManager()
	, m_CollisionDetector()
	, m_CourseGenerator(&m_LaneManager, &m_CollisionDetector) // �R�[�X����
	, m_QTEController()
	, m_pPlayer(NULL)
{
	// �v���C���[����
	m_pPlayer = new TaskPlayer(&m_LaneManager);
	m_QTEController.SetPlayerInstance(dynamic_cast<TaskPlayer*>(m_pPlayer));
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
	
	// �v���C���[�ƃC�x���g�����|�C���g�̓����蔻��
	KVector2 vPlayerPoint = dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCollisionPoint();
	int eCollisionNum = m_CollisionDetector.CollisionDetection(vPlayerPoint);
	// �C�x���g����
	switch (eCollisionNum)
	{
	case static_cast<int>(E_CollisionName::StartQTE):
		// QTE�J�n
		m_QTEController.StartQTE(E_TrikDifficulty::Intermediate);
		break;

	case static_cast<int>(E_CollisionName::EndQTE):
		// QTE�I��
		m_QTEController.FinishQTE();
		break;

	case static_cast<int>(E_CollisionName::CourseGoal):
		// �S�[��

		break;

	case static_cast<int>(E_CollisionName::ScoringGate):
		// ���_

		break;
	}

	m_QTEController.Update();
}