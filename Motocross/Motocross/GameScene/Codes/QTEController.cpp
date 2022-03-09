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
		ProceedQTE(); // QTE進行
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
	// 入力キー決定
	int iTrikDifficulty = static_cast<int>(eTrikDifficulty);
	SetKey(iTrikDifficulty);
	m_eState = E_QTEState::During;
	m_pPlayer->SetEvent(); // プレイヤーをイベント発生状態にする
}

void QTEController::FinishQTE()
{
	if (m_eState == E_QTEState::Success)
	{
		// QTEが成功していたらプレイヤーにトリックを決めさせる
		m_pPlayer->Jump();
	}
	else
	{
		// QTEが失敗or継続中であれば、プレイヤーにただのジャンプをさせる
		m_pPlayer->Jump();
		// 残っているキーを削除
		if (m_eState == E_QTEState::During) DeleteKey();
	}
	m_eState = E_QTEState::None;
}

void QTEController::ProceedQTE()
{
	if (m_iInputKeyList.size() != 0)
	{
		// 入力キーがまだ残っていればキー判定
		if (!JudgeKey())
		{
			// キー入力を間違えたらQTE失敗、ただのジャンプをセットする
			SetJump();
		}
	}
	else
	{
		// 全てのキーを入力し終えたらQTE成功、トリックをセットする
		SetTrik();
	}
}

void QTEController::SetKey(int iKeyNum)
{
	// 入力キーをランダムで決定
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(static_cast<int>(E_UsingKey::W), static_cast<int>(E_UsingKey::L));
	for (int i = 0; i < iKeyNum; i++)
	{
		m_iInputKeyList.push_back((int)distr(eng));
	}
	// キーグラフィックを生成する
	for (auto itr = m_iInputKeyList.begin(); itr != m_iInputKeyList.end();)
	{
		TaskBase* key = new TaskKey((*itr));
		m_pTaskKeyList.push_back(key);
		++itr;
	}
	// 最後は必ずSPACEキー
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
		key->SetOnEnable(true); // キーグラフィックを画面に表示
		m_fInputInvalidTime = INPUT_INVALID_TIME;
	}
	if (0 < m_fInputInvalidTime)
	{
		m_fInputInvalidTime -= GetDeltaTime(); // キー入力無効時間
		return true;
	}

	// 現在押された瞬間のキーのリストを得る
	std::list<E_KEY_NAME> pressedKeyList = GetpKeyState()->GetSomeDownKeys();
	// 間違ったキーが押されていたらQTE失敗
	for (auto itr = pressedKeyList.begin(); itr != pressedKeyList.end();)
	{
		if ((*itr) != static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)]))
		{
			return false;
		}
		++itr;
	}
	// 正しいキーが押されたらキーをリストから外す
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