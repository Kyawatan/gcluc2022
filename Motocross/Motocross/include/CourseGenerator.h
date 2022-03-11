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

	void InitCourse();
	void SetJumpRamp(float posX);
	void SetRock();
};