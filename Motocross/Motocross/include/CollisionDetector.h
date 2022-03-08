/*
	�v���C���[�Ɖ��s���̂���I�u�W�F�N�g�i�R�[����R�u�j�Ƃ̓����蔻����s��
	�_�Ɛ��̓����蔻��
*/

#pragma once
#include <list>
#include "Definition.h"


enum class E_CollisionName
{
	CourseGoal,
	StartQTE,
	EndQTE,
};

struct COLLISION
{
	E_CollisionName eName;
	KVector2		vStart;	// ���̎n�_
	KVector2		vEnd;	// ���̏I�_
};

class CollisionDetector
{
public:
	CollisionDetector();
	~CollisionDetector();

	void SetCollision(COLLISION Collision);
	int CollisionDetection(KVector2 vPoint);

private:
	std::list<COLLISION> m_collisionList;
};