#pragma once
#include "QuadBase.h"
#include "Definition.h"


class TaskNumber : public TaskBase
{
public:
	TaskNumber();
	~TaskNumber();

	void Update() override;
	void Draw() override;

	void SetNumber(int iNumber, float posX, float posY);
	void Disable();

private:
	QuadBase*	m_pSprite;
	int			m_iNumber;
	KVector3	m_vPos;
	bool		m_onEnable;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskNumber::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskNumber::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();							// 処理が終了した関数から抜ける

};