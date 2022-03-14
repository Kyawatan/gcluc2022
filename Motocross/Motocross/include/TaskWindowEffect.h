#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskWindowEffect : public TaskBase
{
public:
	TaskWindowEffect();
	~TaskWindowEffect();

	void Update() override;
	void Draw() override;

	void Dark();
	void FadeIn();
	void FadeOut();

private:
	QuadBase*	m_pSprite;
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskWindowEffect::* m_pFunc)();				// �֐��|�C���^
	void SetFunc(void (TaskWindowEffect::* pFunc)());	// ���s����֐����Z�b�g����
	void ExitFunc();									// �������I�������֐����甲����
};