#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "LaneManager.h"


enum class E_PlayerState
{
	Wait,		// スタート前、停止中（操作不可、自動前進しない）
	Normal,		// コースを走っているとき（レーン変更可能、自動前進）
	ChangeLane,	// レーン変更中（操作不可、自動前進）
	UndoLane,	// レーン変更中（操作不可、自動前進）
	Jump,		// ジャンプ中（操作不可、自動前進）
	Event,		// QTE中（操作不可、自動前進）
	Damage,		// ダメージ（操作不可、自動前進に変化？）
};


class TaskPlayer : public TaskBase
{
public:
	friend class PlayerController;

	TaskPlayer(LaneManager* pLaneManager);
	~TaskPlayer();

	void Update() override;
	void Draw() override;

	bool IsGoal();
	const KVector3 GetCameraMovement();
	void SetEvent();
	void Jump();
	KVector2 GetCollisionPoint();

private:
	QuadBase* m_pSprite;
	KawataAnimation* m_pAnim;
	int m_iAnimTexIndex[2];
	E_PlayerState m_eNowState;

	LaneManager* m_pLaneManager;
	E_CourseLane m_eNowLane;
	E_CourseLane m_eNextLane;
	float m_fNextLaneDirection;
	float m_fNextLanePos;

	float m_fGround;

	KVector3 m_vCameraPos;

	void Init();
	void SetAnimation();

	void SetCameraMovement(KVector3 vec);
	bool CanAutoRun();
	void AutoRun();
	
	void ChangeLane();
	void SetNextLane(E_CourseChange eNextLane);
	void SetUndoLane(E_CourseLane eNextLane);

	void Fall();
};