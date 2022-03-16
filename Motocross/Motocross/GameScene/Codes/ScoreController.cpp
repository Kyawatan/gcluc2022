#include "ScoreController.h"
#include "GameDirector.h"


ScoreController::ScoreController()
	: m_ScoreUIManager()
	, m_iTotalScore(0)
{

}

ScoreController::~ScoreController()
{

}

void ScoreController::AddPoint(E_Point ePoint)
{
	int iBearTip = 0;
	if (ePoint == E_Point::TrikBiginner)
	{
		iBearTip = static_cast<int>(E_BearTip::TrikBiginner);
	}
	else if (ePoint == E_Point::TrikIntermediate)
	{
		iBearTip = static_cast<int>(E_BearTip::TrikIntermediate);
	}
	else if (ePoint == E_Point::TrikAdvanced)
	{
		iBearTip = static_cast<int>(E_BearTip::TrikAdvanced);
	}
	else if (ePoint == E_Point::DamageRock)
	{
		iBearTip = static_cast<int>(E_BearTip::DamageRock);
	}
	m_ScoreUIManager.SetAddPointEffect(iBearTip);
	m_iTotalScore += static_cast<int>(ePoint);
	if (m_iTotalScore < 0)
	{
		m_iTotalScore = 0;
	}
}

void ScoreController::Update()
{

}

const int ScoreController::GetTotalScore()
{
	return m_iTotalScore;
}