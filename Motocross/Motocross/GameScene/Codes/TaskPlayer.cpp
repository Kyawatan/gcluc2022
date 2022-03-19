#include "TaskPlayer.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include <assert.h>
#include "ScrapTexQuad.h"
#include "BoxCollider.h"

#define PLAYER_START_POS_X	400
#define PLAYER_GOAL_POS_X	COURSE_LENGTH - 500
#define SHADOW_INITIAL_POS	KVector3{ 0, -10, 0 }

#define RUN_SPEED_NORMAL	300
#define RUN_SPEED_DAMAGE	200
#define RUN_SPEED_QTE		100

#define CHANGE_SPEED		300
#define JUMP_VY0			367.75f
#define GRAVITY				-294.0f

#define JUMP_NUM			18		// player_jump.png�̃`�b�v�̊J�n�ԍ�
#define TRIK_ANIM_INDEX(i)	5 * i	// �e�g���b�N�A�j���[�V�����̃C���f�b�N�X

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

enum class E_ShadowAnim
{
	Normal,
	Jump,
};

TaskPlayer::TaskPlayer(GameDirector* pGameDirector, ScoreController* pScoreController)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::PlayerDefault), static_cast<int>(E_TaskLayerNum::Player))
	, m_pGameDirector(pGameDirector)
	, m_pScoreController(pScoreController)
	, m_pSprite(NULL)
	, m_pShadow(NULL)
	, m_pPlayerAnim(NULL)
	, m_pShadowAnim(NULL)
	, m_iAnimTexIndex()
	, m_eCurrentState(E_PlayerState::Normal)
	, m_iTrikNum(0)
	, m_eTrikPoint()
	, m_pLaneManager(pGameDirector->GetLaneManagerInstance())
	, m_fAutoRunSpeed(RUN_SPEED_NORMAL)
	, m_fJumpStartPosX(0)
	, m_eCurrentLane(E_CourseLane::Center)
	, m_eNextLane(E_CourseLane::Center)
	, m_fNextLaneDirection(0)
	, m_fNextLanePos(m_pLaneManager->GetLanePos(m_eNextLane))
	, m_canChangeLane(false)
	, m_vMovement()
{
	// �^�X�N�̏����g�����X�t�H�[��
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
	delete m_pShadow;
	delete m_pPlayerAnim;
}

void TaskPlayer::Update()
{
	m_pPlayerAnim->Update();
	m_pShadowAnim->Update();
	m_vMovement = KVector3{ 0, 0, 0 };
	if (CanAutoRun()) AutoRun(); // �����O�i
	if (m_canChangeLane) ChangeLane(); //�@���[���ړ�

	if (m_pGameDirector->GetCurrentGameState() == E_GameState::Playing)
	{
		switch (m_eCurrentState)
		{
		case E_PlayerState::Normal:
			// W/S�L�[�Ń��[���ړ��J�n
			if (!m_canChangeLane)
			{
				if (m_eCurrentLane != E_CourseLane::Left && GetpKeyState()->Down(E_KEY_NAME::W))
				{
					SetChangeLane(E_CourseChange::Left); // 1���̃��[���ֈړ�����
				}
				else if (m_eCurrentLane != E_CourseLane::Right && GetpKeyState()->Down(E_KEY_NAME::S))
				{
					SetChangeLane(E_CourseChange::Right); // 1�E�̃��[���ֈړ�����
				}
			}
			break;

		case E_PlayerState::Jump:
			Jump();
			break;

		case E_PlayerState::Damage:
			Damage();
			break;
		}
	}

	switch (m_pGameDirector->GetCurrentEventName())
	{
	case E_EventName::QTEStart:
		SetEvent();
		break;

	case E_EventName::QTEEnd:
		FinishEvent(false);
		SetJump();
		break;
	
	case E_EventName::ScoringGate:
		m_pScoreController->AddPoint(E_Point::PointGate);
		break;
	}
}

void TaskPlayer::Draw()
{
	// �e�\��
	dynamic_cast<ScrapTexQuad*>(m_pShadow)->Draw(0, m_pShadowAnim->GetCurrentIndex());
	// �v���C���[�\��
	int iIndex = m_pPlayerAnim->GetCurrentIndex();
	int iAnimTexIndex = 0; // �e�N�X�`���؂�ւ�
	if (JUMP_NUM <= iIndex) iAnimTexIndex = 1;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(m_iAnimTexIndex[iAnimTexIndex],iIndex);

	m_pCollider->DrawLine();
}

