#include "TaskBear.h"
#include <assert.h>
#include "ScrapTexQuad.h"


TaskBear::TaskBear()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Bear), NULL)
	, m_pSprite(NULL)
	, m_iTipNum(0)
	, m_onEnable(false)
	, m_isOnce(true)
{
	// スプライト生成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 512;
	texInfo.iTipHeight = 768;
	texInfo.iTipRow = 2;
	texInfo.iTipColumn = 2;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(1024, 1536, L"GameScene/Images/bear_point.png", &texInfo);
}

TaskBear::~TaskBear()
{
	delete m_pSprite;
}

void TaskBear::Update()
{
	if (m_onEnable)
	{
		// カメラ座標に合わせる
		m_TaskTransform.SetPosition(GetpCamera()->GetEyePt());
	}
}

void TaskBear::Draw()
{
	if (m_onEnable)
	{
		dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTipNum);
	}
}

void TaskBear::SetRhythm(int iTipNum)
{
	if (m_isOnce)
	{
		m_iTipNum = iTipNum;
		m_onEnable = true;
		m_isOnce = false;
	}
}
