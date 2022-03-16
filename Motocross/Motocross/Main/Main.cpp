#include "Main.h"
#include "SceneTitle.h"
#include "SceneGame.h"

// �R���X�g���N�^
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Title)
	, m_pScene(NULL)
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
	m_pScene = new SceneTitle();

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

	//case E_SceneName::Result: // �Q�[���I�[�o�[�V�[��
	//	if (m_pScene == NULL) m_pScene = new SceneResult();
	//	break;
	}
	// �V�[���A�b�v�f�[�g
	m_pScene->Update();
	// �V�[���j��
	if (m_pScene->IsFinish())
	{
		m_nowScene = static_cast<E_SceneName>(m_pScene->GetNextSceneNum());
		delete m_pScene;
		m_pScene = NULL;
	}
}