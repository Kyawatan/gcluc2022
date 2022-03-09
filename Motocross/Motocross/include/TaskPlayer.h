#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "LaneManager.h"


enum class E_PlayerState
{
	Wait,		// �X�^�[�g�O�A��~���i����s�A�����O�i���Ȃ��j
	Normal,		// �R�[�X�𑖂��Ă���Ƃ��i���[���ύX�\�A�����O�i�j
	ChangeLane,	// ���[���ύX���i����s�A�����O�i�j
	UndoLane,	// ���[���ύX���i����s�A�����O�i�j
	Jump,		// �W�����v���i����s�A�����O�i�j
	Event,		// QTE���i����s�A�����O�i�j
	Damage,		// �_���[�W�i����s�A�����O�i�ɕω��H�j
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