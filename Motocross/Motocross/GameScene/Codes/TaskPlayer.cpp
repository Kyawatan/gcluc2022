#include "TaskPlayer.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <assert.h>
#include "ScrapTexQuad.h"
#include "BoxCollider.h"

#define PLAYER_START_POS_X	400
#define PLAYER_GOAL_POS_X	COURSE_LENGTH - 500

#define RUN_SPEED_NORMAL	300
#define RUN_SPEED_DAMAGE	200
#define RUN_SPEED_QTE		100

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
	Damage,
	JumpReady,
	JumpEnd,
	JumpRise1,
	JumpRise2,
	JumpStay,
	JumpDescent1,
	JumpDescent2,
};


TaskPlayer::TaskPlayer(GameDirector* pGameDirector)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::PlayerDefault), static_cast<int>(E_TaskLayerNum::Player))
	, m_pGameDirector(pGameDirector)
	, m_pSprite(NULL)
	, m_pAnim(NULL)
	, m_iAnimTexIndex()
	, m_eCurrentState(E_PlayerState::Normal)
	, m_iTrikNum(0)
	, m_pLaneManager(pGameDirector->GetLaneManagerInstance())
	, m_fAutoRunSpeed(RUN_SPEED_NORMAL)
	, m_eCurrentLane(E_CourseLane::Center)
	, m_eNextLane(E_CourseLane::Center)
	, m_fNextLaneDirection(0)
	, m_fNextLanePos(m_pLaneManager->GetLanePos(m_eNextLane))
	, m_canChangeLane(false)
	, m_vMovement()
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
	m_vMovement = KVector3{ 0, 0, 0 };
	m_pAnim->Update();

	if (m_pGameDirector->GetCurrentGameState() == E_GameState::Playing)
	{
		AutoRun();
		if (m_canChangeLane) ChangeLane();

		switch (m_eCurrentState)
		{
		case E_PlayerState::Normal:
			// W/Sキーでレーン移動
			if (!m_canChangeLane)
			{
				if (m_eCurrentLane != E_CourseLane::Left && GetpKeyState()->Down(E_KEY_NAME::W))
				{
					SetChangeLane(E_CourseChange::Left); // 1つ左のレーンへ移動する
				}
				else if (m_eCurrentLane != E_CourseLane::Right && GetpKeyState()->Down(E_KEY_NAME::S))
				{
					SetChangeLane(E_CourseChange::Right); // 1つ右のレーンへ移動する
				}
			}
			break;

		case E_PlayerState::Jump:
			Jump();
			break;

		case E_PlayerState::Damage:
			Damage();
			break;

		case E_PlayerState::Event:
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
		//KVector3{ -5, 10, 0 },
		//KVector3{ 0, 0, 0 },
		//KVector3{ 0.47, 0.15, 1 }
		KVector3{ 27, 15, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.2, 0.2, 1 }
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
	m_iAnimTexIndex[1] = dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(1280, 2048, L"GameScene/Images/player_jump.png", &texInfo);

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
	for (int i = 1; i < static_cast<int>(E_TrikName::Num); i++)
	//for (int i = 1; i < 2; i++)
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

const KVector3 TaskPlayer::GetMovement()
{
	return m_vMovement;
}

const KVector2 TaskPlayer::GetCollisionPoint()
{
	// 当たり判定を行う、プレイヤーの右下足元の座標
	KVector3 vSize = m_pCollider->GetSize();
	KVector3 vCenter = m_pCollider->GetCenter();
	KVector2 vPoint = KVector2{ vCenter.x + (vSize.x / 2.0f), vCenter.y - (vSize.y / 2.0f) };
	return vPoint;
}

/***********************************************************************************
	AutoRun
*************************************************************************************/

void TaskPlayer::AutoRun()
{
	assert(PLAYER_START_POS_X <= m_TaskTransform.GetPosition().x && m_TaskTransform.GetPosition().x <= PLAYER_GOAL_POS_X);
	
	// 停止中ならreturn
	if (m_fAutoRunSpeed == 0) return;

	// 移動
	m_vMovement = KVector3{ m_fAutoRunSpeed * GetDeltaTime(), 0, 0 };
	m_TaskTransform.Translate(m_vMovement);
	KVector3 vPos = m_TaskTransform.GetPosition();

	// ゴールに到達したら停止
	if (PLAYER_GOAL_POS_X <= vPos.x)
	{
		m_TaskTransform.SetPosition(KVector3{ PLAYER_GOAL_POS_X, vPos.y, vPos.z });
		m_fAutoRunSpeed = 0;
	}
}

/***********************************************************************************
	ChangeLane
*************************************************************************************/

void TaskPlayer::SetChangeLane(E_CourseChange eNextLane)
{
	float vCurrentPosZ = m_TaskTransform.GetPosition().z;
	if (eNextLane == E_CourseChange::Here)
	{
		// 今いるレーンに戻るとき、そのレーンの中央座標への方向
		if (m_eCurrentLane == m_pLaneManager->GetCurrentLane(vCurrentPosZ))
		{
			if (vCurrentPosZ < m_pLaneManager->GetLanePos(m_eCurrentLane))
			{
				m_fNextLaneDirection = static_cast<int>(E_CourseChange::Left);
			}
			else
			{
				m_fNextLaneDirection = static_cast<int>(E_CourseChange::Right);
			}
		}
	}
	else
	{
		m_fNextLaneDirection = static_cast<int>(eNextLane);
	}

	// 次のレーンを決定する
	m_eNextLane = static_cast<E_CourseLane>(static_cast<int>(m_eCurrentLane) + m_fNextLaneDirection);
	m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
	m_canChangeLane = true;

	// 次のレーンへの方向に応じたアニメーションセット
	if (eNextLane == E_CourseChange::Right)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
	}
	else
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
	}
}

