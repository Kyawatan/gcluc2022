#include "GameDirector.h"
#define _USE_MATH_DEFINES // math.hインクルード前に記述必要
#include <math.h>
#include <assert.h>
#include "TaskPlayer.h"
#include "TaskWindowEffect.h"
#include "LaneManager.h"
#include "CollisionDetector.h"


GameDirector::GameDirector()
	: m_eCurrentGameState(E_GameState::BeforeStart)
	, m_eCurrentEventName(E_EventName::None)
	, m_LaneManager()
	, m_CollisionDetector()
	, m_pPlayer(NULL)
	, m_pWindowEffect(NULL)
	, m_eQTEDifficulty()
	, m_QTEInfo()
	, m_iCurrentJumpRump(0)
	, m_fDepthCorrection(m_LaneManager.GetLaneInterval()* cos((1.0f / 6.0f)* M_PI))
{

}

GameDirector::~GameDirector()
{

}

void GameDirector::SetQTEInformation(QTE_INFORMATION info)
{
	m_QTEInfo[info.iIndex] = info;
}

const int GameDirector::GetCurrentJumpRump()
{
	return m_iCurrentJumpRump;
}

const E_GameState GameDirector::GetCurrentGameState()
{
	return m_eCurrentGameState;
}

const E_EventName GameDirector::GetCurrentEventName()
{
	return m_eCurrentEventName;
}

const float GameDirector::GetDepthCorrection()
{
	return m_fDepthCorrection;
}
//
//
TaskPlayer* GameDirector::GetPlayerInstance()
{
	return m_pPlayer;
}

TaskWindowEffect* GameDirector::GetWindowEffectInstance()
{
	return m_pWindowEffect;
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
//
//
const E_TrikDifficulty GameDirector::GetQTEDifficulty()
{
	return m_eQTEDifficulty;
}

void GameDirector::SetQTEDifficulty()
{
	// プレイヤーの現在地点によって実行するQTE難度を決定
	switch (m_LaneManager.GetCurrentLane(m_pPlayer->m_TaskTransform.GetPosition().z))
	{
	case E_CourseLane::Right:
		m_eQTEDifficulty = m_QTEInfo[m_iCurrentJumpRump].eRightDifficulty;
		break;

	case E_CourseLane::Center:
		m_eQTEDifficulty = m_QTEInfo[m_iCurrentJumpRump].eCenterDifficulty;
		break;

	case E_CourseLane::Left:
		m_eQTEDifficulty = m_QTEInfo[m_iCurrentJumpRump].eLeftDifficulty;
		break;
	}
}

void GameDirector::SetInstance(TaskPlayer* pPlayer, TaskWindowEffect* pWindowEffect)
{
	assert(m_pPlayer == NULL && pPlayer != NULL);
	assert(m_pWindowEffect == NULL && pWindowEffect != NULL);
	m_pPlayer = pPlayer;
	m_pWindowEffect = pWindowEffect;
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
		m_iCurrentJumpRump++;
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