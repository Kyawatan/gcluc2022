#include "ScoreUIManager.h"
#include "TaskBear.h"


ScoreUIManager::ScoreUIManager()
	: m_pBear(NULL)
{
	m_pBear = new TaskBear();
}

ScoreUIManager::~ScoreUIManager()
{
	delete m_pBear;
}

void ScoreUIManager::SetAddPointEffect(int iBearTip)
{
	dynamic_cast<TaskBear*>(m_pBear)->SetAppear(iBearTip);
}