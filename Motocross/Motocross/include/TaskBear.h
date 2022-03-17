#pragma once
#include "QuadBase.h"
#include "Definition.h"



class TaskBear : public TaskBase
{
public:
	TaskBear();
	~TaskBear();

	void Update() override;
	void Draw() override;

	void SetAppear(int iTipNum);

private:
	QuadBase*	m_pSprite;
	KVector3	m_vPos;
	int			m_iTipNum;
	float		m_fWaitTime;
	float		m_fAngle;
	bool		m_onEnable;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskBear::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskBear::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();							// 処理が終了した関数から抜ける

	void Appear();
};