void TaskPlayer::Init()
{
	// �X�v���C�g�쐬
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_pSprite->m_transform.SetPosition(KVector3{ 0, 76, 0 }); // �C���X�g�̑������^�X�N�̍��W�ɂ��炷
	// �e�N�X�`�����Z�b�g�iindex:0�`17�j
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 6;
	texInfo.iTipColumn = 3;
	m_iAnimTexIndex[0] = dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(768, 1536, L"Resources/img/player_base.png", &texInfo);
	// �e�N�X�`�����Z�b�g�iindex:18�`27�j
	texInfo.iTipWidth = 256;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 8;
	texInfo.iTipColumn = 5;
	m_iAnimTexIndex[1] = dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(1280, 2048, L"Resources/img/player_jump.png", &texInfo);
	// �A�j���[�V�����쐬
	m_pPlayerAnim = new KawataAnimation();
	SetPlayerAnimation();
	m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Run), true, NULL);

	// �v���C���[�̑����ɉe�쐬
	m_pShadow = new ScrapTexQuad(&m_TaskTransform);
	m_pShadow->m_transform.SetPosition(SHADOW_INITIAL_POS);
	// �e�N�X�`�����Z�b�g
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 32;
	texInfo.iTipRow = 2;
	texInfo.iTipColumn = 3;
	m_iAnimTexIndex[0] = dynamic_cast<ScrapTexQuad*>(m_pShadow)->SetTexture(384, 64, L"Resources/img/player_shadow.png", &texInfo);
	// �A�j���[�V�����쐬
	m_pShadowAnim = new KawataAnimation();
	SetShadowAnimation();
	m_pShadowAnim->SetAnimation(static_cast<int>(E_ShadowAnim::Normal), true, NULL);

	// �R���C�_�\�쐬
	m_pCollider = new BoxCollider(&m_TaskTransform, KVector3{ 256, 256, 0 });
	m_pCollider->m_transform.SetTransform(
		//KVector3{ -5, 10, 0 },
		//KVector3{ 0, 0, 0 },
		//KVector3{ 0.47, 0.15, 1 }
		KVector3{ 27, 15, 0 },
		KVector3{ 0, 0, 0 },
		KVector3{ 0.2, 0.2, 1 }
	);
	//m_pCollider->AddLine(); // �⏕���\��
}

void TaskPlayer::SetPlayerAnimation()
{
	int index = static_cast<int>(E_PlayerAnim::Run);
	const int runNum = 3;
	int runOrders[runNum] = { 0, 1, 2 };
	float runSpeeds[runNum] = { 0.1f, 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, runNum, runOrders, runSpeeds);
	
	index = static_cast<int>(E_PlayerAnim::Crouch);
	const int crouchNum = 2;
	int crouchOrders[crouchNum] = { 3, 4 };
	float crouchSpeeds[crouchNum] = { 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, crouchNum, crouchOrders, crouchSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveRight);
	const int rightNum = 2;
	int rightOrders[rightNum] = { 5, 6 };
	float rightSpeeds[rightNum] = { 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, rightNum, rightOrders, rightSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveLeft);
	const int leftNum = 2;
	int leftOrders[leftNum] = { 7, 8 };
	float leftSpeeds[leftNum] = { 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, leftNum, leftOrders, leftSpeeds);

	index = static_cast<int>(E_PlayerAnim::MoveKusshon);
	const int rMoveEndNum = 1;
	int rMoveEndOrders[rMoveEndNum] = { 4 };
	float rMoveEndSpeeds[rMoveEndNum] = { 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, rMoveEndNum, rMoveEndOrders, rMoveEndSpeeds);
	
	index = static_cast<int>(E_PlayerAnim::JumpReady);
	const int rReadyNum = 3;
	int rReadyOrders[rReadyNum] = { 9, 10, 11 };
	float rReadySpeeds[rReadyNum] = { 0.16f, 0.16f, 0.16f };
	m_pPlayerAnim->SetAnimationInfo(index, rReadyNum, rReadyOrders, rReadySpeeds);

	index = static_cast<int>(E_PlayerAnim::JumpEnd);
	const int rJumpEndNum = 4;
	int rJumpEndOrders[rJumpEndNum] = { 12, 13, 14, 15 };
	float rJumpEndSpeeds[rJumpEndNum] = { 0.1f, 0.1f, 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, rJumpEndNum, rJumpEndOrders, rJumpEndSpeeds);

	index = static_cast<int>(E_PlayerAnim::Damage);
	const int rJDamageNum = 10;
	int rDamageOrders[rJDamageNum] = { 9, 10, 11, 20, 22, 25, 12, 13, 14, 15 };
	float rDamageSpeeds[rJDamageNum] = { 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f };
	m_pPlayerAnim->SetAnimationInfo(index, rJDamageNum, rDamageOrders, rDamageSpeeds);

	// �e�g���b�N�̃A�j���[�V����
	int iNum = JUMP_NUM;
	index = static_cast<int>(E_PlayerAnim::JumpRise1);
	for (int i = static_cast<int>(E_TrikName::NormalJump); i < static_cast<int>(E_TrikName::Num); i++)
	{
		// JumpRise1
		const int rRise1Num = 2;
		int rRise1Orders[rRise1Num] = { iNum, iNum += 1 };
		float rRise1Speeds[rRise1Num] = { 0.1f, 0.1f };
		m_pPlayerAnim->SetAnimationInfo(index, rRise1Num, rRise1Orders, rRise1Speeds);

		// JumpRise2
		index++;
		const int rRise2Num = 2;
		int rRise2Orders[rRise2Num] = { iNum += 1, iNum += 1 };
		float rRise2Speeds[rRise2Num] = { 0.1f, 0.1f };
		m_pPlayerAnim->SetAnimationInfo(index, rRise2Num, rRise2Orders, rRise2Speeds);

		// JumpStay
		index++;
		const int rStayNum = 3;
		int rStayOrders[rStayNum] = { iNum += 1, iNum += 1, iNum += 1 };
		float rStaySpeeds[rStayNum] = { 0.1f, 0.1f, 0.1f };
		m_pPlayerAnim->SetAnimationInfo(index, rStayNum, rStayOrders, rStaySpeeds);

		// JumpDescent1
		index++;
		const int rDescentNum1 = 2;
		int rDescentOrders1[rDescentNum1] = { iNum += 1, iNum += 1 };
		float rDescentSpeeds1[rDescentNum1] = { 0.1f, 0.1f };
		m_pPlayerAnim->SetAnimationInfo(index, rDescentNum1, rDescentOrders1, rDescentSpeeds1);

		// JumpDescent2
		index++;
		const int rDescentNum2 = 1;
		int rDescentOrders2[rDescentNum2] = { iNum += 1 };
		float rDescentSpeeds2[rDescentNum2] = { 0.1f };
		m_pPlayerAnim->SetAnimationInfo(index, rDescentNum2, rDescentOrders2, rDescentSpeeds2);

		iNum++;
		index++;
	}
}

