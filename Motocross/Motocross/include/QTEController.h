#pragma once
#include <list>
#include "Definition.h"
#include "GameDirector.h"
#include "TaskPlayer.h"
#include "TaskWindowEffect.h"
#include "QTEUIManager.h"


class QTEController
{
public:
	QTEController(GameDirector* pGameDirector);
	~QTEController();

	void Update();

private:
	GameDirector*	m_pGameDirector;
	TaskPlayer*		m_pPlayer;
	TaskWindowEffect* m_pWindowEffect;
	QTEUIManager	m_QTEUIManager;
	std::list<int>	m_iInputKeyList;	// 決定したキーの番号リスト
	bool	m_onQTE;
	float	m_fJudgeInvalidTime;
	int		m_iCorrespondingKeys[KEY_USING_NUM];	// QTEで使用するキーを実際のキーコードに対応させる

	void StartQTE();
	void FinishQTE();

	void ProceedQTE();
	void SetKey(int iKeyNum);
	bool JudgeKey();
};