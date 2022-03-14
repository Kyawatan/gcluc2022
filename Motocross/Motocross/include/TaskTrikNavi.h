#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskTrikNavi : public TaskBase
{
public:
	TaskTrikNavi(GameDirector* pGameDirector, int iIndex, E_TrikDifficulty eDifficulty, KVector3 vPos);
	~TaskTrikNavi();

	void Update() override;
	void Draw() override;

private:
	QuadBase*	m_pSprite;
	GameDirector* m_pGameDirector;
	int			m_iTrikNum;
	int			m_iJumpRumpIndex; // 何番目のジャンプ台に属しているか
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;
	float		m_fEmphasizeCos; // ふきだし上下移動時のcosの値
	float		m_fFadeOutOpacityWait; // フェードアウト時の透明化の待ち時間

	void (TaskTrikNavi::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskTrikNavi::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();								// 処理が終了した関数から抜ける

	void Emphasize();
	void FadeOut(bool isSelect);
	void SelectFadeOut();
	void FadeOut();
};