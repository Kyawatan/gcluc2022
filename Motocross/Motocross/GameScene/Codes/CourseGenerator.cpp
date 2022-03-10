#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskCorn.h"
#include "TaskKobu.h"

#define JUMP_RAMP_NUM		2		// コース内のジャンプ台の数
#define JUMP_RAMP_PERIOD	2000	// ジャンプ台の間隔
#define KOBU_INTERVAL		550		// ジャンプ始めとジャンプ終わりのコブの間隔


CourseGenerator::CourseGenerator(LaneManager* pLaneManager, CollisionDetector* pCollisionDetector)
	: m_pLaneManager(pLaneManager)
	, m_pCollisionDetector(pCollisionDetector)
{
	InitCourse();
}

CourseGenerator::~CourseGenerator()
{

}

void CourseGenerator::InitCourse()
{
	// 背景生成
	TaskBackGround* back = new TaskBackGround();
	// ジャンプ台生成
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(JUMP_RAMP_PERIOD * (i + 1));
	}
}

void CourseGenerator::SetJumpRamp(float posX)
{
	const float fLaneEdgeOffset = 40; // レーン中央から端にずらす量
	const float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 5.0f) * M_PI); // 差分
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fLaneEdgeOffset;
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fLaneEdgeOffset;
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 600, 900, 1050, 1200 } // 2秒、3秒、3.5秒、4秒
	;
	// コーンとコブの距離（4段階、QTEレベル）をランダムで決定
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // 乱数生成

	// 左右コーン生成
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + fDifference, fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - fDifference, fRightPosYZ, fRightPosYZ });
	// コブ生成
	TaskBase* kobuStart = new TaskKobu(KVector3{ posX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ posX + KOBU_INTERVAL, 0, 0 });

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
	vStart = KVector2{ posX - fDifference - fAdjustment, fRightPosYZ };
	vEnd = KVector2{ posX + fDifference - fAdjustment, fLeftPosYZ };
	cs.eName = E_CollisionName::EndQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// デバッグ用補助線表示
	//dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine(vStart, vEnd);
}