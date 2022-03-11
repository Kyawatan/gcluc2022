#include "TaskPlayer.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <assert.h>
#include "ScrapTexQuad.h"
#include "BoxCollider.h"

#define PLAYER_START_POS_X	400
#define PLAYER_GOAL_POS_X	COURSE_LENGTH - 300
#define RUN_SPEED			300
#define DAMAGE_SPEED		200
#define CHANGE_SPEED		300
#define JUMP_VY0			367.75f
#define GRAVITY				-294.0f

#define JUMP_NUM 18	// player_jump.pngのチップの開始番号

enum class E_PlayerAnim
{
	Run,
	Crouch,
	MoveRight,
	MoveLeft,
	MoveKusshon,
	JumpReady,
	JumpRise1,
	JumpRise2,
	JumpStay,
	JumpDescent1,
	JumpDescent2,
	JumpEnd,
	Damage,
};


TaskPlayer::TaskPlayer(LaneManager* pLaneManager)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::PlayerDefault), static_cast<int>(E_TaskLayerNum::Player))
	, m_pSprite(NULL)
	, m_pAnim(NULL)
	, m_iAnimTexIndex()
	, m_eNowState(E_PlayerState::Wait)
	, m_iTrikNum(0)
	, m_pLaneManager(pLaneManager)
	, m_eNowLane(E_CourseLane::Center)
	, m_eNextLane(E_CourseLane::Center)
	, m_fNextLaneDirection(0)
	, m_fNextLanePos(pLaneManager->GetLanePos(m_eNextLane))
	, m_vCameraPos()
{
	// タスクの初期トランスフォーム
	m_TaskTransform.SetTransform
	(
		KVector3{ PLAYER_START_POS_X, m_pLaneManager->GetLanePos(E_CourseLane::Center), m_pLaneManager->GetLanePos(E_CourseLane::Center) }, // pos
		KVector3{ 0, 0, 0 }, // rot
		KVector3{ 1, 1, 1 }	// scale
	);
	Init();
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

	if (!IsGoal())
	{
		switch (m_eNowState)
		{
		case E_PlayerState::Wait:
			if (GetpKeyState()->Down(E_KEY_NAME::SPACE))
			{
				m_eNowState = E_PlayerState::Normal; // ゲームスタート
			}
			break;

		case E_PlayerState::Normal:
			if (m_eNowLane != E_CourseLane::Left && GetpKeyState()->Down(E_KEY_NAME::W))
			{
				SetNextLane(E_CourseChange::Left);	// 1つ左のレーンへ移動する
			}
			else if (m_eNowLane != E_CourseLane::Right && GetpKeyState()->Down(E_KEY_NAME::S))
			{
				SetNextLane(E_CourseChange::Right); // 1つ右のレーンへ移動する
			}
			break;

		case E_PlayerState::ChangeLane:
		case E_PlayerState::UndoLane:
			ChangeLane(); // レーン移動
			break;

		case E_PlayerState::Jump:
			Fall();	// 自由落下
			break;

		case E_PlayerState::Event:
			break;

		case E_PlayerState::Damage:
			DamageMotion();
			break;

		default:
			break;
		}
	}
}

void TaskPlayer::Draw()
{
	int iIndex = m_pAnim->GetCurrentIndex();
	int iAnimTexIndex = 0; // テクスチャ切り替え
	if (JUMP_NUM <= iIndex) iAnimTexIndex = 1;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(m_iAnimTexIndex[iAnimTexIndex],iIndex);

	m_pCollider->DrawLine();
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

void TaskPlayer::Init()
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 76, 0 }); // イラストの足元をタスクの座標にずらす

	// アニメーション作成
	m_pAnim = new KawataAnimation();
	SetAnimation();
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Run), true, NULL);

	// コライダ―作成
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 256, 256, 0 });
	m_pCollider->m_transform.SetTransform(
		KVector3{ -5, 10, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.47, 0.15, 1 }
	);
	//m_pCollider->AddLine(); // 補助線表示
}

