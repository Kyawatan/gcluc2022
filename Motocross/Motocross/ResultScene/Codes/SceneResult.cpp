#include "SceneResult.h"
#include "TaskWindowEffect.h"


SceneResult::SceneResult()
	: SceneBase()
	, m_pWindowEffect(NULL)
	, m_isOnce(true)
{
	// �E�B���h�E�G�t�F�N�g����
	//m_pWindowEffect = new TaskWindowEffect();
}

SceneResult::~SceneResult()
{

}

void SceneResult::Update()
{
	if (m_isOnce)
	{
		m_isOnce = false;
	}

	// SPACE�L�[�����ŃV�[���I��
	if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
	{
		SetIsFinish();
		SetNextSceneNum(static_cast<int>(E_SceneName::Title));
		DeleteAllTask();
	}
}