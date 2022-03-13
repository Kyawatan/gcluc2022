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
	, m_QTEDifficulty()
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

E_TrikDifficulty const GameDirector::GetQTEDifficulty()
{
	return m_QTEDifficulty;
}

void GameDirector::SetQTEDifficulty()
{
	switch (m_LaneManager.GetCurrentLane(m_pPlayer->m_TaskTransform.GetPosition().z))
	{
	case E_CourseLane::Right:
		m_QTEDifficulty = E_TrikDifficulty::Beginner;
		break;

	case E_CourseLane::Center:
		m_QTEDifficulty = E_TrikDifficulty::Intermediate;
		break;

	case E_CourseLane::Left:
		m_QTEDifficulty = E_TrikDifficulty::Advanced;
		break;
	}
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
	case static_cast<int>(E_EventName::QTEStart):
		// QTE開始
		SetQTEDifficulty();
		m_eCurrentEventName = E_EventName::QTEStart;
		break;

	case static_cast<int>(E_EventName::QTEEnd):
		// QTE終了
		m_eCurrentEventName = E_EventName::QTEEnd;
		break;

	case static_cast<int>(E_EventName::CourseGoal):
		// ゴール
		m_eCurrentGameState = E_GameState::AfterGoal;
		break;

	case static_cast<int>(E_EventName::ScoringGate):
		// 加点

		break;

	default:
		m_eCurrentEventName = E_EventName::None;
		break;
	}
}