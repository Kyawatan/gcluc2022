#include "QTEController.h"
#include <random>

#define INVALID_TIME 0.1f


QTEController::QTEController(GameDirector* pGameDirector)
	: m_pGameDirector(pGameDirector)
	, m_pPlayer(NULL)
	, m_QTEUIManager()
	, m_iInputKeyList(NULL)
	, m_onQTE(false)
	, m_fJudgeInvalidTime(INVALID_TIME)
	, m_iCorrespondingKeys()
{
	m_iCorrespondingKeys[0] = 30;	// W
	m_iCorrespondingKeys[1] = 8;	// A
	m_iCorrespondingKeys[2] = 26;	// S
	m_iCorrespondingKeys[3] = 11;	// D
	m_iCorrespondingKeys[4] = 16;	// I
	m_iCorrespondingKeys[5] = 17;	// J
	m_iCorrespondingKeys[6] = 18;	// K
	m_iCorrespondingKeys[7] = 19;	// L
	m_iCorrespondingKeys[8] = 1;	// SPACE
}

QTEController::~QTEController()
{

}

void QTEController::Update()
{
	switch (m_pGameDirector->GetCurrentEventName())
	{
	case E_EventName::QTEStart:
		// QTE�J�n
		if (m_pPlayer == NULL) m_pPlayer = m_pGameDirector->GetPlayerInstance();
		StartQTE();
		break;

	case E_EventName::QTEEnd:
		// QTE�p�����Ȃ�I��
		if (m_onQTE) FinishQTE();
		break;
	}

	if (m_onQTE)
	{
		ProceedQTE(); // QTE�i�s
	}
}

void QTEController::StartQTE()
{
	// ���̓L�[������
	int iTrikDifficulty = static_cast<int>(m_pGameDirector->GetQTEDifficulty());
	SetKey(iTrikDifficulty);
	m_onQTE = true;
}

void QTEController::FinishQTE()
{
	// �c���Ă���L�[���폜
	m_iInputKeyList.clear();
	m_QTEUIManager.FinishRemainingInputKey();
	m_onQTE = false;
}

void QTEController::ProceedQTE()
{
	if (m_iInputKeyList.size() != 0)
	{
		// ���̓L�[���܂��c���Ă���΃L�[����
		if (!JudgeKey())
		{
			// �L�[���͂��ԈႦ����QTE���s
			FinishQTE();
			m_pPlayer->FinishEvent(false);
		}
	}
	else
	{
		// �S�ẴL�[����͂��I������QTE����
		FinishQTE();
		m_pPlayer->FinishEvent(true);
	}
}

void QTEController::SetKey(int iKeyNum)
{
	// ���̓L�[�������_���Ō���
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(static_cast<int>(E_UsingKey::W), static_cast<int>(E_UsingKey::L));
	for (int i = 0; i < iKeyNum; i++)
	{
		m_iInputKeyList.push_back((int)distr(eng));
	}
	// �Ō�͕K��SPACE�L�[
	m_iInputKeyList.push_back(static_cast<int>(E_UsingKey::SPACE));
	// �L�[UI�����A�\��
	m_QTEUIManager.SetInputKeyList(m_iInputKeyList);

}

bool QTEController::JudgeKey()
{
	// �L�[���͖�������
	if (0 < m_fJudgeInvalidTime)
	{
		m_fJudgeInvalidTime -= GetDeltaTime();
		return true;
	}

	// ���݉����ꂽ�u�Ԃ̃L�[�̃��X�g�𓾂�
	std::list<E_KEY_NAME> pressedKeyList = GetpKeyState()->GetSomeDownKeys();
	auto itrInputKey = m_iInputKeyList.begin();
	// �Ԉ�����L�[��������Ă�������̓L�[�����ׂč폜
	for (auto itr = pressedKeyList.begin(); itr != pressedKeyList.end();)
	{
		if ((*itr) != static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)]))
		{
			m_iInputKeyList.clear();
			m_QTEUIManager.SetFailure();
			m_QTEUIManager.FinishRemainingInputKey();
			return false;
		}
		++itr;
	}
	// �������L�[�������ꂽ�玟�̓��̓L�[���Z�b�g
	if (GetpKeyState()->Down(static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)])))
	{
		m_iInputKeyList.pop_front();
		m_QTEUIManager.SetNextInputKey();
		m_fJudgeInvalidTime = INVALID_TIME;
	}
	return true;
}