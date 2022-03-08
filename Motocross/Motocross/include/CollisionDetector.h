/*
	プレイヤーと奥行きのあるオブジェクト（コーンやコブ）との当たり判定を行う
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
	KVector2 m_vStart;	// 始点
	KVector2 m_vEnd;	// 終点
};