void TaskPlayer::SetAnimation()
{
	// テクスチャをセット（index:0～17）
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 6;
	texInfo.iTipColumn = 3;
	m_iAnimTexIndex[0] = dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(768, 1536, L"GameScene/Images/player_base.png", &texInfo);

	// テクスチャをセット（index:18～27）
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 2;
	texInfo.iTipColumn = 5;
	m_iAnimTexIndex[1] = dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(1280, 512, L"GameScene/Images/player_jump.png", &texInfo);

	//　アニメーションをセット
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

	index = static_cast<int>(E_PlayerAnim::MoveKusshon);
	const int rMoveEndNum = 1;
	int rMoveEndOrders[rMoveEndNum] = { 4 };
	float rMoveEndSpeeds[rMoveEndNum] = { 0.1f };
	m_pAnim->SetAnimationInfo(index, rMoveEndNum, rMoveEndOrders, rMoveEndSpeeds);
	
	index = static_cast<int>(E_PlayerAnim::JumpReady);
	const int rReadyNum = 3;
	int rReadyOrders[rReadyNum] = { 9, 10, 11 };
	float rReadySpeeds[rReadyNum] = { 0.16f, 0.16f, 0.16f };
	m_pAnim->SetAnimationInfo(index, rReadyNum, rReadyOrders, rReadySpeeds);

	index = static_cast<int>(E_PlayerAnim::JumpEnd);
	const int rJumpEndNum = 4;
	int rJumpEndOrders[rJumpEndNum] = { 12, 13, 14, 15 };
	float rJumpEndSpeeds[rJumpEndNum] = { 0.1f, 0.1f, 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, rJumpEndNum, rJumpEndOrders, rJumpEndSpeeds);

	index = static_cast<int>(E_PlayerAnim::Damage);
	const int rJDamageNum = 10;
	int rDamageOrders[rJDamageNum] = { 9, 10, 11, 20, 22, 25, 12, 13, 14, 15 };
	float rDamageSpeeds[rJDamageNum] = { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
	m_pAnim->SetAnimationInfo(index, rJDamageNum, rDamageOrders, rDamageSpeeds);

	// 各トリックのアニメーション
	//for (int i = 1; i < static_cast<int>(E_TrikName::Num); i++)
	for (int i = 1; i < 2; i++)
	{
		int iNum = JUMP_NUM * i;

		index = static_cast<int>(E_PlayerAnim::JumpRise1) * i;
		const int rRise1Num = 2;
		int rRise1Orders[rRise1Num] = { iNum, iNum += 1 };
		float rRise1Speeds[rRise1Num] = { 0.1f, 0.1f };
		m_pAnim->SetAnimationInfo(index, rRise1Num, rRise1Orders, rRise1Speeds);

		index = static_cast<int>(E_PlayerAnim::JumpRise2) * i;
		const int rRise2Num = 2;
		int rRise2Orders[rRise2Num] = { iNum += 1, iNum += 1 };
		float rRise2Speeds[rRise2Num] = { 0.1f, 0.1f };
		m_pAnim->SetAnimationInfo(index, rRise2Num, rRise2Orders, rRise2Speeds);

		index = static_cast<int>(E_PlayerAnim::JumpStay) * i;
		const int rStayNum = 3;
		int rStayOrders[rStayNum] = { iNum += 1, iNum += 1, iNum += 1 };
		float rStaySpeeds[rStayNum] = { 0.1f, 0.1f, 0.1f };
		m_pAnim->SetAnimationInfo(index, rStayNum, rStayOrders, rStaySpeeds);

		index = static_cast<int>(E_PlayerAnim::JumpDescent1) * i;
		const int rDescentNum1 = 2;
		int rDescentOrders1[rDescentNum1] = { iNum += 1, iNum += 1 };
		float rDescentSpeeds1[rDescentNum1] = { 0.1f, 0.1f };
		m_pAnim->SetAnimationInfo(index, rDescentNum1, rDescentOrders1, rDescentSpeeds1);

		index = static_cast<int>(E_PlayerAnim::JumpDescent2) * i;
		const int rDescentNum2 = 1;
		int rDescentOrders2[rDescentNum2] = { iNum += 1 };
		float rDescentSpeeds2[rDescentNum2] = { 0.1f };
		m_pAnim->SetAnimationInfo(index, rDescentNum2, rDescentOrders2, rDescentSpeeds2);
	}
}

/***********************************************************************************
	AutoRun
*************************************************************************************/

const KVector3 TaskPlayer::GetCameraMovement()
{
	return m_vCameraPos;
}

void TaskPlayer::SetCameraMovement(KVector3 vec)
{
	m_vCameraPos = vec;
}

bool TaskPlayer::CanAutoRun()
{
	if (IsGoal()) return false;
	if (m_eNowState == E_PlayerState::Wait) return false;

	return true;
}

void TaskPlayer::AutoRun()
{
	float fMovement;
	if (m_eNowState == E_PlayerState::Damage)
	{
		fMovement = DAMAGE_SPEED * GetDeltaTime();
	}
	else
	{
		fMovement = RUN_SPEED * GetDeltaTime();
	}

	m_TaskTransform.Translate(KVector3{ fMovement, 0, 0 });
	SetCameraMovement(KVector3{ fMovement, 0, 0 });
	if (IsGoal())
	{
		KVector3 vPos = m_TaskTransform.GetPosition();
		m_TaskTransform.SetPosition(KVector3{ PLAYER_GOAL_POS_X, vPos.y, vPos.z });
	}
	assert(PLAYER_START_POS_X <= m_TaskTransform.GetPosition().x < PLAYER_GOAL_POS_X);
}

/***********************************************************************************
	ChangeLane
*************************************************************************************/

void TaskPlayer::ChangeLane()
{
	// 移動
	float fMovementYZ = m_fNextLaneDirection * CHANGE_SPEED * GetDeltaTime();
	float fMovementX = fMovementYZ * cos((1.0f / 6.0f) * M_PI);
	m_TaskTransform.Translate(KVector3{ fMovementX, fMovementYZ, fMovementYZ });
	SetDrawNum(-m_TaskTransform.GetPosition().z); // Draw番号更新

	// 次のレーンまで来ていなければreturn
	if (m_fNextLaneDirection < 0 && m_fNextLanePos <= m_TaskTransform.GetPosition().z)
	{
		return; // 次のレーン（1つ右）まで来ていなければreturn
	}
	else if (0 < m_fNextLaneDirection && m_TaskTransform.GetPosition().z <= m_fNextLanePos)
	{
		return; // 次のレーン（1つ左）まで来ていなければreturn
	}

	// 次のレーンまで来たらレーン移動終了
	m_TaskTransform.SetPosition(KVector3{ m_TaskTransform.GetPosition().x, m_fNextLanePos, m_fNextLanePos });
	SetDrawNum(-m_TaskTransform.GetPosition().z); // Draw番号更新
	m_eNowLane = m_eNextLane;

	if (m_eNowState == E_PlayerState::ChangeLane)
	{
		m_eNowState = E_PlayerState::Normal;
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::Run));
	}
	else if (m_eNowState == E_PlayerState::UndoLane)
	{
		m_eNowState = E_PlayerState::Event;
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}

}

