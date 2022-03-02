#include "Main.h"
#include "SceneGame.h"

// �R���X�g���N�^
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Game)
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
	m_pScene = new SceneGame();

	this->SetFunc(&Main::Update);
}

// �A�b�v�f�[�g����
void Main::Update()
{
	m_pScene->Update();
}