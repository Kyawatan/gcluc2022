#pragma once
#include "Definition.h"

enum class E_BearTip
{
	TrikBiginner,
	TrikIntermediate,
	TrikAdvanced,
	DamageRock,
	PointGate,
};

class ScoreUIManager
{
public:
	ScoreUIManager();
	~ScoreUIManager();

	void SetAddPointEffect(int iBearTip);

private:
	TaskBase* m_pBear;
};