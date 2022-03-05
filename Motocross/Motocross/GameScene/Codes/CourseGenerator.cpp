#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include "TaskBackGround.h"
#include "TaskCorn.h"

#define JUMP_RAMP_PERIOD	2000	// �W�����v��̊Ԋu
#define CORN_SHIFT			40		// �R�[�������[���̐^�񒆂������炵�Ēu��

CourseGenerator::CourseGenerator(LaneManager* pLaneManager)
	: m_pLaneManager(pLaneManager)
{
	// �w�i����
	TaskBackGround* back = new TaskBackGround();
	// �W�����v�䐶��
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
	// �R�[������
	float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 6.0f) * M_PI); // ����
	float posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + CORN_SHIFT;
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX + fDifference, posYZ, posYZ });
	posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - CORN_SHIFT;
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - fDifference, posYZ, posYZ });
}