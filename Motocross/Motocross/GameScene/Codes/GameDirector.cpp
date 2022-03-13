#include "GameDirector.h"
#include <assert.h>
#include "TaskPlayer.h"
#include "LaneManager.h"
#include "CollisionDetector.h"


GameDirector::GameDirector()
	: m_eCurrentGameState(E_GameState::BeforeStart)
	, m_eCurrentEventName(E_EventName::None)
	, m_LaneManager()
	, m_CollisionDetector()
	, m_pPlayer(NULL)
{

}

GameDirector::~GameDirector()
{

}

const E_GameState GameDirector::GetCurrentGameState()
{
	return m_eCurrentGameState;
}

const E_EventName GameDirector::GetCurrentEventName()
{
	return m_eCurrentEventName;
}

TaskPlayer* GameDirector::GetPlayerInstance()
{
	return m_pPlayer;
}

LaneManager* GameDirector::GetLaneManagerInstance()
{
	assert(&m_LaneManager != NULL);
	return &m_LaneManager;
}

CollisionDetector* GameDirector::GetCollisionDetectorInstance()
{
	assert(&m_CollisionDetector != NULL);
	return &m_CollisionDetector;
}

void GameDirector::SetPlayerInstance(TaskPlayer* pPlayer)
{
	assert(m_pPlayer == NULL && pPlayer != NULL);
	m_pPlayer = pPlayer;
}

void GameDirector::Update()
{
	switch (m_eCurrentGameState)
	{
	case E_GameState::BeforeStart:
		// SPACEキー押下でプレイ中に
		if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
		{
			m_eCurrentGameState = E_GameState::Playing;
		}
		break;

	case E_GameState::Playing:

		break;

	case E_GameState::AfterGoal:

		break;
	}

	// プレイヤーとイベント発生ポイントの当たり判定
	KVector2 vPlayerPoint = m_pPlayer->GetCollisionPoint();
	int eCollisionNum = m_CollisionDetector.CollisionDetection(vPlayerPoint);
	// イベント発生
	switch (eCollisionNum)
	{
	case static_cast<int>(E_EventName::StartQTE):
		// QTE開始
		//m_QTEController.StartQTE(E_TrikDifficulty::Intermediate);
		m_eCurrentEventName = E_EventName::StartQTE;
		break;

	case static_cast<int>(E_EventName::EndQTE):
		// QTE終了
		//m_QTEController.FinishQTE();
		m_eCurrentEventName = E_EventName::EndQTE;
		break;

	case static_cast<int>(E_EventName::CourseGoal):
		// ゴール
		m_eCurrentEventName = E_EventName::CourseGoal;
		break;

	case static_cast<int>(E_EventName::ScoringGate):
		// 加点

		break;

	default:
		m_eCurrentEventName = E_EventName::None;
		break;
	}
}