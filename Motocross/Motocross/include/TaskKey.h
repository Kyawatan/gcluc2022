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
	void FadeOut();
	void KeyDownEffect(bool isSuccess);
	void FailureEffect();
	void TranslatetoNext();

private:
	QuadBase*		m_pSprite;
	E_KeyPossition	m_eCurrentKeyPos;
	KVector3		m_vPos;
	KVector3		m_vDiffuse;
	float			m_fOpacity;
	int				m_iKeyCode;
	bool			m_isOnce;
	bool			m_isFinish;

	void (TaskKey::* m_pFunc)();				// 関数ポインタ
	void SetFunc(void (TaskKey::* pFunc)());	// 実行する関数をセットする
	void ExitFunc();							// 処理が終了した関数から抜ける

	void FadeIn();
	void KeyDownEffect();
	KVector3 SetPosition(E_KeyPossition eKeyPos);
};