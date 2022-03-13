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
	Beginner = 2,	// ����
	Intermediate,	// ����
	Advanced,		// �㋉
};

class GameDirector
{
public:
	friend class SceneGame;

	GameDirector();
	~GameDirector();

	const E_GameState GetCurrentGameState();
	const E_EventName GetCurrentEventName();
	const float GetDepthCorrection();
	const E_TrikDifficulty GetQTEDifficulty();

	TaskPlayer* GetPlayerInstance();
	LaneManager* GetLaneManagerInstance();
	CollisionDetector* GetCollisionDetectorInstance();

private:
	E_GameState	m_eCurrentGameState;
	E_EventName	m_eCurrentEventName;
	LaneManager			m_LaneManager;
	CollisionDetector	m_CollisionDetector; // ���[���̉��s�␳
	TaskPlayer*			m_pPlayer;

	E_TrikDifficulty	m_QTEDifficulty;
	const float			m_fDepthCorrection;

	void SetQTEDifficulty();

	// SceneGame�N���X�������Ăяo��
	void SetPlayerInstance(TaskPlayer* pPlayer);
	void Update();
};