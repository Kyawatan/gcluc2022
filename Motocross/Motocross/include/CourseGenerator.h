#pragma once
#include "Definition.h"
#include "LaneManager.h"

#define JUMP_RAMP_NUM	2		// �R�[�X���̃W�����v��̐�


class CourseGenerator
{
public:
	CourseGenerator(LaneManager* pLaneManager);
	~CourseGenerator();

private:
	LaneManager* m_pLaneManager;

	void SetJumpRamp(float posX);
};