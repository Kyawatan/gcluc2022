#include "SceneTitle.h"
#include "TexQuad.h"
#include "TaskWindowEffect.h"
#include "TaskBackGround.h"

#define FINISH_WAIT_TIME 1.0f

SceneTitle::SceneTitle()
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_isOnce(true)
	, m_fWaitTime(0)
{
	// �E�B���h�E�G�t�F�N�g����
	m_pWindowEffect = new TaskWindowEffect(0);
	// ���C���J�����ʒu
	m_CameraController.SetDefault();

	// �w�i����
	TaskBase* back = new TaskBackGround(E_SceneName::Title);
	// ���j���[�|�C���^����
	
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Update()
{
	if (m_isOnce)
	{
		m_isOnce = false;
	}

	// SPACE�L�[�����ŃV�[���I��
	if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		m_fWaitTime = FINISH_WAIT_TIME;
	}
	if (0 < m_fWaitTime) Finish();

	// ���j���[����
	if (GetpKeyState()->Down(E_KEY_NAME::W))
	{

	}
	else if (GetpKeyState()->Down(E_KEY_NAME::S))
	{

	}
}

void SceneTitle::Finish()
{
	m_fWaitTime -= GetDeltaTime();
	if (m_fWaitTime <= 0)
	{
		// �V�[���I��
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Game));
		DeleteAllTask();
	}
}