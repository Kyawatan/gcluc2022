#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "LaneManager.h"

enum class E_PlayerState
{
	Wait,		// スタート前、停止中（操作不可、自動前進しない）
	Normal,		// コースを走っているとき（レーン変更可能、自動前進）
	ChangeLane,	// レーン変更中（操作不可、自動前進）
	Jump,		// ジャンプ中（操作不可、自動前進）
	Event,		// QTE中（操作不可、ゆっくり自動前進）
	FinishEvent,// QTE終了後（操作不可、自動前進）
	Damage,		// ダメージ（操作不可、自動前進に変化？）
};

enum class E_TrikName
{
	NormalJump = 1,	// ただのジャンプ
	Beginner1,		// 初級1
	Intermediate1,	// 中級1
	Advanced1,		// 上級1
	Num,
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
	void FinishEvent();
	void Jump(E_TrikName eTrikName);
	void Damage();
	KVector2 GetCollisionPoint();

private:
	QuadBase*			m_pSprite;
	KawataAnimation*	m_pAnim;
	int					m_iAnimTexIndex[2];
	E_PlayerState		m_eNowState;
	int					m_iTrikNum;

	LaneManager*	m_pLaneManager;
	E_CourseLane	m_eNowLane;
	E_CourseLane	m_eNextLane;
	float			m_fNextLaneDirection;
	float			m_fNextLanePos;

	KVector3		m_vCameraPos;

	void Init();
	void SetAnimation();

	void SetCameraMovement(KVector3 vec);
	bool CanAutoRun();
	void AutoRun();
	
	void ChangeLane();
	void SetNextLane(E_CourseChange eNextLane);
	void SetPreviousLane(E_CourseLane eNextLane);

	void Fall();

	void DamageMotion();
};