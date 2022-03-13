#pragma once
#include "Definition.h"
#include "LaneManager.h"
#include "CollisionDetector.h"

class TaskPlayer;

enum class E_GameState
{
	BeforeStart,
	Playing,
	AfterGoal,
};

enum class E_TrikDifficulty
{
	Beginner = 2,	// 初級
	Intermediate,	// 中級
	Advanced,		// 上級
};

class GameDirector
{
public:
	friend class SceneGame;

	GameDirector();
	~GameDirector();

	E_GameState const GetCurrentGameState();
	E_EventName const GetCurrentEventName();

	E_TrikDifficulty const GetQTEDifficulty();

	TaskPlayer* GetPlayerInstance();
	LaneManager* GetLaneManagerInstance();
	CollisionDetector* GetCollisionDetectorInstance();

private:
	E_GameState	m_eCurrentGameState;
	E_EventName	m_eCurrentEventName;
	LaneManager			m_LaneManager;
	CollisionDetector	m_CollisionDetector;
	TaskPlayer* m_pPlayer;

	E_TrikDifficulty m_QTEDifficulty;

	void SetQTEDifficulty();

	// SceneGameクラスだけが呼び出す
	void SetPlayerInstance(TaskPlayer* pPlayer);
	void Update();
};