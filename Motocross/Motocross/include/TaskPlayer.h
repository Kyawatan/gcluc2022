#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

enum class E_PlayerState
{
	Normal,		// コースを走っているとき（レーン変更可能、自動前進）
	Jump,		// ジャンプ中（操作不可、自動前進）
	Damage,		// ダメージ（操作不可、ゆっくり自動前進）
	Event,		// QTE中（操作不可、ゆっくり自動前進）
};

enum class E_TrikName
{
	NormalJump,		// ただのジャンプ
	Beginner1,		// 初級1
	Intermediate1,	// 中級1
	Advanced1,		// 上級1
	Num,
};


class TaskPlayer : public TaskBase
{
public:
	TaskPlayer(GameDirector* pGameDirector);
	~TaskPlayer();

	void Update() override;
	void Draw() override;

	const KVector3 GetMovement();
	const KVector2 GetCollisionPoint();

	void FinishEvent(bool isSuccessful);
	void SetDamage();

private:
	GameDirector*		m_pGameDirector;
	QuadBase*			m_pSprite;
	KawataAnimation*	m_pAnim;
	int					m_iAnimTexIndex[2];
	E_PlayerState		m_eCurrentState;
	int					m_iTrikNum;

	float			m_fAutoRunSpeed;

	LaneManager*	m_pLaneManager;
	E_CourseLane	m_eCurrentLane;
	E_CourseLane	m_eNextLane;
	float			m_fNextLaneDirection;
	float			m_fNextLanePos;
	bool			m_canChangeLane;

	KVector3		m_vMovement;

	void Init();
	void SetAnimation();

	bool CanAutoRun();
	void AutoRun();
	
	void SetChangeLane(E_CourseChange eNextCourse);
	void ChangeLane();

	void SetJump();
	void Jump();
	void Damage();

	void SetEvent();
};