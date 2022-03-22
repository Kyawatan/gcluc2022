#include "ScoreUIManager.h"
#include "TaskBear.h"
#include "TaskNumber.h"

#define CARNING 168 // �����Ԋu


ScoreUIManager::ScoreUIManager()
	: m_pBear(NULL)
	, m_iCount(0)
{

}

ScoreUIManager::~ScoreUIManager()
{
	delete m_pBear;
}

void ScoreUIManager::SetAddPointEffect(int iBearTip)
{
	// ���_�N�}�O���t�B�b�N�\��
	TaskBear* bear = new TaskBear(E_BearTex::Point);
	
	float fEnableTime = 0;
	switch (iBearTip)
	{
	case static_cast<int>(E_BearPointTip::DamageRock):
		fEnableTime = 1.0f;
	case static_cast<int>(E_BearPointTip::PointGate):
		fEnableTime = 1.0f;
		break;

	default:
		fEnableTime = 2.0f;
		break;
	}
	bear->SetAppear(iBearTip, fEnableTime);
}

void  ScoreUIManager::SetScoreEffect(int iBearTip)
{
	// �����N�N�}�O���t�B�b�N�\��
	m_pBear = new TaskBear(E_BearTex::Rank);
	dynamic_cast<TaskBear*>(m_pBear)->SetAppear(iBearTip, 0);
}

void ScoreUIManager::SetScoreNumber(int iNumber)
{
	// �����O���t�B�b�N�\��
	TaskNumber* num = new TaskNumber();
	num->SetNumber(iNumber, -CARNING * m_iCount, 0);
	m_iCount++;
}