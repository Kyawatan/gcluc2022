#include "CourseGenerator.h"
#define _USE_MATH_DEFINES // math.h���C���N���[�h����O�ɋL�q
#include <math.h>
#include <random>
#include "LaneManager.h"
#include "CollisionDetector.h"
#include "TaskBackGround.h"
#include "TaskGate.h"
#include "TaskFlag.h"
#include "TaskKobu.h"
#include "TaskRock.h"
#include "TaskTrikNavi.h"

#define JUMP_RAMP_PERIOD	4000	// �W�����v��̊Ԋu
#define KOBU_INTERVAL		550		// �W�����v�n�߂ƃW�����v�I���̃R�u�̊Ԋu
#define GATE_PERIOD			600		// ���_�Q�[�g�̊Ԋu
#define ROCK_PERIOD			600		// ��̊Ԋu


CourseGenerator::CourseGenerator(GameDirector* pGameDirector)
	: m_pGameDirector(pGameDirector)
	, m_pLaneManager(m_pGameDirector->GetLaneManagerInstance())
	, m_pCollisionDetector(m_pGameDirector->GetCollisionDetectorInstance())
	, m_fDepthCorrection(m_pGameDirector->GetDepthCorrection())
	, m_fCourseLength(0)
	, m_fLaneLeftEndPos(m_pLaneManager->GetLanePos(E_CourseLane::Left) + 50)
	, m_fLaneRightEndPos(m_pLaneManager->GetLanePos(E_CourseLane::Right) - 50)
{

}

CourseGenerator::~CourseGenerator()
{

}

void CourseGenerator::Init()
{
	// �w�i����
	SetBackGround();
	// �X�^�[�g����
	SetStart();
	// �S�[������
	SetGoal();
	// �W�����v�䐶���i4000, 8000, 12000�j
	for (int i = 0; i < JUMP_RAMP_NUM; i++)
	{
		SetJumpRamp(i, JUMP_RAMP_PERIOD * (i + 1));
	}
	// �W�����v��̂Ȃ��ꏊ�Ɋe�I�u�W�F�N�g�𐶐��i1000�`3000�A5000�`7000�A9000�`11000�j
	const int iStartPos = 1000;
	const int iLastPos = iStartPos + 2000;
	const int iInterval = 4000;
	for (int i = 0; i < 3; i++)
	{
		// ���_�Q�[�g����
		for (int j = iStartPos + 300; j <= iLastPos; j += GATE_PERIOD)
		{
			SetGate(j + (i * iInterval));
		}
		// �␶��
		for (int j = iStartPos; j <= iLastPos; j += ROCK_PERIOD)
		{
			SetRock(j + (i * iInterval));
		}
	}
}

void CourseGenerator::SetBackGround()
{
	TaskBackGround* back = new TaskBackGround(E_SceneName::Game);
	m_fCourseLength = back->GetCourseLength();
}

void CourseGenerator::SetStart()
{
	const float fStartPos = 250;

	// �X�^�[�g�Q�[�g����
	TaskBase* gateLeft = new TaskGame(KVector3{ fStartPos, 0, 0 }, E_FrameName::Left);
	TaskBase* gateRight = new TaskGame(KVector3{ fStartPos, 0, 0 }, E_FrameName::Base);
}

