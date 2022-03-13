#include "QTEController.h"
#include <random>

#define INVALID_TIME 0.1f


QTEController::QTEController(GameDirector* pGameDirector)
	: m_pGameDirector(pGameDirector)
	, m_pPlayer(NULL)
	, m_QTEUIManager()
	, m_eState(E_QTEState::None)
	, m_iInputKeyList(NULL)
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
	case E_EventName::StartQTE:
		// QTE�J�n
		if (m_pPlayer == NULL) m_pPlayer = m_pGameDirector->GetPlayerInstance();
		StartQTE(E_TrikDifficulty::Beginner);
		break;

	case E_EventName::EndQTE:
		// QTE�I��
		FinishQTE();
		break;
	}

	switch (m_eState)
	{
	case E_QTEState::None:
		break;

	case E_QTEState::During:
		ProceedQTE(); // QTE�i�s
		break;

	case E_QTEState::Success:
		
		break;

	case E_QTEState::Failure:
		
		break;
	}
}

void QTEController::StartQTE(E_TrikDifficulty eTrikDifficulty)
{
	// ���̓L�[����
	int iTrikDifficulty = static_cast<int>(eTrikDifficulty);
	SetKey(iTrikDifficulty);
	m_eState = E_QTEState::During;
	m_pPlayer->SetEvent(); // �v���C���[���C�x���g������Ԃɂ���
}

void QTEController::FinishQTE()
{
	if (m_eState == E_QTEState::Success)
	{
		// QTE���������Ă�����v���C���[�Ƀg���b�N�����߂�����
		m_pPlayer->SetTrik(E_TrikName::NormalJump);
	}
	else
	{
		// QTE�����sor�p�����ł���΁A�v���C���[�ɂ����̃W�����v��������
		m_pPlayer->FinishEvent();
		m_pPlayer->SetTrik(E_TrikName::NormalJump);
	}
	// �c���Ă���L�[���폜
	m_iInputKeyList.clear();
	m_QTEUIManager.DeleteInputKeyList();
	m_eState = E_QTEState::None;
}

void QTEController::ProceedQTE()
{
	if (m_iInputKeyList.size() != 0)
	{
		// ���̓L�[���܂��c���Ă���΃L�[����
		if (!JudgeKey())
		{
			// �L�[���͂��ԈႦ����QTE���s�A�����̃W�����v���Z�b�g����
			SetJump();
		}
	}
	if (m_iInputKeyList.size() == 0)
	{
		// �S�ẴL�[����͂��I������QTE�����A�g���b�N���Z�b�g����
		SetTrik();
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
			m_QTEUIManager.DeleteInputKeyList();
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

void QTEController::SetTrik()
{
	m_pPlayer->FinishEvent();
	m_eState = E_QTEState::Success;
}

void QTEController::SetJump()
{
	m_pPlayer->FinishEvent();
	m_eState = E_QTEState::Failure;
}