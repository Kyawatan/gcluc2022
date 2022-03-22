#include "CollisionDetector.h"
#include "GameDirector.h"
#include "TaskPlayer.h"


CollisionDetector::CollisionDetector()
	: m_collisionList()
{

}

CollisionDetector::~CollisionDetector()
{
	m_collisionList.clear();
}

void CollisionDetector::SetCollision(EVENT_COLLISION Collision)
{
	m_collisionList.push_back(Collision);
}

int CollisionDetector::CollisionDetection(KVector2 vPoint)
{
	if (m_collisionList.size() == 0) return -1;

	for (auto itr = m_collisionList.begin(); itr != m_collisionList.end();)
	{
		KVector2 v1 = KVec2Sub((*itr).vEnd, (*itr).vStart);
		KVector2 v2 = KVec2Sub(vPoint, (*itr).vStart);
		int iDot = KVec2Dot(v1, v2);
		int iMag = KVec2Mag(v1) * KVec2Mag(v2);
		if (iDot == iMag && KVec2Mag(v2) <= KVec2Mag(v1))
		{
			// Õ“Ë‚µ‚Ä‚¢‚é
			E_EventName eName = (*itr).eName;
			m_collisionList.erase(itr);
			return static_cast<int>(eName);
		}
		++itr;
	}
	return -1;
}
