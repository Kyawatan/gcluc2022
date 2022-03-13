#pragma once
#include "QuadBase.h"
#include "Definition.h"
#include "GameDirector.h"

class TaskTrikNavi : public TaskBase
{
public:
	TaskTrikNavi(GameDirector* pGameDirector, int iTrikNum, KVector3 vPos);
	~TaskTrikNavi();

	void Update() override;
	void Draw() override;

private:
	QuadBase*	m_pSprite;
	GameDirector* m_pGameDirector;
	int			m_iTrikNum;
	float		m_fOpacity;
	bool		m_isOnce;
	bool		m_isFinish;
	float		m_fEmphasizeCos; // �ӂ������㉺�ړ�����cos�̒l
	float		m_fFadeOutOpacityWait; // �t�F�[�h�A�E�g���̓������̑҂�����

	void (TaskTrikNavi::* m_pFunc)();				// �֐��|�C���^
	void SetFunc(void (TaskTrikNavi::* pFunc)());	// ���s����֐����Z�b�g����
	void ExitFunc();							// �������I�������֐����甲����

	void Emphasize();
	void FadeOut(bool isSelect);
	void SelectFadeOut();
	void FadeOut();
};