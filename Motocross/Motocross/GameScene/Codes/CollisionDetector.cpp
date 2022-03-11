#include "CollisionDetector.h"


CollisionDetector::CollisionDetector()
	: m_collisionList()
{

}

CollisionDetector::~CollisionDetector()
{
	m_collisionList.clear();
}

void CollisionDetector::SetCollision(COLLISION Collision)
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
		if (iDot == iMag)
		{
			// è’ìÀÇµÇƒÇ¢ÇÈ
			E_CollisionName eName = (*itr).eName;
			m_collisionList.pop_front();
			return static_cast<int>(eName);
		}
		++itr;
	}
	return -1;
}
