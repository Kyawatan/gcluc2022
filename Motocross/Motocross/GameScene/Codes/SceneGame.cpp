#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskWindowEffect.h"
#include "TaskNumber.h"

SceneGame::SceneGame()
	: SceneBase()
	, m_CameraController()
	, m_GameDirector()
	, m_CourseGenerator(&m_GameDirector)
	, m_QTEController(&m_GameDirector)
	, m_ScoreController()
	, m_pWindowEffect(NULL)
	, m_pPlayer(NULL)
	, m_pNumber(NULL)
	, m_isOnce(true)
	, m_fWaitTime(0)
	, m_iCountDown(4)
	, m_iAfterGoalCount(0)
{
	// �R�[�X����
	m_CourseGenerator.Init();
	// �E�B���h�E�G�t�F�N�g����
	m_pWindowEffect = new TaskWindowEffect(1);
	// ��������
	m_pNumber = new TaskNumber();
	// �v���C���[����
	m_pPlayer = new TaskPlayer(&m_GameDirector, &m_ScoreController);
	m_GameDirector.SetInstance(dynamic_cast<TaskPlayer*>(m_pPlayer), dynamic_cast<TaskWindowEffect*>(m_pWindowEffect));
	// ���C���J�����ʒu
	m_CameraController.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	// �Q�[���i�s
	switch (m_GameDirector.GetCurrentGameState())
	{
	case E_GameState::BeforeStart:
		if (m_isOnce)
		{
			// ��ʃt�F�[�h�C��
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn(1.5f);
			m_fWaitTime = 1.5f;
			m_isOnce = false;
		}
		m_fWaitTime -= GetDeltaTime();
		if (m_fWaitTime <= 0)
		{
			// �J�E���g�_�E��
			m_fWaitTime = 1.0f;
			m_iCountDown--;
			if (0 < m_iCountDown)
			{
				// 3,2,1�����\���؂�ւ�
				dynamic_cast<TaskNumber*>(m_pNumber)->SetNumber(m_iCountDown, 0, 100);
			}
			else
			{
				// 0�̃^�C�~���O�ŃQ�[���X�^�[�g
				dynamic_cast<TaskNumber*>(m_pNumber)->Disable();
				m_GameDirector.m_eCurrentGameState = E_GameState::Playing;
				m_isOnce = true;
			}
		}
		break;

	case E_GameState::Playing:
		m_CameraController.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetMovement());
		m_GameDirector.Update();
		m_QTEController.Update();
		m_ScoreController.Update();
		break;

	case E_GameState::AfterGoal:
		if (m_isOnce)
		{
			m_fWaitTime = 4.0f;
			m_isOnce = false;
		}
		m_fWaitTime -= GetDeltaTime();
		if (m_fWaitTime <= 0 && m_iAfterGoalCount == 0)
		{
			// ��ʃt�F�[�h�A�E�g
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.5f);
			m_fWaitTime = 1.0f;
			m_iAfterGoalCount++;
		}
		if (m_fWaitTime <= 0 && m_iAfterGoalCount == 1)
		{
			// �V�[���I���A���U���g��ʑJ��
			SetIsFinish();
			SetNextSceneNum(static_cast<int>(E_SceneName::Result));
			DeleteAllTask();
		}
	}

}

int SceneGame::GetTotalScore()
{
	return m_ScoreController.GetTotalScore();
}