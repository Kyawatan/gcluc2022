/*
	ゲーム全体の管理、シーン管理
*/

#pragma once
#include "Definition.h"

// シーン名
enum class E_SceneName
{
	Title,
	Game,
	Result,
};

class Main
{
public:
	Main();
	~Main();

	void App();		// アプリケーション処理

private:
	void Setup();	// 初期化処理
	void Update();	// アップデート処理
	void (Main::* m_pFunc)(); // 関数ポインタ
	void SetFunc(void (Main::* pFunc)()) { m_pFunc = pFunc; } // 実行する関数をセットする

	E_SceneName m_nowScene;
	SceneBase* m_pScene;
};

