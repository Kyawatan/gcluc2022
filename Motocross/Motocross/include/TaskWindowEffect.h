#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskWindowEffect : public TaskBase
{
public:
	TaskWindowEffect(float fOpacity);
	~TaskWindowEffect();

	void Update() override;
	void Draw() override;

	void Dark(float fTime);
	void FadeIn(float fTime);
	void FadeOut(float fTime);

private:
	QuadBase*	m_pSprite;
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;

	float m_fDarkTime;
	float m_fFadeInTime;
	float m_fFadeOutTime;

	void Dark();
	void FadeIn();
	void FadeOut();

	void (TaskWindowEffect::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskWindowEffect::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();									// 処理が終了した関数から抜ける
};