#include "ScoreController.h"
#include "GameDirector.h"
#include "TaskNumber.h"

#define CARNING 168 // 文字間隔


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

void ScoreController::SetTotalScore(const int iTotalScore)
{
	// 一の位（必ず0）
	TaskNumber* one = new TaskNumber();
	one->SetNumber(0, 0, 0);
	if (iTotalScore == 0) return;

	// 百の位（0は表示しない）
	int iHundred = iTotalScore / 100;
	if (iHundred != 0)
	{
		TaskNumber* hundred = new TaskNumber();
		hundred->SetNumber(iHundred, -CARNING * 2, 0);
	}

	// 十の位
	int iTen = (iTotalScore - (iHundred * 100)) / 10;
	TaskNumber* ten = new TaskNumber();
	ten->SetNumber(iTen, -CARNING, 0);
}