void TaskPlayer::ChangeLane()
{
	// 移動
	float fMovementYZ = m_fNextLaneDirection * CHANGE_SPEED * GetDeltaTime();
	float fMovementX = fMovementYZ * cos((1.0f / 6.0f) * M_PI);
	m_TaskTransform.Translate(KVector3{ fMovementX, fMovementYZ, fMovementYZ });
	SetDrawNum(m_TaskTransform.GetPosition().z); // Draw番号更新

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
	SetDrawNum(m_TaskTransform.GetPosition().z); // Draw番号更新
	m_eCurrentLane = m_eNextLane;
	m_canChangeLane = false;

	// プレイヤーの状態に応じたアニメーションセット
	if (m_eCurrentState == E_PlayerState::Event)
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}
	else
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::Run));
	}
}

//void TaskPlayer::SetNextLane(E_CourseChange eNextLane)
//{
//	// 次のレーンを決定してレーン移動に遷移
//	m_eNextLane = static_cast<E_CourseLane>(static_cast<int>(m_eCurrentLane) + static_cast<int>(eNextLane));
//	m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
//	m_fNextLaneDirection = static_cast<int>(eNextLane);
//	m_eCurrentState = E_PlayerState::ChangeLane;
//	// アニメーションセット
//	if (static_cast<int>(eNextLane) < 0)
//	{
//		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
//	}
//	else
//	{
//		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
//	}
//}
//
//void TaskPlayer::SetPreviousLane(E_CourseLane eNextLane)
//{
//	// 元のレーン移動に遷移
//	m_eNextLane = eNextLane;
//	m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
//	m_eCurrentState = E_PlayerState::ChangeLane;
//	if (m_TaskTransform.GetPosition().z < m_fNextLanePos)
//	{
//		m_fNextLaneDirection = static_cast<int>(E_CourseChange::Left);
//		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
//	}
//	else
//	{
//		m_fNextLaneDirection = static_cast<int>(E_CourseChange::Right);
//		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
//	}
//		
//}

/***********************************************************************************
	Jump
*************************************************************************************/

void TaskPlayer::SetTrik(E_TrikName eTrikName)
{
	m_iTrikNum = static_cast<int>(eTrikName); // 今から行うトリックをセット
	m_eCurrentState = E_PlayerState::Jump;
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpReady), false, static_cast<int>(E_PlayerAnim::JumpRise1));
}

void TaskPlayer::Jump()
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
		m_eCurrentState = E_PlayerState::Normal;
		fTime = 0;
		iAnimCnt = 0;
	}
}

/***********************************************************************************
	Damage
*************************************************************************************/

void TaskPlayer::SetDamage()
{
	if (m_eCurrentState == E_PlayerState::Damage) return;

	m_canChangeLane = false; // レーン移動一時停止
	m_fAutoRunSpeed = RUN_SPEED_DAMAGE;
	m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Damage), false, static_cast<int>(E_PlayerAnim::Run));
	m_eCurrentState = E_PlayerState::Damage;
}

void TaskPlayer::Damage()
{
	if (m_pAnim->IsFinishAnimation())
	{
		m_fAutoRunSpeed = RUN_SPEED_NORMAL;
		m_eCurrentState = E_PlayerState::Normal;

		// レーン移動中なら再開
		if (m_TaskTransform.GetPosition().z != m_pLaneManager->GetLanePos(m_eCurrentLane))
		{
			m_canChangeLane = true;
		}
	}
}

/***********************************************************************************
	QTE
*************************************************************************************/


void TaskPlayer::SetEvent()
{
	m_fAutoRunSpeed = RUN_SPEED_QTE;
	m_eCurrentState = E_PlayerState::Event;

	// レーン移動中なら今いるレーンの中央へ戻る
	if (m_canChangeLane)
	{
		SetChangeLane(E_CourseChange::Here);
	}
	else
	{
		m_pAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}
}

void TaskPlayer::FinishEvent()
{
	m_fAutoRunSpeed = RUN_SPEED_NORMAL;
}
