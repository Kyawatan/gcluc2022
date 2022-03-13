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
		// QTE開始
		if (m_pPlayer == NULL) m_pPlayer = m_pGameDirector->GetPlayerInstance();
		StartQTE();
		break;

	case E_EventName::QTEEnd:
		// QTE継続中なら終了
		if (m_onQTE) FinishQTE();
		break;
	}

	if (m_onQTE)
	{
		ProceedQTE(); // QTE進行
	}
}

void QTEController::StartQTE()
{
	// 入力キー数決定
	int iTrikDifficulty = static_cast<int>(m_pGameDirector->GetQTEDifficulty());
	SetKey(iTrikDifficulty);
	m_onQTE = true;
}

void QTEController::FinishQTE()
{
	// 残っているキーを削除
	m_iInputKeyList.clear();
	m_QTEUIManager.FinishRemainingInputKey();
	m_onQTE = false;
}

void QTEController::ProceedQTE()
{
	if (m_iInputKeyList.size() != 0)
	{
		// 入力キーがまだ残っていればキー判定
		if (!JudgeKey())
		{
			// キー入力を間違えたらQTE失敗
			FinishQTE();
			m_pPlayer->FinishEvent(false);
		}
	}
	else
	{
		// 全てのキーを入力し終えたらQTE成功
		FinishQTE();
		m_pPlayer->FinishEvent(true);
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
	// 最後は必ずSPACEキー
	m_iInputKeyList.push_back(static_cast<int>(E_UsingKey::SPACE));
	// キーUI生成、表示
	m_QTEUIManager.SetInputKeyList(m_iInputKeyList);

}

bool QTEController::JudgeKey()
{
	// キー入力無効時間
	if (0 < m_fJudgeInvalidTime)
	{
		m_fJudgeInvalidTime -= GetDeltaTime();
		return true;
	}

	// 現在押された瞬間のキーのリストを得る
	std::list<E_KEY_NAME> pressedKeyList = GetpKeyState()->GetSomeDownKeys();
	auto itrInputKey = m_iInputKeyList.begin();
	// 間違ったキーが押されていたら入力キーをすべて削除
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
	// 正しいキーが押されたら次の入力キーをセット
	if (GetpKeyState()->Down(static_cast<E_KEY_NAME>(m_iCorrespondingKeys[(*itrInputKey)])))
	{
		m_iInputKeyList.pop_front();
		m_QTEUIManager.SetNextInputKey();
		m_fJudgeInvalidTime = INVALID_TIME;
	}
	return true;
}