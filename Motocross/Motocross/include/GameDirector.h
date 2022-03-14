#pragma once
#include "Definition.h"
#include "LaneManager.h"
#include "CollisionDetector.h"

#define JUMP_RAMP_NUM 3	// コース内のジャンプ台の数

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
	Beginner = 2,	// 初級
	Intermediate,	// 中級
	Advanced,		// 上級
};

struct QTE_INFORMATION
{
	int					iIndex;
	E_TrikDifficulty	eLeftDifficulty;
	E_TrikDifficulty	eCenterDifficulty;
	E_TrikDifficulty	eRightDifficulty;
};

class GameDirector
{
public:
	friend class SceneGame;

	GameDirector();
	~GameDirector();

	void SetQTEInformation(QTE_INFORMATION info);
	const int GetCurrentJumpRump();
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
	CollisionDetector	m_CollisionDetector; // レーンの奥行補正
	TaskPlayer*			m_pPlayer;
	TaskWindowEffect*	m_pWindowEffect;

	E_TrikDifficulty	m_eQTEDifficulty;
	QTE_INFORMATION		m_QTEInfo[JUMP_RAMP_NUM];
	int					m_iCurrentJumpRump;
	const float			m_fDepthCorrection;

	void SetQTEDifficulty();

	// SceneGameクラスだけが呼び出す
	void SetInstance(TaskPlayer* pPlayer, TaskWindowEffect* pWindowEffect);
	void Update();
};