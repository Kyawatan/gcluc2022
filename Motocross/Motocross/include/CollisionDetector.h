/*
	�v���C���[�Ɖ��s���̂���I�u�W�F�N�g�i�R�[����R�u�j�Ƃ̓����蔻����s��
*/

#pragma once
#include "Definition.h"


class CollisionDetector
{
public:
	CollisionDetector(KVector2 vStart, KVector2 vEnd);
	~CollisionDetector();

	void CollisionDetection();

private:
	KVector2 m_vStart;	// �n�_
	KVector2 m_vEnd;	// �I�_
};