#include "SceneTitle.h"
#include "TexQuad.h"
#include "TaskWindowEffect.h"
#include "TaskBackGround.h"


SceneTitle::SceneTitle()
	: SceneBase()
	, m_CameraController()
	, m_pWindowEffect(NULL)
	, m_isOnce(true)
{
	// �E�B���h�E�G�t�F�N�g����
	//m_pWindowEffect = new TaskWindowEffect();
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
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Game));
		DeleteAllTask();
	}

	// ���j���[����
	if (GetpKeyState()->Down(E_KEY_NAME::W))
	{

	}
	else if (GetpKeyState()->Down(E_KEY_NAME::S))
	{

	}
}