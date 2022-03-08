#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "CollisionDetector.h"


class TaskCorn : public TaskBase
{
public:
	TaskCorn(KVector3 vPos);
	~TaskCorn();

	void Update() override;
	void Draw() override;

	void SetCollisionDetector(KVector2 vEnd);	// �v���C���[�Ɠ����蔻����s��
	void DrawCollisionLine();					// �����蔻��̕⏕��������

private:
	QuadBase* m_pSprite;

	QuadBase* m_pCollisionLine;
	CollisionDetector* m_pCollisionDetector;
	KVector2 m_vCollisionStart;
	KVector2 m_vCollisionEnd;
};