#pragma once
#include "QuadBase.h"
#include "Definition.h"


enum class E_BearTex
{
	Point,
	Rank,
};

class TaskBear : public TaskBase
{
public:
	TaskBear(E_BearTex eTex);
	~TaskBear();

	void Update() override;
	void Draw() override;

	void SetAppear(int iTipNum, float fTime);

private:
	QuadBase*	m_pSprite;
	KVector3	m_vPos;
	int			m_iTipNum;
	float		m_fWaitTime;
	float		m_fAngle;
	bool		m_isInfinity;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskBear::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskBear::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();							// 処理が終了した関数から抜ける

	void Appear();
};