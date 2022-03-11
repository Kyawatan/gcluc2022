#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include <random>
#include "TaskBackGround.h"
#include "TaskStealFrame.h"
#include "TaskCorn.h"
#include "TaskKobu.h"
#include "TaskRock.h"

#define JUMP_RAMP_NUM		2		// �R�[�X���̃W�����v��̐�
#define JUMP_RAMP_PERIOD	2000	// �W�����v��̊Ԋu
#define KOBU_INTERVAL		550		// �W�����v�n�߂ƃW�����v�I���̃R�u�̊Ԋu


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
	// �w�i����
	SetBackGround();
	// �S�[������
	SetGoal();
	// �W�����v�䐶��
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(JUMP_RAMP_PERIOD * (i + 1));
	}
	// �␶��
	SetRock();
}

void CourseGenerator::SetBackGround()
{
	TaskBackGround* back = new TaskBackGround();
	m_fCourseLength = back->GetCourseLength();
}

void CourseGenerator::SetGoal()
{
	const float fGoalPos = m_fCourseLength - 550;
	
	// �S�[���S������
	TaskBase* frameLeft = new TaskStealFrame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Left);
	TaskBase* frameRight = new TaskStealFrame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Goal);

	// �S�[���ʒu��o�^
	KVector2 vStart = KVector2{ fGoalPos - m_fDifference, m_fLaneRightEndPos };
	KVector2 vEnd = KVector2{ fGoalPos + m_fDifference, m_fLaneLeftEndPos };
	COLLISION cs;
	cs.eName = E_CollisionName::CourseGoal;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskStealFrame*>(frameRight)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetJumpRamp(float posX)
{
	// �R�[���ƃR�u�̋����i4�i�K�AQTE���x���j�������_���Ō���
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 600, 900, 1050, 1200 }; // 2�b�A3�b�A3.5�b�A4�b
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr(0, iQTELevel - 1);
	int fRand = (int)distr(eng); // ��������

	// ���E�R�[������
	const float fLaneEdgeOffset = 40; // ���[����������[�ɂ��炷��
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left) + fLaneEdgeOffset;
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right) - fLaneEdgeOffset;
	TaskBase* cornLeft = new TaskCorn(KVector3{ posX - iDistances[fRand] + m_fDifference, fLeftPosYZ, fLeftPosYZ });
	TaskBase* cornRight = new TaskCorn(KVector3{ posX - iDistances[fRand] - m_fDifference, fRightPosYZ, fRightPosYZ });
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
	vStart = KVector2{ posX - fAdjustment - m_fDifference, m_fLaneRightEndPos };
	vEnd = KVector2{ posX - fAdjustment + m_fDifference, m_fLaneLeftEndPos };
	cs.eName = E_CollisionName::EndQTE;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetRock()
{
	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left);
	TaskBase* rock = new TaskRock(KVector3{ 800, fLeftPosYZ, fLeftPosYZ });
}