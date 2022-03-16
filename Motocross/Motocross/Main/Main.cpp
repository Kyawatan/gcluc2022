#include "Main.h"
#include "SceneTitle.h"
#include "SceneGame.h"

// コンストラクタ
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Title)
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
	m_pScene = new SceneTitle();

	this->SetFunc(&Main::Update);
}

// アップデート処理
void Main::Update()
{
	// シーン生成
	switch (m_nowScene)
	{
	case E_SceneName::Title: // タイトルシーン
		if (m_pScene == NULL) m_pScene = new SceneTitle();
		break;

	case E_SceneName::Game: // ゲームシーン
		if (m_pScene == NULL) m_pScene = new SceneGame();
		break;

	//case E_SceneName::Result: // ゲームオーバーシーン
	//	if (m_pScene == NULL) m_pScene = new SceneResult();
	//	break;
	}
	// シーンアップデート
	m_pScene->Update();
	// シーン破棄
	if (m_pScene->IsFinish())
	{
		m_nowScene = static_cast<E_SceneName>(m_pScene->GetNextSceneNum());
		delete m_pScene;
		m_pScene = NULL;
	}
}