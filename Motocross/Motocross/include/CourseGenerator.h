#pragma once
#include "Definition.h"
#include "LaneManager.h"

#define JUMP_RAMP_NUM	2		// コース内のジャンプ台の数


class CourseGenerator
{
public:
	CourseGenerator(LaneManager* pLaneManager);
	~CourseGenerator();

private:
	LaneManager* m_pLaneManager;

	void SetJumpRamp(float posX);
};