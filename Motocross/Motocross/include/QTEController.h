#pragma once
#include <list>
#include "Definition.h"
#include "TaskPlayer.h"
#include "TaskKey.h"

// �e�g���b�N��x�̃L�[���͐�
enum class E_TrikDifficulty
{
	Beginner = 2,		// ����
	Intermediate = 3,	// ����
	Advanced = 4,		// �㋉
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
	TaskPlayer* m_pPlayer;
	E_QTEState m_eState;
	std::list<int> m_iInputKeyList;
	std::list<TaskBase*> m_pTaskKeyList;

	float m_fInputInvalidTime;					// �L�[���͖�������
	int m_iCorrespondingKeys[USING_KEY_NUM];	// QTE�Ŏg�p����L�[�����ۂ̃L�[�R�[�h�ɑΉ�������

	void ProceedQTE();
	void SetKey(int iKeyNum);
	bool JudgeKey();
	void DeleteKey();
	void SetTrik();
	void SetJump();
};