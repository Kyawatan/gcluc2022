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