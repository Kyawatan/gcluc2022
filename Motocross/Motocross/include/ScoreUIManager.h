#pragma once
#include "Definition.h"

enum class E_BearPointTip
{
	TrikBiginner,
	TrikIntermediate,
	TrikAdvanced,
	DamageRock,
	PointGate,
};

enum class E_BearRankTip
{
	Participation,
	Bronze,
	Silver,
	Gold,
};

class ScoreUIManager
{
public:
	ScoreUIManager();
	~ScoreUIManager();

	void SetAddPointEffect(int iBearTip);
	void SetScoreEffect(int iBearTip);
	void SetScoreNumber(int iTotalScore);

private:
	TaskBase* m_pBear;
	int	m_iCount;
};