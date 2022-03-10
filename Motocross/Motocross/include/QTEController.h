#pragma once
#include <list>
#include "Definition.h"
#include "QTEUIManager.h"
#include "TaskPlayer.h"

// �e�g���b�N��x�̃L�[���͐�
enum class E_TrikDifficulty
{
	Beginner = 2,	// ����
	Intermediate,	// ����
	Advanced,		// �㋉
};

enum class E_QTEState
{
	None,		// QTE���łȂ�
	During,		// QTE��
	Success,	// QTE����
	Failure,	// QTE���s
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
	std::list<int>	m_iInputKeyList;	// ���肵���L�[�̔ԍ����X�g
	float	m_fJudgeInvalidTime;
	int		m_iCorrespondingKeys[KEY_USING_NUM];	// QTE�Ŏg�p����L�[�����ۂ̃L�[�R�[�h�ɑΉ�������

	void ProceedQTE();
	void SetKey(int iKeyNum);
	bool JudgeKey();
	void SetTrik();
	void SetJump();
};