#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskCorn.h"
#include "TaskKobu.h"

#define JUMP_RAMP_PERIOD	2000	// ジャンプ台の間隔
#define KOBU_INTERVAL		600		// ジャンプ始めとジャンプ終わりのコブの間隔


CourseGenerator::CourseGenerator(LaneManager* pLaneManager)
	: m_pLaneManager(pLaneManager)
{
	// 背景生成
	TaskBackGround* back = new TaskBackGround();
	// ジャンプ台生成
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(JUMP_RAMP_PERIOD * (i + 1));
	}
}

CourseGenerator::~CourseGenerator()
{

}

void CourseGenerator::SetJumpRamp(float posX)
{
	const float fLaneEdgeOffset = 40; // レーン中央から端にずらす量
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fLaneEdgeOffset;
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fLaneEdgeOffset;
	const float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 6.0f) * M_PI); // 差分

	// コブ生成
	TaskBase* kobuStart = new TaskKobu(KVector3{ posX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ posX + KOBU_INTERVAL, 0, 0 });
	// QTE終了位置
	const float fAdjustment = 150; // コブの手前に調整
	KVector2 vStart = KVector2{ posX - fDifference - fAdjustment, fRightPosYZ };
	KVector2 vEnd = KVector2{ posX + fDifference - fAdjustment, fLeftPosYZ };
	dynamic_cast<TaskKobu*>(kobuStart)->SetCollisionDetector(vStart, vEnd);
	dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine();

	// コーンとコブの距離のレベル（4段階）をランダムで決定
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 400, 500, 600, 700 };
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // 乱数生成

	// 左右コーン生成
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + fDifference, fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - fDifference, fRightPosYZ, fRightPosYZ });
	// 右側のコーンにプレイヤーと当たり判定させる（QTE発生位置）
	KVector2 vCollisionEnd = KVector2{ cornLeft->m_TaskTransform.GetPosition().x, cornLeft->m_TaskTransform.GetPosition().y };
	dynamic_cast<TaskCorn*>(cornRight)->SetCollisionDetector(vCollisionEnd);
	dynamic_cast<TaskCorn*>(cornRight)->DrawCollisionLine();

}