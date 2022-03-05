#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include "TaskBackGround.h"
#include "TaskCorn.h"

#define JUMP_RAMP_PERIOD	2000	// ジャンプ台の間隔
#define CORN_SHIFT			40		// コーンをレーンの真ん中よりもずらして置く

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
	// コーン生成
	float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 6.0f) * M_PI); // 差分
	float posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + CORN_SHIFT;
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX + fDifference, posYZ, posYZ });
	posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - CORN_SHIFT;
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - fDifference, posYZ, posYZ });
}