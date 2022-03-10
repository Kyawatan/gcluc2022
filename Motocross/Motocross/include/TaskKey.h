#pragma once
#include "QuadBase.h"
#include "Definition.h"

#define KEY_USING_NUM		static_cast<int>(E_UsingKey::NUM)

enum class E_UsingKey
{
	W,
	A,
	S,
	D,
	I,
	J,
	K,
	L,
	SPACE,
	NUM,
};

enum class E_KeyPossition
{
	Hide,
	NextNext,
	Next,
	Now,
};

class TaskKey : public TaskBase
{
public:
	TaskKey(int iKeyCode);
	~TaskKey();

	void Update() override;
	void Draw() override;

	void FadeIn(E_KeyPossition eKeyPos);
	void FadeIn();
	void FadeOut();
	void SuccessEffect();
	void TranslatetoNext();

private:
	QuadBase*		m_pSprite;
	E_KeyPossition	m_eCurrentKeyPos;
	KVector3		m_vPos;
	KVector3		m_vDiffuse;
	float			m_fOpacity;
	int				m_iKeyCode;
	bool			m_isOnce;

	void (TaskKey::* m_pFunc)(); // 関数ポインタ
	void SetFunc(void (TaskKey::* pFunc)()); // 実行する関数をセットする
	void ExitFunc();

	KVector3 SetPosition(E_KeyPossition eKeyPos);
};