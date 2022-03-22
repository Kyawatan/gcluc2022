#include "ScoreController.h"
#include "GameDirector.h"


ScoreController::ScoreController()
	: m_ScoreUIManager()
	, m_iTotalScore(0)
	, m_iHundred(0)
	, m_iTen(0)
	, m_fWaitTime(0)
	, m_iCount(0)
	, m_onTotalScore(false)
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
		iBearTip = static_cast<int>(E_BearPointTip::TrikBiginner);
	}
	else if (ePoint == E_Point::TrikIntermediate)
	{
		iBearTip = static_cast<int>(E_BearPointTip::TrikIntermediate);
	}
	else if (ePoint == E_Point::TrikAdvanced)
	{
		iBearTip = static_cast<int>(E_BearPointTip::TrikAdvanced);
	}
	else if (ePoint == E_Point::DamageRock)
	{
		iBearTip = static_cast<int>(E_BearPointTip::DamageRock);
	}
	else if (ePoint == E_Point::PointGate)
	{
		iBearTip = static_cast<int>(E_BearPointTip::PointGate);
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
	if (m_onTotalScore)
	{
		SetTotalScore();
	}
}

const int ScoreController::GetTotalScore()
{
	return m_iTotalScore;
}

void ScoreController::SetTotalScore(const int iTotalScore)
{
	m_iTotalScore = iTotalScore;
	m_ScoreUIManager.SetScoreNumber(0); // 一の位表示(必ず0)
	if (iTotalScore != 0)
	{
		m_iHundred = iTotalScore / 100; // 百の位
		m_iTen = (iTotalScore - (m_iHundred * 100)) / 10; // 十の位
	}
	else
	{
		m_iCount = 2; // クマ表示へ
	}
	m_fWaitTime = 0.5f;
	m_onTotalScore = true;
}

void ScoreController::SetTotalScore()
{
	m_fWaitTime -= GetDeltaTime();

	if (m_fWaitTime <= 0)
	{
		switch (m_iCount)
		{
		case 0:
			m_ScoreUIManager.SetScoreNumber(m_iTen); // 十の位表示
			m_fWaitTime = 0.5f;
			m_iCount++;
			break;

		case 1:
			if (m_iHundred != 0)
			{
				m_ScoreUIManager.SetScoreNumber(m_iHundred); // 百の位表示
			}
			m_fWaitTime = 0.5f;
			m_iCount++;
			break;

		case 2:
			int iBearTip = static_cast<int>(E_BearRankTip::Gold);
			if (m_iTotalScore < 150)
			{
				iBearTip = static_cast<int>(E_BearRankTip::Participation);
			}
			else if (m_iTotalScore < 250)
			{
				iBearTip = static_cast<int>(E_BearRankTip::Bronze);
			}
			else if (m_iTotalScore < 350)
			{
				iBearTip = static_cast<int>(E_BearRankTip::Silver);
			}
			m_ScoreUIManager.SetScoreEffect(iBearTip); // クマ表示
			m_onTotalScore = false;
			break;
		}
	}
}