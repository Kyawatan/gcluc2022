#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "LaneManager.h"

enum class E_PlayerState
{
	Wait,		// �X�^�[�g�O�A��~���i����s�A�����O�i���Ȃ��j
	Normal,		// �R�[�X�𑖂��Ă���Ƃ��i���[���ύX�\�A�����O�i�j
	ChangeLane,	// ���[���ύX���i����s�A�����O�i�j
	Jump,		// �W�����v���i����s�A�����O�i�j
	Event,		// QTE���i����s�A������莩���O�i�j
	FinishEvent,// QTE�I����i����s�A�����O�i�j
	Damage,		// �_���[�W�i����s�A�����O�i�ɕω��H�j
};

enum class E_TrikName
{
	NormalJump = 1,	// �����̃W�����v
	Beginner1,		// ����1
	Intermediate1,	// ����1
	Advanced1,		// �㋉1
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