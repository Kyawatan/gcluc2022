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

class GameDirector
{
public:
	friend class SceneGame;

	GameDirector();
	~GameDirector();

	E_GameState const GetCurrentGameState();
	E_EventName const GetCurrentEventName();

	TaskPlayer* GetPlayerInstance();
	LaneManager* GetLaneManagerInstance();
	CollisionDetector* GetCollisionDetectorInstance();

private:
	E_GameState	m_eCurrentGameState;
	E_EventName	m_eCurrentEventName;
	LaneManager			m_LaneManager;
	CollisionDetector	m_CollisionDetector;
	TaskPlayer* m_pPlayer;

	// SceneGameƒNƒ‰ƒX‚¾‚¯‚ªŒÄ‚Ño‚·
	void SetPlayerInstance(TaskPlayer* pPlayer);
	void Update();
};