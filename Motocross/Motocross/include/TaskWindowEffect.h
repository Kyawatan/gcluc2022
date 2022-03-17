#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskWindowEffect : public TaskBase
{
public:
	TaskWindowEffect(float fOpacity);
	~TaskWindowEffect();

	void Update() override;
	void Draw() override;

	void Dark(float fTime);
	void FadeIn(float fTime);
	void FadeOut(float fTime);

private:
	QuadBase*	m_pSprite;
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;

	float m_fDarkTime;
	float m_fFadeInTime;
	float m_fFadeOutTime;

	void Dark();
	void FadeIn();
	void FadeOut();

	void (TaskWindowEffect::* m_pFunc)();				// �֐��|�C���^
	void SetFunc(void (TaskWindowEffect::* pFunc)());	// ���s����֐����Z�b�g����
	void ExitFunc();									// �������I�������֐����甲����
};