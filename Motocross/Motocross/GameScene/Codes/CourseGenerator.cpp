#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskStealFrame.h"
#include "TaskCorn.h"
#include "TaskKobu.h"
#include "TaskRock.h"

#define JUMP_RAMP_NUM		3		// コース内のジャンプ台の数
#define JUMP_RAMP_PERIOD	4000	// ジャンプ台の間隔
#define KOBU_INTERVAL		550		// ジャンプ始めとジャンプ終わりのコブの間隔
#define ROCK_PERIOD			300		// 岩の間隔


CourseGenerator::CourseGenerator(LaneManager* pLaneManager, CollisionDetector* pCollisionDetector)
	: m_pLaneManager(pLaneManager)
	, m_pCollisionDetector(pCollisionDetector)
	, m_fDifference(m_pLaneManager->GetLaneInterval() * cos((1.0f / 5.0f) * M_PI))
	, m_fCourseLength(0)
	, m_fLaneLeftEndPos(m_pLaneManager->GetLanePos(E_CourseLane::Left) + 50)
	, m_fLaneRightEndPos(m_pLaneManager->GetLanePos(E_CourseLane::Right) - 50)
{
	InitCourse();
}

CourseGenerator::~CourseGenerator()
{

}

void CourseGenerator::InitCourse()
{
	// 背景生成
	SetBackGround();
	// スタート生成
	SetStart();
	// ゴール生成
	SetGoal();
	// ジャンプ台生成（4000, 8000, 12000）
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(JUMP_RAMP_PERIOD * (i + 1));
	}
	// ジャンプ台のない場所に各オブジェクトを生成（1000～3000、5000～7000、9000～11000）
	const int iStartPos = 1000;
	const int iLastPos = iStartPos + 2000;
	const int iInterval = 4000;
	for (int i = 0; i < 3; i++)
	{
		// 岩生成
		for (int j = iStartPos; j <= iLastPos; j += ROCK_PERIOD)
		{
			SetRock(j + ( i * iInterval));
		}
		// 小加点ゲート生成
		//SetGate(800);
	}
}

void CourseGenerator::SetBackGround()
{
	TaskBackGround* back = new TaskBackGround();
	m_fCourseLength = back->GetCourseLength();
}

void CourseGenerator::SetStart()
{
	const float fStartPos = 250;

	// スタート鉄骨生成
	TaskBase* frameLeft = new TaskStealFrame(KVector3{ fStartPos, 0, 0 }, E_FrameName::Left);
	TaskBase* frameRight = new TaskStealFrame(KVector3{ fStartPos, 0, 0 }, E_FrameName::Base);
}

void CourseGenerator::SetGoal()
{
	const float fGoalPos = m_fCourseLength - 1000;
	
	// ゴール鉄骨生成
	TaskBase* frameLeft = new TaskStealFrame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Left);
	TaskBase* frameRight = new TaskStealFrame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Goal);

	// ゴール位置を登録
	KVector2 vStart = KVector2{ fGoalPos - m_fDifference, m_fLaneRightEndPos };
	KVector2 vEnd = KVector2{ fGoalPos + m_fDifference, m_fLaneLeftEndPos };
	COLLISION cs;
	cs.eName = E_CollisionName::CourseGoal;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// デバッグ用補助線表示
	//dynamic_cast<TaskStealFrame*>(frameRight)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetJumpRamp(float fPosX)
{
	// コーンとコブの距離（4段階、QTEレベル）をランダムで決定
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 300, 400, 500, 600 }; // 3秒、4秒、5秒、6秒
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // 乱数生成

	// 左右コーン生成
	const float fLaneEdgeOffset = 40; // レーン中央から端にずらす量
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fLaneEdgeOffset;
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fLaneEdgeOffset;
	TaskBase* cornLeft = new TaskCorn(KVector3{ fPosX - iDistances[fRand] + m_fDifference, fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ fPosX - iDistances[fRand] - m_fDifference, fRightPosYZ, fRightPosYZ });
	// コブ生成
	TaskBase* kobuStart = new TaskKobu(KVector3{ fPosX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ fPosX + KOBU_INTERVAL, 0, 0 });

	KVector2 vStart;
	KVector2 vEnd;
	COLLISION cs;

	// QTE開始位置を登録
	vStart = KVector2{ cornRight->m_TaskTransform.GetPosition().x, cornRight->m_TaskTransform.GetPosition().y };
	vEnd = KVector2{ cornLeft->m_TaskTransform.GetPosition().x, cornLeft->m_TaskTransform.GetPosition().y };
	cs.eName = E_CollisionName::StartQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// デバッグ用補助線表示
	//dynamic_cast<TaskCorn*>(cornRight)->DrawCollisionLine(vStart, vEnd);

	// QTE終了位置を登録
	const float fAdjustment = 90; // コブの手前に調整
	vStart = KVector2{ fPosX - fAdjustment - m_fDifference, m_fLaneRightEndPos };
	vEnd = KVector2{ fPosX - fAdjustment + m_fDifference, m_fLaneLeftEndPos };
	cs.eName = E_CollisionName::EndQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// デバッグ用補助線表示
	//dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetGate(float fPosX)
{
	float fLanePos = m_pLaneManager->GetLanePos(E_CourseLane::Center);

	// 左右コーン生成
	const float fLaneEdgeOffset = 50; // レーン中央から端にずらす量

	float fLeftPosYZ = fLanePos + fLaneEdgeOffset;
	float fRightPosYZ = fLanePos - fLaneEdgeOffset;
	TaskBase* cornLeft = new TaskCorn(KVector3{ fPosX + (m_fDifference * 0.35f), fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ fPosX - (m_fDifference * 0.35f), fRightPosYZ, fRightPosYZ });

	// 加点位置を登録
	KVector2 vStart = KVector2{ cornRight->m_TaskTransform.GetPosition().x, cornRight->m_TaskTransform.GetPosition().y };
	KVector2 vEnd = KVector2{ cornLeft->m_TaskTransform.GetPosition().x, cornLeft->m_TaskTransform.GetPosition().y };
	COLLISION cs;
	cs.eName = E_CollisionName::ScoringGate;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	//m_pCollisionDetector->SetCollision(cs);
	// デバッグ用補助線表示
	dynamic_cast<TaskCorn*>(cornRight)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetRock(float fPosX)
{
	// 乱数生成
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, 2);
	int fRandNum = (int)distr(eng);	// 同じX座標の岩の個数
	int fRandLane = (int)distr(eng); // 1個目の岩のレーン

	// 同じX座標に0～2個生成
	for (int i = 0; i < fRandNum; i++)
	{
		// 2個目の岩は1個目の岩のレーンと重ならないようにレーンを1つずらして（1 2 -）（- 1 2）（2 - 1）の3パターンができる
		float fPosYZ = m_pLaneManager->GetLanePos(static_cast<E_CourseLane>((fRandLane + i) % 3));
		if (m_pLaneManager->GetCurrentLane(fPosYZ) == E_CourseLane::Left)
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX + m_fDifference, fPosYZ, fPosYZ });
		}
		else if (m_pLaneManager->GetCurrentLane(fPosYZ) == E_CourseLane::Right)
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX - m_fDifference, fPosYZ, fPosYZ });
		}
		else
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX, fPosYZ, fPosYZ });
		}
	}
}