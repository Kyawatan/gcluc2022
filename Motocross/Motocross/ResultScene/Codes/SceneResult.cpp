#include "SceneResult.h"
#include "TaskWindowEffect.h"
#include "TaskNumber.h"
#include "TaskBackGround.h"


SceneResult::SceneResult(int iTotalScore)
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_fWaitTime(0)
	, m_iCount(0)
	, m_iTotalScore(iTotalScore)
	, m_isOnce(true)
{
	// �E�B���h�E�G�t�F�N�g����
	m_pWindowEffect = new TaskWindowEffect(0);
	SetBackGroundColor(0, 0, 0);
	// ��������
	m_pNumber = new TaskNumber();
	// �w�i����
	TaskBase* back = new TaskBackGround(E_SceneName::Result);
	// ���C���J�����ʒu
	m_CameraController.SetDefault();
}

SceneResult::~SceneResult()
{

}

void SceneResult::Update()
{
	if (m_isOnce)
	{
		m_fWaitTime = 5.0f;
		m_isOnce = false;
	}
	m_fWaitTime -= GetDeltaTime();
	if (m_fWaitTime <= 0 && m_iCount == 0)
	{
		// SPACE�L�[�����ŃV�[���I��
		if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
		{
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.8f);
			m_fWaitTime = 1.5f;
			m_iCount++;
		}
	}
	else if (m_fWaitTime <= 0 && m_iCount == 1)
	{
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Title));
		DeleteAllTask();
	}
}
