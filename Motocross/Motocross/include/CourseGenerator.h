#pragma once
#include "Definition.h"
#include "GameDirector.h"


class CourseGenerator
{
public:
	CourseGenerator(GameDirector* pGameDirector);
	~CourseGenerator();

	void Init();

private:
	LaneManager*		m_pLaneManager;
	CollisionDetector*	m_pCollisionDetector;
	const float			m_fDifference; // Zé≤ÇÃç∑ï™
	float				m_fCourseLength;
	float				m_fLaneLeftEndPos;
	float				m_fLaneRightEndPos;

	void SetBackGround();
	void SetStart();
	void SetGoal();

	void SetJumpRamp(float fPosX);
	void SetGate(float fPosX);
	void SetRock(float fPosX);
};