#include "SceneTitle.h"
#include "TexQuad.h"
#include "TaskWindowEffect.h"
#include "TaskBackGround.h"
#include "TaskMenuPointer.h"

#define FINISH_WAIT_TIME 1.0f


SceneTitle::SceneTitle()
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_pMenuPointer(NULL)
	, m_isOnce(true)
	, m_fWaitTime(0)
{
	// �E�B���h�E�G�t�F�N�g����
	m_pWindowEffect = new TaskWindowEffect(0);
	// ���C���J�����ʒu
	m_CameraController.SetDefault();
	// ���j���[�|�C���^����
	m_pMenuPointer = new TaskMenuPointer();
	// �w�i����
	TaskBase* back = new TaskBackGround(E_SceneName::Title);
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

	// �V�[���I������
	if (0 < m_fWaitTime)
	{
		Finish();
		return;
	}
	// SPACE�L�[�����ŃV�[���I��
	if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		m_fWaitTime = FINISH_WAIT_TIME;
		dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeOut(0.8f);
		return;
	}

	// ���j���[����
	if (GetpKeyState()->Down(E_KEY_NAME::W))
	{
		dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->MoveUp();
	}
	else if (GetpKeyState()->Down(E_KEY_NAME::S))
	{
		dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->MoveDown();
	}
}

void SceneTitle::Finish()
{
	m_fWaitTime -= GetDeltaTime();
	if (m_fWaitTime <= 0)
	{
		switch (dynamic_cast<TaskMenuPointer*>(m_pMenuPointer)->GetCurrentPos())
		{
		case E_Menu::Play:
			// �V�[���I��
			SetIsFinish();
			SetNextSceneNum(static_cast<int>(E_SceneName::Game));
			DeleteAllTask();
			break;

		case E_Menu::Book:
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn(0.8f);
			break;

		case E_Menu::Howto:
			dynamic_cast<TaskWindowEffect*>(m_pWindowEffect)->FadeIn(0.8f);
			break;
		}
	}
}