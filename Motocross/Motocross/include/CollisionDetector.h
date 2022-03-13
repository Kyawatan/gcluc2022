/*
	プレイヤーと奥行きのあるオブジェクト（コーンやコブ）との当たり判定を行う
	点と線の当たり判定
*/

#pragma once
#include <list>
#include "Definition.h"

enum class E_EventName
{
	None,
	CourseGoal,
	StartQTE,
	EndQTE,
	ScoringGate,
};

struct EVENT_COLLISION
{
	E_EventName eName;
	KVector2	vStart;	// 線の始点
	KVector2	vEnd;	// 線の終点
};

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	void SetCollision(EVENT_COLLISION Collision);
	int CollisionDetection(KVector2 vPoint);

private:
	std::list<EVENT_COLLISION>	m_collisionList;
};