#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskWindowEffect.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_CameraController()
	, m_GameDirector()
	, m_CourseGenerator(&m_GameDirector)
	, m_QTEController(&m_GameDirector)
	, m_pWindowEffect(NULL)
	, m_pPlayer(NULL)
	, m_isOnce(true)
{
	// �R�[�X����
	m_CourseGenerator.Init();
	// �E�B���h�E�G�t�F�N�g����
	m_pWindowEffect = new TaskWindowEffect();
	// �v���C���[����
	m_pPlayer = new TaskPlayer(&m_GameDirector);
	m_GameDirector.SetInstance(dynamic_cast<TaskPlayer*>(m_pPlayer), dynamic_cast<TaskWindowEffect*>(m_pWindowEffect));
	// ���C���J�����ʒu
	m_CameraController.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	if (m_isOnce)
	{
		// ��ʃt�F�[�h�C��
		dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn();
		m_isOnce = false;
	}
	m_CameraController.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetMovement());
	m_GameDirector.Update();
	m_QTEController.Update();
}