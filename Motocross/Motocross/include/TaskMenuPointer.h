#pragma once
#include "QuadBase.h"
#include "Definition.h"

enum class E_Menu
{
	Play,
	Book,
	Howto,
};

class TaskMenuPointer : public TaskBase
{
public:
	TaskMenuPointer();
	~TaskMenuPointer();

	void Update() override;
	void Draw() override;

	void MoveUp();
	void MoveDown();
	const E_Menu GetCurrentPos();

private:
	QuadBase* m_pSprite;
	E_Menu m_currentPos;

	void SetPlay();
	void SetBook();
	void SetHowto();

	void Effect();
};