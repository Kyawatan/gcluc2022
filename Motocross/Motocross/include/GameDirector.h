#pragma once
#include "Definition.h"
#include "LaneManager.h"
#include "CollisionDetector.h"

class TaskPlayer;
class TaskWindowEffect;

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
	TaskWindowEffect* GetWindowEffectInstance();
	LaneManager* GetLaneManagerInstance();
	CollisionDetector* GetCollisionDetectorInstance();

private:
	E_GameState	m_eCurrentGameState;
	E_EventName	m_eCurrentEventName;
	LaneManager			m_LaneManager;
	CollisionDetector	m_CollisionDetector; // ���[���̉��s�␳
	TaskPlayer*			m_pPlayer;
	TaskWindowEffect*	m_pWindowEffect;

	E_TrikDifficulty	m_QTEDifficulty;
	const float			m_fDepthCorrection;

	void SetQTEDifficulty();

	// SceneGame�N���X�������Ăяo��
	void SetInstance(TaskPlayer* pPlayer, TaskWindowEffect* pWindowEffect);
	void Update();
};