void TaskPlayer::SetShadowAnimation()
{
	int index = static_cast<int>(E_ShadowAnim::Normal);
	const int iNum = 3;
	int iNormalOrders[iNum] = { 0, 1, 2 };
	float iSpeeds[iNum] = { 0.1f, 0.1f, 0.1f };
	m_pShadowAnim->SetAnimationInfo(index, iNum, iNormalOrders, iSpeeds);

	index = static_cast<int>(E_ShadowAnim::Jump);
	int iJumpOrders[iNum] = { 3, 4, 5 };
	m_pShadowAnim->SetAnimationInfo(index, iNum, iJumpOrders, iSpeeds);
}

const KVector3 TaskPlayer::GetMovement()
{
	return m_vMovement;
}

const KVector2 TaskPlayer::GetCollisionPoint()
{
	// �����蔻����s���A�v���C���[�̉E�������̍��W
	KVector3 vSize = m_pCollider->GetSize();
	KVector3 vCenter = m_pCollider->GetCenter();
	KVector2 vPoint = KVector2{ vCenter.x + (vSize.x / 2.0f), vCenter.y - (vSize.y / 2.0f) };
	return vPoint;
}

/***********************************************************************************
	AutoRun
*************************************************************************************/

bool TaskPlayer::CanAutoRun()
{
	// ��~���ƊJ�n�O�͎����O�iOFF
	if (m_fAutoRunSpeed == 0
		|| m_pGameDirector->GetCurrentGameState() == E_GameState::BeforeStart)
	{
		return false;
	}
	return true;
}

