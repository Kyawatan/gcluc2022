#pragma once
#include "QuadBase.h"
#include "Definition.h"

#define USING_KEY_NUM static_cast<int>(E_UsingKey::NUM)

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

class TaskKey : public TaskBase
{
public:
	TaskKey(int iKeyCode);
	~TaskKey();

	void Update() override;
	void Draw() override;

	bool GetOnEnable();
	void SetOnEnable(bool a);

private:
	QuadBase* m_pSprite;
	int m_iKeyCode;
	bool m_onEnable;;
};