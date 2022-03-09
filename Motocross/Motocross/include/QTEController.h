#pragma once
#include <list>
#include "Definition.h"
#include "TaskPlayer.h"
#include "TaskKey.h"

// 各トリック難度のキー入力数
enum class E_TrikDifficulty
{
	Beginner = 2,		// 初級
	Intermediate = 3,	// 中級
	Advanced = 4,		// 上級
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
	TaskPlayer* m_pPlayer;
	E_QTEState m_eState;
	std::list<int> m_iInputKeyList;
	std::list<TaskBase*> m_pTaskKeyList;

	float m_fInputInvalidTime;					// キー入力無効時間
	int m_iCorrespondingKeys[USING_KEY_NUM];	// QTEで使用するキーを実際のキーコードに対応させる

	void ProceedQTE();
	void SetKey(int iKeyNum);
	bool JudgeKey();
	void DeleteKey();
	void SetTrik();
	void SetJump();
};