void TaskPlayer::AutoRun()
{
	assert(PLAYER_START_POS_X <= m_TaskTransform.GetPosition().x && m_TaskTransform.GetPosition().x <= PLAYER_GOAL_POS_X);

	// �ړ�
	m_vMovement = KVector3{ m_fAutoRunSpeed * GetDeltaTime(), 0, 0 };
	m_TaskTransform.Translate(m_vMovement);
	KVector3 vPos = m_TaskTransform.GetPosition();

	// �S�[���ɓ��B�������~
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
		// �܂������郌�[������o�Ă��Ȃ���΁A���̃��[���i�t�����j�֖߂�
		if (m_eCurrentLane == m_pLaneManager->GetCurrentLane(vCurrentPosZ))
		{
			m_fNextLaneDirection *= -1;
			m_eNextLane = m_eCurrentLane;
			m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
		}
		// �������̃��[���͈̔͂ɂ���΁A���̂܂܎��̃��[���ւ̈ړ��𑱂���
	}
	else
	{
		// ���̃��[�������肵�Ĉړ����J�n
		m_fNextLaneDirection = static_cast<int>(eNextLane);
		m_eNextLane = static_cast<E_CourseLane>(static_cast<int>(m_eCurrentLane) + m_fNextLaneDirection);
		m_fNextLanePos = m_pLaneManager->GetLanePos(m_eNextLane);
		m_canChangeLane = true;
	}

	// ���̃��[���ւ̕����ɉ������A�j���[�V�����Z�b�g
	if (eNextLane == E_CourseChange::Right)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveRight));
	}
	else
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::MoveLeft));
	}
}

void TaskPlayer::ChangeLane()
{
	// �ړ�
	float fMovementYZ = m_fNextLaneDirection * CHANGE_SPEED * GetDeltaTime();
	float fMovementX = fMovementYZ * cos((1.0f / 6.0f) * M_PI);
	m_TaskTransform.Translate(KVector3{ fMovementX, fMovementYZ, fMovementYZ });
	SetDrawNum(m_TaskTransform.GetPosition().z); // Draw�ԍ��X�V

	// ���̃��[���܂ŗ��Ă��Ȃ����return
	if (m_fNextLaneDirection < 0 && m_fNextLanePos <= m_TaskTransform.GetPosition().z)
	{
		return; // ���̃��[���i1�E�j�܂ŗ��Ă��Ȃ����return
	}
	else if (0 < m_fNextLaneDirection && m_TaskTransform.GetPosition().z <= m_fNextLanePos)
	{
		return; // ���̃��[���i1���j�܂ŗ��Ă��Ȃ����return
	}

	// ���̃��[���܂ŗ����烌�[���ړ��I��
	m_TaskTransform.SetPosition(KVector3{ m_TaskTransform.GetPosition().x, m_fNextLanePos, m_fNextLanePos });
	SetDrawNum(m_TaskTransform.GetPosition().z); // Draw�ԍ��X�V
	m_eCurrentLane = m_eNextLane;
	m_canChangeLane = false;

	// �v���C���[�̏�Ԃɉ������A�j���[�V�����Z�b�g
	if (m_eCurrentState == E_PlayerState::Event)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}
	else
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::MoveKusshon), false, static_cast<int>(E_PlayerAnim::Run));
	}
}

/***********************************************************************************
	Jump
*************************************************************************************/

void TaskPlayer::SetJump()
{
	m_eCurrentState = E_PlayerState::Jump;
	m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpReady), false,
		static_cast<int>(E_PlayerAnim::JumpRise1) + TRIK_ANIM_INDEX(m_iTrikNum));
	m_pShadowAnim->SetAnimation(static_cast<int>(E_ShadowAnim::Jump), true, NULL);
	m_fJumpStartPosX = m_TaskTransform.GetPosition().x;
	// �g���b�N�����߂��Ă����瓾�_
	if (m_iTrikNum != 0)
	{
		m_pScoreController->AddPoint(m_eTrikPoint);
	}
}

