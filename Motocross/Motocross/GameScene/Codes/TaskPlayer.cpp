#include "TaskPlayer.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <assert.h>
#include "ScrapTexQuad.h"

#define RUN_SPEED		250
#define CHANGE_SPEED	300

enum class E_PlayerAnim
{
	Run,
	Crouch,
	MoveRight,
	MoveLeft,
	MoveEnd,
	JumpReady,
};

TaskPlayer::TaskPlayer()
	: TaskBase(0, 10, NULL)
	, m_pSprite(NULL)
	, m_pAnim(NULL)
	, m_eNowState(E_PlayerState::Normal)
	, m_LaneManager()
	, m_eNowLane(E_CourseLane::Center)
	, m_eNextLane(E_CourseLane::Center)
	, m_fNextLaneDirection(0)
	, m_fNextLanePos(m_LaneManager.GetLanePos(m_eNextLane))
	, m_vCameraPos()
{
	// タスクの初期トランスフォーム
	m_TaskTransform.SetTransform
	(
		KVector3{ PLAYER_START_POS_X, m_LaneManager.GetLanePos(E_CourseLane::Center), m_LaneManager.GetLanePos(E_CourseLane::Center) }, // pos
		KVector3{ 0, 0, 0 }, // rot
		KVector3{ 1, 1, 1 }	// scale
	);

	// スプライト作成
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 4;
	texInfo.iTipColumn = 3;
	m_pSprite = new ScrapTexQuad(&m_TaskTransform, 768, 1024, L"GameScene/Images/player_normal.png", &texInfo);
	m_TaskTransform.AddChild(m_pSprite);

	// アニメーション作成
	m_pAnim = new KawataAnimation();
	SetAnimation();
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Run), true, NULL);
}

TaskPlayer::~TaskPlayer()
{
	delete m_pSprite;
	delete m_pAnim;
}

void TaskPlayer::Update()
{
	m_pAnim->Update();

	if (CanAutoRun())
	{
		AutoRun();
	}

	if (CanChangeLane())
	{
		if (m_eNowState == E_PlayerState::Normal)
		{
			if (m_eNowLane != E_CourseLane::Left && GetpKeyState()->Down(E_KEY_NAME::W))
			{
				// 1つ左のレーンへ移動する
				SetNextLane(E_CourseChange::Left);
			}
			else if (m_eNowLane != E_CourseLane::Right && GetpKeyState()->Down(E_KEY_NAME::S))
			{
				// 1つ右のレーンへ移動する
				SetNextLane(E_CourseChange::Right);
			}
		}
		else if (m_eNowState == E_PlayerState::ChangeLane)
		{
			ChangeLane();
		}
	}
}

void TaskPlayer::Draw()
{
	m_pSprite->Draw(m_pAnim->GetCurrentIndex());
}

bool TaskPlayer::IsGoal()
{
	if (PLAYER_GOAL_POS_X <= m_TaskTransform.GetPosition().x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const KVector3 TaskPlayer::GetCameraMovement()
{
	return m_vCameraPos;
}

void TaskPlayer::SetAnimation()
{
	int index = static_cast<int>(E_PlayerAnim::Run);
	const int runNum = 3;
	int runOrders[runNum] = { 0, 1, 2 };
	float runSpeeds[runNum] = { 0.1f, 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, runNum, runOrders, runSpeeds);
	
	index = static_cast<int>(E_PlayerAnim::Crouch);
	const int crouchNum = 2;
	int crouchOrders[crouchNum] = { 3, 4 };
	float crouchSpeeds[crouchNum] = { 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, crouchNum, crouchOrders, crouchSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveRight);
	const int rightNum = 2;
	int rightOrders[rightNum] = { 5, 6 };
	float rightSpeeds[rightNum] = { 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, rightNum, rightOrders, rightSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveLeft);
	const int leftNum = 2;
	int leftOrders[leftNum] = { 7, 8 };
	float leftSpeeds[leftNum] = { 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, leftNum, leftOrders, leftSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveEnd);
	const int rEndNum = 1;
	int rEndOrders[rEndNum] = { 4 };
	float rEndSpeeds[rEndNum] = { 0.1f };
	m_pAnim->SetAnimationInfo(index, rEndNum, rEndOrders, rEndSpeeds);
}

void TaskPlayer::SetCameraMovement(KVector3 vec)
{
	m_vCameraPos = vec;
}

bool TaskPlayer::CanAutoRun()
{
	if (!IsGoal())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TaskPlayer::AutoRun()
{
	m_TaskTransform.Translate(KVector3{ RUN_SPEED * GetDeltaTime(), 0, 0 });
	SetCameraMovement(KVector3{ RUN_SPEED * GetDeltaTime(), 0, 0 });
	if (IsGoal())
	{
		KVector3 vPos = m_TaskTransform.GetPosition();
		m_TaskTransform.SetPosition(KVector3{ PLAYER_GOAL_POS_X, vPos.y, vPos.z });
	}
	assert(PLAYER_START_POS_X <= m_TaskTransform.GetPosition().x < PLAYER_GOAL_POS_X);
}

bool TaskPlayer::CanChangeLane()
{
	if (!IsGoal())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void TaskPlayer::ChangeLane()
{
	// 移動
	float fMovementYZ = m_fNextLaneDirection * CHANGE_SPEED * GetDeltaTime();
	float fMovementX = fMovementYZ * cos((1.0f / 6.0f) * M_PI);
	m_TaskTransform.Translate(KVector3{ fMovementX, fMovementYZ, fMovementYZ });

	if (m_fNextLaneDirection < 0 && m_fNextLanePos <= m_TaskTransform.GetPosition().z)
	{
		return;
	}
	else if (0 < m_fNextLaneDirection && m_TaskTransform.GetPosition().z <= m_fNextLanePos)
	{
		return;
	}
	// 次のレーンまできたらレーン移動終了
	m_TaskTransform.SetPosition(KVector3{ m_TaskTransform.GetPosition().x, m_fNextLanePos, m_fNextLanePos });
	m_eNowState = E_PlayerState::Normal;
	m_eNowLane = m_eNextLane;
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveEnd), false, static_cast<int>(E_PlayerAnim::Run));
}

void TaskPlayer::SetNextLane(E_CourseChange eNextLane)
{
	// 次のレーンを決定してレーン移動に遷移
	m_eNextLane = static_cast<E_CourseLane>(static_cast<int>(m_eNowLane) + static_cast<int>(eNextLane));
	m_fNextLanePos = m_LaneManager.GetLanePos(m_eNextLane);
	m_fNextLaneDirection = static_cast<int>(eNextLane);
	if (static_cast<int>(eNextLane) < 0)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), false, static_cast<int>(E_PlayerAnim::MoveRight));
	}
	else
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), false, static_cast<int>(E_PlayerAnim::MoveLeft));
	}
	m_eNowState = E_PlayerState::ChangeLane;
}