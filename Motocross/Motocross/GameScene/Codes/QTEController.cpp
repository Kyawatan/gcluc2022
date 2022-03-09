#include "QTEController.h"
#include <random>

#define INPUT_INVALID_TIME 0.1f


QTEController::QTEController()
	: m_pPlayer(NULL)
	, m_eState(E_QTEState::None)
	, m_iInputKeyList(NULL)
	, m_pTaskKeyList(NULL)
	, m_fInputInvalidTime(INPUT_INVALID_TIME)
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

void QTEController::SetPlayerInstance(TaskPlayer* pPlayer)
{
	if (m_pPlayer != NULL) return;
	m_pPlayer = pPlayer;
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
		m_pPlayer->Jump();
	}
	else
	{
		// QTE�����sor�p�����ł���΁A�v���C���[�ɂ����̃W�����v��������
		m_pPlayer->Jump();
		// �c���Ă���L�[���폜
		if (m_eState == E_QTEState::During) DeleteKey();
	}
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
	else
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
	// �L�[�O���t�B�b�N�𐶐�����
	for (auto itr = m_iInputKeyList.begin(); itr != m_iInputKeyList.end();)
	{
		TaskBase* key = new TaskKey((*itr));
		m_pTaskKeyList.push_back(key);
		++itr;
	}
	// �Ō�͕K��SPACE�L�[
	TaskBase* spaceKey = new TaskKey((static_cast<int>(E_UsingKey::SPACE)));
	m_pTaskKeyList.push_back(spaceKey);
	m_iInputKeyList.push_back(static_cast<int>(E_UsingKey::SPACE));
}

bool QTEController::JudgeKey()
{
	auto itrInputKey = m_iInputKeyList.begin();
	auto itrTaskKey = m_pTaskKeyList.begin();
	TaskKey* key = dynamic_cast<TaskKey*>(*itrTaskKey);

	if (!key->GetOnEnable())
	{
		key->SetOnEnable(true); // �L�[�O���t�B�b�N����ʂɕ\��
		m_fInputInvalidTime = INPUT_INVALID_TIME;
	}
	if (0 < m_fInputInvalidTime)
	{
		m_fInputInvalidTime -= GetDeltaTime(); // �L�[���͖�������
		return true;
	}

	// ���݉����ꂽ�u�Ԃ̃L�[�̃��X�g�𓾂�
	std::list<E_KEY_NAME> pressedKeyList = GetpKeyState()->GetSomeDownKeys();
	// �Ԉ�����L�[��������Ă�����QTE���s
	for (auto itr = pressedKeyList.begin(); itr != pressedKeyList.end();)
	{
		if ((*itr) != static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)]))
		{
			return false;
		}
		++itr;
	}
	// �������L�[�������ꂽ��L�[�����X�g����O��
	if (GetpKeyState()->Down(static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)])))
	{
		(*itrTaskKey)->SetTaskStateDying();
		m_iInputKeyList.pop_front();
		m_pTaskKeyList.pop_front();
	}
	return true;
}

void QTEController::DeleteKey()
{
	for (auto itr = m_pTaskKeyList.begin(); itr != m_pTaskKeyList.end();)
	{
		(*itr)->SetTaskStateDying();
		++itr;
	}
	m_iInputKeyList.clear();
	m_pTaskKeyList.clear();
}

void QTEController::SetTrik()
{
	m_eState = E_QTEState::Success;
}

void QTEController::SetJump()
{
	DeleteKey();
	m_eState = E_QTEState::Failure;
}