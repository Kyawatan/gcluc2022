#include "Main.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"

// �R���X�g���N�^
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Result)
	, m_pScene(NULL)
	, m_iTotalScore(0)
{
	
}

// �f�X�g���N�^
Main::~Main()
{
	delete m_pScene;
}

// �A�v���P�[�V��������
void Main::App()
{
	(this->*m_pFunc)();
}

// �Z�b�g�A�b�v����
void Main::Setup()
{
	this->SetFunc(&Main::Update);
}

// �A�b�v�f�[�g����
void Main::Update()
{
	// �V�[������
	switch (m_nowScene)
	{
	case E_SceneName::Title: // �^�C�g���V�[��
		if (m_pScene == NULL) m_pScene = new SceneTitle();
		break;

	case E_SceneName::Game: // �Q�[���V�[��
		if (m_pScene == NULL) m_pScene = new SceneGame();
		break;

	case E_SceneName::Result: // �Q�[���I�[�o�[�V�[��
		if (m_pScene == NULL) m_pScene = new SceneResult(m_iTotalScore);
		break;
	}
	// �V�[���A�b�v�f�[�g
	m_pScene->Update();
	// �V�[���j��
	if (m_pScene->IsFinish())
	{
		if (m_nowScene == E_SceneName::Game)
		{
			m_iTotalScore = dynamic_cast<SceneGame*>(m_pScene)->GetTotalScore();
		}
		m_nowScene = static_cast<E_SceneName>(m_pScene->GetNextSceneNum());
		delete m_pScene;
		m_pScene = NULL;
	}
}