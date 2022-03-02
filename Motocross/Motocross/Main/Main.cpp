#include "Main.h"
#include "SceneGame.h"

// コンストラクタ
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Game)
	, m_pScene(NULL)
{
	
}

// デストラクタ
Main::~Main()
{
	delete m_pScene;
}

// アプリケーション処理
void Main::App()
{
	(this->*m_pFunc)();
}

// セットアップ処理
void Main::Setup()
{
	m_pScene = new SceneGame();

	this->SetFunc(&Main::Update);
}

// アップデート処理
void Main::Update()
{
	m_pScene->Update();
}