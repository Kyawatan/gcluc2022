#pragma once
#include "Definition.h"
#include "LaneManager.h"
#include "CollisionDetector.h"


class CourseGenerator
{
public:
	CourseGenerator(LaneManager* pLaneManager, CollisionDetector* pCollisionDetector);
	~CourseGenerator();

private:
	LaneManager*		m_pLaneManager;
	CollisionDetector*	m_pCollisionDetector;
	const float			m_fDifference; // Zé≤ÇÃç∑ï™
	float				m_fCourseLength;
	float				m_fLaneLeftEndPos;
	float				m_fLaneRightEndPos;

	void InitCourse();

	void SetBackGround();
	void SetStart();
	void SetGoal();

	void SetJumpRamp(float fPosX);
	void SetGate(float fPosX);
	void SetRock(float fPosX);
};