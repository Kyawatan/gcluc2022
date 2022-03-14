#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

enum class E_PlayerState
{
	Normal,		// �R�[�X�𑖂��Ă���Ƃ��i���[���ύX�\�A�����O�i�j
	Jump,		// �W�����v���i����s�A�����O�i�j
	Damage,		// �_���[�W�i����s�A������莩���O�i�j
	Event,		// QTE���i����s�A������莩���O�i�j
};

enum class E_TrikName
{
	NormalJump,		// �����̃W�����v
	Beginner1,		// ����1
	Intermediate1,	// ����1
	Advanced1,		// �㋉1
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