void CourseGenerator::SetGoal()
{
	const float fGoalPos = m_fCourseLength - 1000;
	
	// �S�[���Q�[�g����
	TaskBase* gateLeft = new TaskGame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Left);
	TaskBase* gateRight = new TaskGame(KVector3{ fGoalPos, 0, 0 }, E_FrameName::Goal);

	// �S�[���ʒu��o�^
	KVector2 vStart = KVector2{ fGoalPos - m_fDepthCorrection, m_fLaneRightEndPos };
	KVector2 vEnd = KVector2{ fGoalPos + m_fDepthCorrection, m_fLaneLeftEndPos };
	EVENT_COLLISION cs;
	cs.eName = E_EventName::CourseGoal;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskGame*>(gateRight)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetJumpRamp(int iIndex, float fPosX)
{
	// �R�[���ƃR�u�̋����i4�i�K�AQTE���x���j�������_���Ō���
	const int iQTELevel = 4;
	const int iDistances[iQTELevel] = { 300, 400, 500, 600 }; // 3�b�A4�b�A5�b�A6�b
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distrDistance(0, iQTELevel - 1);
	int fDistanceRand = (int)distrDistance(eng); // ��������

	// �e���[����QTE��x�������_���Ō���
	std::uniform_int_distribution<int> distrDifficulty(static_cast<int>(E_TrikDifficulty::Beginner), static_cast<int>(E_TrikDifficulty::Advanced));
	int fDifficultyRand = (int)distrDifficulty(eng);
	QTE_INFORMATION info;
	info.iIndex = iIndex;
	info.eLeftDifficulty = static_cast<E_TrikDifficulty>(fDifficultyRand % static_cast<int>(E_TrikDifficulty::Num));
	info.eCenterDifficulty = static_cast<E_TrikDifficulty>((fDifficultyRand + 1) % static_cast<int>(E_TrikDifficulty::Num));
	info.eRightDifficulty = static_cast<E_TrikDifficulty>((fDifficultyRand + 2) % static_cast<int>(E_TrikDifficulty::Num));
	m_pGameDirector->SetQTEInformation(info);

	float fLeftPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Left);
	float fCentertPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Center);
	float fRightPosYZ = m_pLaneManager->GetLanePos(E_CourseLane::Right);
	// �g���b�N��x�ӂ���������
	const float fTrikDistance = fPosX - iDistances[fDistanceRand] + 80; // �R�u����̋���
	TaskBase* RightTrikNavi = new TaskTrikNavi(m_pGameDirector, iIndex, info.eRightDifficulty, KVector3{ fTrikDistance - m_fDepthCorrection, fRightPosYZ, fRightPosYZ });
	TaskBase* CenterTrikNavi = new TaskTrikNavi(m_pGameDirector, iIndex, info.eCenterDifficulty, KVector3{ fTrikDistance, fCentertPosYZ, fCentertPosYZ });
	TaskBase* LeftTrikNavi = new TaskTrikNavi(m_pGameDirector, iIndex, info.eLeftDifficulty, KVector3{ fTrikDistance + m_fDepthCorrection, fLeftPosYZ, fLeftPosYZ });
	// ���E�t���O����
	const float fCornDistance = fPosX - iDistances[fDistanceRand]; // �R�u����̋���
	const float fLaneEdgeOffset = 40; // ���[����������[�ɂ��炷��
	TaskBase* flagLeft = new TaskFlag(KVector3{ fCornDistance + m_fDepthCorrection, fLeftPosYZ + fLaneEdgeOffset, fLeftPosYZ + fLaneEdgeOffset });
	TaskBase* flagRight = new TaskFlag(KVector3{ fCornDistance - m_fDepthCorrection, fRightPosYZ - fLaneEdgeOffset, fRightPosYZ - fLaneEdgeOffset });
	// �R�u����
	TaskBase* kobuStart = new TaskKobu(KVector3{ fPosX, 0, 0 });
	TaskBase* kobuEnd = new TaskKobu(KVector3{ fPosX + KOBU_INTERVAL, 0, 0 });

	KVector2 vStart;
	KVector2 vEnd;
	EVENT_COLLISION cs;

	// QTE�J�n�ʒu��o�^
	vStart = KVector2{ flagRight->m_TaskTransform.GetPosition().x, flagRight->m_TaskTransform.GetPosition().y };
	vEnd = KVector2{ flagLeft->m_TaskTransform.GetPosition().x, flagLeft->m_TaskTransform.GetPosition().y };
	cs.eName = E_EventName::QTEStart;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskCorn*>(cornRight)->DrawCollisionLine(vStart, vEnd);

	// QTE�I���ʒu��o�^
	const float fAdjustment = 70; // �R�u�̎�O�ɒ���
	vStart = KVector2{ fPosX - fAdjustment - m_fDepthCorrection, m_fLaneRightEndPos };
	vEnd = KVector2{ fPosX - fAdjustment + m_fDepthCorrection, m_fLaneLeftEndPos };
	cs.eName = E_EventName::QTEEnd;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskKobu*>(kobuStart)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetGate(float fPosX)
{
	// ���[��������
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_int_distribution<int> distr1(0, 2);
	float fLanePos = m_pLaneManager->GetLanePos(static_cast<E_CourseLane>((int)distr1(eng)));

	// ���E�t���O����
	const float fLaneEdgeOffset = 50; // ���[����������[�ɂ��炷��
	float fLeftPosYZ = fLanePos + fLaneEdgeOffset;
	float fRightPosYZ = fLanePos - fLaneEdgeOffset;
	TaskBase* flagLeft = new TaskFlag(KVector3{ fPosX + (m_fDepthCorrection * 0.35f), fLeftPosYZ, fLeftPosYZ });
	TaskBase* flagRight = new TaskFlag(KVector3{ fPosX - (m_fDepthCorrection * 0.35f), fRightPosYZ, fRightPosYZ });

	// ���_�ʒu��o�^
	KVector2 vStart = KVector2{ flagRight->m_TaskTransform.GetPosition().x, flagRight->m_TaskTransform.GetPosition().y };
	KVector2 vEnd = KVector2{ flagLeft->m_TaskTransform.GetPosition().x, flagLeft->m_TaskTransform.GetPosition().y };
	EVENT_COLLISION cs;
	cs.eName = E_EventName::ScoringGate;
	cs.vStart = vStart;
	cs.vEnd = vEnd;
	m_pCollisionDetector->SetCollision(cs);
	// �f�o�b�O�p�⏕���\��
	//dynamic_cast<TaskFlag*>(flagRight)->DrawCollisionLine(vStart, vEnd);
}

void CourseGenerator::SetRock(float fPosX)
{
	// ��������
	std::random_device rd;
	std::default_random_engine eng(rd());
	//std::uniform_int_distribution<int> distr(0, 2);
	std::uniform_int_distribution<int> distr(1, 2);
	int fRandNum = (int)distr(eng);	// ����X���W�̊�̌�
	int fRandLane = (int)distr(eng); // 1�ڂ̊�̃��[��

	// ����X���W��0�`2����
	// ����X���W��1�`2����
	for (int i = 0; i < fRandNum; i++)
	{
		// 2�ڂ̊��1�ڂ̊�̃��[���Əd�Ȃ�Ȃ��悤�Ƀ��[����1���炵�āi1 2 -�j�i- 1 2�j�i2 - 1�j��3�p�^�[�����ł���
		float fPosYZ = m_pLaneManager->GetLanePos(static_cast<E_CourseLane>((fRandLane + i) % 3));
		if (m_pLaneManager->GetCurrentLane(fPosYZ) == E_CourseLane::Left)
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX + m_fDepthCorrection, fPosYZ, fPosYZ });
		}
		else if (m_pLaneManager->GetCurrentLane(fPosYZ) == E_CourseLane::Right)
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX - m_fDepthCorrection, fPosYZ, fPosYZ });
		}
		else
		{
			TaskBase* rock = new TaskRock(KVector3{ fPosX, fPosYZ, fPosYZ });
		}
	}
}