void TaskPlayer::SetNextLane(E_CourseChange eNextLane)
{
	// 次のレーンを決定してレーン移動に遷移
	m_eNextLane = static_cast<E_CourseLane>(static_cast<int>(m_eNowLane) + static_cast<int>(eNextLane));
	m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
	m_fNextLaneDirection = static_cast<int>(eNextLane);
	m_eNowState = E_PlayerState::ChangeLane;
	// アニメーションセット
	if (static_cast<int>(eNextLane) < 0)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
	}
	else
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
	}
}

void TaskPlayer::SetPreviousLane(E_CourseLane eNextLane)
{
	// 元のレーン移動に遷移
	m_eNextLane = eNextLane;
	m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
	m_eNowState = E_PlayerState::UndoLane;
	if (m_TaskTransform.GetPosition().z < m_fNextLanePos)
	{
		m_fNextLaneDirection = static_cast<int>(E_CourseChange::Left);
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
	}
	else
	{
		m_fNextLaneDirection = static_cast<int>(E_CourseChange::Right);
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
	}
		
}

/***********************************************************************************
	Jump
*************************************************************************************/

void TaskPlayer::Jump(E_TrikName eTrikName)
{
	m_iTrikNum = static_cast<int>(eTrikName); // 今から行うトリックをセット
	m_eNowState = E_PlayerState::Jump;
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpReady), false, static_cast<int>(E_PlayerAnim::JumpRise1));
}

