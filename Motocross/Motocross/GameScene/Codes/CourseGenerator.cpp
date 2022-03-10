#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskCorn.h"
#include "TaskKobu.h"

#define JUMP_RAMP_NUM		2		// �R�[�X���̃W�����v��̐�
#define JUMP_RAMP_PERIOD	2000	// �W�����v��̊Ԋu
#define KOBU_INTERVAL		550		// �W�����v�n�߂ƃW�����v�I���̃R�u�̊Ԋu


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
	// �w�i����
	TaskBackGround* back = new TaskBackGround();
	// �W�����v�䐶��
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(JUMP_RAMP_PERIOD * (i + 1));
	}
}

void CourseGenerator::SetJumpRamp(float posX)
{
	const float fLaneEdgeOffset = 40; // ���[����������[�ɂ��炷��
	const float fDifference = m_pLaneManager->GetLaneInterval() * cos((1.0f / 5.0f) * M_PI); // ����
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fLaneEdgeOffset;
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fLaneEdgeOffset;
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 600, 900, 1050, 1200 } // 2�b�A3�b�A3.5�b�A4�b
	;
	// �R�[���ƃR�u�̋����i4�i�K�AQTE���x���j�������_���Ō���
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // ��������

	// ���E�R�[������
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + fDifference, fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - fDifference, fRightPosYZ, fRightPosYZ });
	// �R�u����
	TaskBase* kobuStart = new TaskKobu(KVector3{ posX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ posX + KOBU_INTERVAL, 0, 0 });

	KVector2 vStart;
	KVector2 vEnd;
	COLLISION cs;

	// QTE�J�n�ʒu��o�^
	vStart = KVector2{ cornRight->m_TaskTransform.GetPosition().x, cornRight->m_TaskTransform.GetPosition().y };
	vEnd = KVector2{ cornLeft->m_TaskTransform.GetPosition().x, cornLeft->m_TaskTransform.GetPosition().y };
	cs.eName = E_CollisionName::StartQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskCorn*>(cornRight)->DrawCollisionLine(vStart, vEnd);

	// QTE�I���ʒu��o�^
	const float fAdjustment = 90; // �R�u�̎�O�ɒ���
	vStart = KVector2{ posX - fDifference - fAdjustment, fRightPosYZ };
	vEnd = KVector2{ posX + fDifference - fAdjustment, fLeftPosYZ };
	cs.eName = E_CollisionName::EndQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine(vStart, vEnd);
}