void TaskPlayer::Jump()
{
	assert(0 <= m_iTrikNum && m_iTrikNum < static_cast<int>(E_TrikName::Num));

	// �W�����v�͖�2.5�b, ������230px
	const float fMaxHeight = 230.0f;
	static float fTime = 0;
	fTime += GetDeltaTime();
	float fHeight = (JUMP_VY0 * fTime) + (0.5f * GRAVITY * fTime * fTime);
	KVector3 vPos = m_TaskTransform.GetPosition();
	m_TaskTransform.SetPosition(KVector3{vPos.x, vPos.z + fHeight, vPos.z});

	// �e�͒n�ʂ�
	m_pShadow->m_transform.SetPosition(KVector3{ 0, -fHeight, 0 });
	static float fKobuHeight = 0;
	float fDistance = m_TaskTransform.GetPosition().x - m_fJumpStartPosX;
	if (fDistance < 70 || (550 <= fDistance && fDistance < 620))
	{
		// �R�u����
		if (fKobuHeight < 100)
		{
			fKobuHeight += GetDeltaTime() * 200;
		}
		else
		{
			fKobuHeight = 100;
		}
		m_pShadow->m_transform.Translate(KVector3{ 0, fKobuHeight, 0 });
	}
	else
	{
		// �R�u�����
		if (0 < fKobuHeight)
		{
			fKobuHeight -= GetDeltaTime() * 200;
			m_pShadow->m_transform.Translate(KVector3{ 0, fKobuHeight, 0 });
		}
		else
		{
			fKobuHeight = 0;
		}
	}

	// �A�j���[�V�����؂�ւ�
	static int iAnimCnt = 0;
	if (fMaxHeight * 0.4f <= fHeight && iAnimCnt == 0)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpRise2) + TRIK_ANIM_INDEX(m_iTrikNum), true, NULL);
		iAnimCnt++;
	}
	else if (fMaxHeight * 0.9f <= fHeight && iAnimCnt == 1)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpStay) + TRIK_ANIM_INDEX(m_iTrikNum), true, NULL);
		iAnimCnt++;
	}
	else if (fHeight <= fMaxHeight * 0.9f && iAnimCnt == 2)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpDescent1) + TRIK_ANIM_INDEX(m_iTrikNum), true, NULL);
		iAnimCnt++;
	}
	else if (fHeight <= fMaxHeight * 0.5f && iAnimCnt == 3)
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpDescent2) + TRIK_ANIM_INDEX(m_iTrikNum), true, NULL);
		iAnimCnt++;
	}

	// ���n
	if (vPos.y < vPos.z)
	{
		m_TaskTransform.SetPosition(KVector3{ vPos.x, vPos.z, vPos.z });
		m_pShadow->m_transform.SetPosition(SHADOW_INITIAL_POS);
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpEnd), false, static_cast<int>(E_PlayerAnim::Run));
		m_pShadowAnim->SetAnimation(static_cast<int>(E_ShadowAnim::Normal), true, NULL);
		//m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::JumpEnd), true, NULL);
		m_eCurrentState = E_PlayerState::Normal;
		fTime = 0;
		fKobuHeight = 0;
		iAnimCnt = 0;
	}
}

/***********************************************************************************
	Damage
*************************************************************************************/

void TaskPlayer::SetDamage()
{
	if (m_eCurrentState == E_PlayerState::Damage) return;

	// ���[���ړ����Ȃ�_���[�W��ɍ����郌�[���̒����֖߂�
	if (m_canChangeLane)
	{
		SetChangeLane(E_CourseChange::Here);
		m_canChangeLane = false; // ���[���ړ��ꎞ��~
	}
	m_fAutoRunSpeed = RUN_SPEED_DAMAGE;
	m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Damage), false, static_cast<int>(E_PlayerAnim::Run));
	m_eCurrentState = E_PlayerState::Damage;
	m_pScoreController->AddPoint(E_Point::DamageRock); // ���_
}

void TaskPlayer::Damage()
{
	if (m_pPlayerAnim->IsFinishAnimation())
	{
		m_fAutoRunSpeed = RUN_SPEED_NORMAL;
		m_eCurrentState = E_PlayerState::Normal;
		m_canChangeLane = true; // ���[���ړ��ĊJ
	}
}

/***********************************************************************************
	QTE
*************************************************************************************/

void TaskPlayer::SetEvent()
{
	m_fAutoRunSpeed = RUN_SPEED_QTE;
	m_eCurrentState = E_PlayerState::Event;
	// �f�t�H���g�ł����̃W�����v���Z�b�g����
	m_iTrikNum = static_cast<int>(E_TrikName::NormalJump);

	// ���[���ړ����Ȃ獡���郌�[���̒����֖߂�
	if (m_canChangeLane)
	{
		SetChangeLane(E_CourseChange::Here);
	}
	else
	{
		m_pPlayerAnim->SetAnimation(static_cast<int>(E_PlayerAnim::Crouch), true, NULL);
	}
}

void TaskPlayer::FinishEvent(bool isSuccessful)
{
	m_fAutoRunSpeed = RUN_SPEED_NORMAL;

	// QTE�����Ȃ�g���b�N���Z�b�g����
	if (isSuccessful)
	{
		switch (m_pGameDirector->GetQTEDifficulty())
		{
		case E_TrikDifficulty::Beginner:
			m_iTrikNum = static_cast<int>(E_TrikName::Beginner1);
			m_eTrikPoint = E_Point::TrikBiginner;
			break;

		case E_TrikDifficulty::Intermediate:
			m_iTrikNum = static_cast<int>(E_TrikName::Intermediate1);
			m_eTrikPoint = E_Point::TrikIntermediate;
			break;
		
		case E_TrikDifficulty::Advanced:
			m_iTrikNum = static_cast<int>(E_TrikName::Advanced1);
			m_eTrikPoint = E_Point::TrikAdvanced;
			break;
		}
	}
}
