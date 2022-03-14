#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskWindowEffect : public TaskBase
{
public:
	TaskWindowEffect();
	~TaskWindowEffect();

	void Update() override;
	void Draw() override;

	void Dark();
	void FadeIn();
	void FadeOut();

private:
	QuadBase*	m_pSprite;
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskWindowEffect::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskWindowEffect::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();									// 処理が終了した関数から抜ける
};