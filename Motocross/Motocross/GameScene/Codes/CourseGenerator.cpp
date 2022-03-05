#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskCorn.h"
#include "TaskKobu.h"

#define JUMP_RAMP_PERIOD	2000	// �W�����v��̊Ԋu
#define KOBU_INTERVAL		800		// �W�����v�n�߂ƃW�����v�I���̃R�u�̊Ԋu


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
	// �R�u����
	TaskBase* kobuStart = new TaskKobu(KVector3{ posX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ posX + KOBU_INTERVAL, 0, 0 });

	// �R�[���ƃR�u�̋����̃��x���i4�i�K�j�������_���Ō���
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 400, 500, 600, 700 };

	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // ��������

	// �R�[������
	const float fCornShift = 40; // �R�[�������[���̒[�ɂ���悤��
	float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 6.0f) * M_PI); // ����
	float posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fCornShift;
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + fDifference, posYZ, posYZ });
	posYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fCornShift;
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - fDifference, posYZ, posYZ });
}