void TaskPlayer::Fall()
{
	// ジャンプは約2.5秒, 高さ約230px
	const float fMaxHeight = 230.0f;
	static float fTime = 0;
	fTime += GetDeltaTime();
	float fHeight = (JUMP_VY0 * fTime) + (0.5f * GRAVITY * fTime * fTime);
	KVector3 vPos = m_TaskTransform.GetPosition();
	m_TaskTransform.SetPosition(KVector3{vPos.x, vPos.z + fHeight, vPos.z});

	// アニメーション切り替え
	static int iAnimCnt = 0;
	if (fMaxHeight * 0.5f <= fHeight && iAnimCnt == 0)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpRise2) * m_iTrikNum, true, NULL);
		iAnimCnt++;
	}
	else if (fMaxHeight * 0.9f <= fHeight && iAnimCnt == 1)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpStay) * m_iTrikNum, true, NULL);
		iAnimCnt++;
	}
	else if (fHeight <= fMaxHeight * 0.9f && iAnimCnt == 2)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpDescent1) * m_iTrikNum, true, NULL);
		iAnimCnt++;
	}
	else if (fHeight <= fMaxHeight * 0.5f && iAnimCnt == 3)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpDescent2) * m_iTrikNum, true, NULL);
		iAnimCnt++;
	}

	// 着地
	if (vPos.y < vPos.z)
	{
		m_TaskTransform.SetPosition(KVector3{ vPos.x, vPos.z, vPos.z });
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpEnd), false, static_cast<int>(E_PlayerAnim::Run));
		//m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpEnd), true, NULL);
		m_eNowState = E_PlayerState::Normal;
		fTime = 0;
		iAnimCnt = 0;
	}
}

void TaskPlayer::SetEvent()
{
	// 今いるレーンの中央へ戻る
	float currentPos = m_TaskTransform.GetPosition().z;
	if (currentPos != m_pLaneManager->GetLanePos(m_eNowLane))
	{
		SetPreviousLane(m_pLaneManager->GetCurrentLane(currentPos));
	}
	else
	{
		m_eNowState = E_PlayerState::Event;
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}
}

KVector2 TaskPlayer::GetCollisionPoint()
{
	// 当たり判定を行う、プレイヤーの右下足元の座標
	KVector3 vSize = m_pCollider->GetSize();
	KVector3 vCenter = m_pCollider->GetCenter();
	KVector2 vPoint = KVector2{ vCenter.x + (vSize.x / 2.0f), vCenter.y - (vSize.y / 2.0f) };
	return vPoint;
}

/***********************************************************************************
	Damage
*************************************************************************************/

void TaskPlayer::Damage()
{
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Damage), false, static_cast<int>(E_PlayerAnim::Run));
	m_eNowState = E_PlayerState::Damage;
}

void TaskPlayer::DamageMotion()
{
	if (m_pAnim->IsFinishAnimation())
	{
		m_eNowState = E_PlayerState::Normal;
	}
}