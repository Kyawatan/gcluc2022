#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "LaneManager.h"

#define PLAYER_START_POS_X		-2600
#define PLAYER_GOAL_POS_X		1800

enum class E_PlayerState
{
	Wait,
	Normal,
	ChangeLane,
	Event, // QTE��
	Damage,
};


class TaskPlayer : public TaskBase
{
public:
	friend class PlayerController;

	TaskPlayer();
	~TaskPlayer();

	void Update() override;
	void Draw() override;

	bool IsGoal();
	const KVector3 GetCameraMovement();

private:
	QuadBase* m_pSprite;
	KawataAnimation* m_pAnim;
	E_PlayerState m_eNowState;
	LaneManager m_LaneManager;
	E_CourseLane m_eNowLane;
	E_CourseLane m_eNextLane;
	float m_fNextLaneDirection;
	float m_fNextLanePos;
	KVector3 m_vCameraPos;

	void SetAnimation();
	void SetCameraMovement(KVector3 vec);

	bool CanAutoRun();
	void AutoRun();
	bool CanChangeLane();
	void ChangeLane();
	void UndoLane();
	void SetNextLane(E_CourseChange eNextLane);
};