#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskCorn.h"
#include "TaskKobu.h"

#define JUMP_RAMP_PERIOD	2000	// ジャンプ台の間隔
#define KOBU_INTERVAL		800		// ジャンプ始めとジャンプ終わりのコブの間隔


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
	// コブ生成
	TaskBase* kobuStart = new TaskKobu(KVector3{ posX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ posX + KOBU_INTERVAL, 0, 0 });

	// コーンとコブの距離のレベル（4段階）をランダムで決定
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 400, 500, 600, 700 };

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // 乱数生成

	// コーン生成
	const float fCornShift = 40; // コーンがレーンの端にくるように
	float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 6.0f) * M_PI); // 差分
	float posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fCornShift;
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + fDifference, posYZ, posYZ });
	posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fCornShift;
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - fDifference, posYZ, posYZ });
}