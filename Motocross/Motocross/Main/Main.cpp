#include "Main.h"
#include "SceneTitle.h"
#include "SceneGame.h"
#include "SceneResult.h"

// コンストラクタ
Main::Main()
	: m_pFunc(&Main::Setup)
	, m_nowScene(E_SceneName::Result)
	, m_pScene(NULL)
	, m_iTotalScore(0)
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

	case E_SceneName::Result: // ゲームオーバーシーン
		if (m_pScene == NULL) m_pScene = new SceneResult(m_iTotalScore);
		break;
	}
	// シーンアップデート
	m_pScene->Update();
	// シーン破棄
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