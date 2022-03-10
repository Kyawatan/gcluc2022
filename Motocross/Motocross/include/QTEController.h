#pragma once
#include <list>
#include "Definition.h"
#include "QTEUIManager.h"
#include "TaskPlayer.h"

// 各トリック難度のキー入力数
enum class E_TrikDifficulty
{
	Beginner = 2,	// 初級
	Intermediate,	// 中級
	Advanced,		// 上級
};

enum class E_QTEState
{
	None,		// QTE中でない
	During,		// QTE中
	Success,	// QTE成功
	Failure,	// QTE失敗
};

class QTEController
{
public:
	QTEController();
	~QTEController();

	void Update();
	void SetPlayerInstance(TaskPlayer* pPlayer);
	void StartQTE(E_TrikDifficulty eTrikDifficulty);
	void FinishQTE();

private:
	TaskPlayer*		m_pPlayer;
	QTEUIManager	m_QTEUIManager;
	E_QTEState		m_eState;
	std::list<int>	m_iInputKeyList;	// 決定したキーの番号リスト
	float	m_fJudgeInvalidTime;
	int		m_iCorrespondingKeys[KEY_USING_NUM];	// QTEで使用するキーを実際のキーコードに対応させる

	void ProceedQTE();
	void SetKey(int iKeyNum);
	bool JudgeKey();
	void SetTrik();
	void SetJump();
};