#pragma once
#include "QuadBase.h"
#include "Definition.h"


class TaskNumber : public TaskBase
{
public:
	TaskNumber();
	~TaskNumber();

	void Update() override;
	void Draw() override;

	void SetNumber(int iNumber, float posX, float posY);
	void Disable();

private:
	QuadBase*	m_pSprite;
	int			m_iNumber;
	KVector3	m_vPos;
	bool		m_onEnable;
	bool		m_isOnce;
	bool		m_isFinish;

	void (TaskNumber::* m_pFunc)();				// �֐��|�C���^
	void SetFunc(void (TaskNumber::* pFunc)());	// ���s����֐����Z�b�g����
	void ExitFunc();							// �������I�������֐����甲����

};