#include "TaskBear.h"
#include <assert.h>
#define _USE_MATH_DEFINES // math.hをインクルードする前に記述
#include <math.h>
#include "ScrapTexQuad.h"

#define INITIAL_POS KVector3{ 460, -120, 0 }

TaskBear::TaskBear()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::Bear), NULL)
	, m_pSprite(NULL)
	, m_vPos(INITIAL_POS)
	, m_iTipNum(0)
	, m_fWaitTime(0)
	, m_fAngle(0)
	, m_onEnable(false)
	, m_isOnce(true)
	, m_isFinish(false)
	, m_pFunc(NULL)
{
	// スプライト生成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_TaskTransform.SetRotation(KVector3{ 0, 0, (1.0 / 9.0) * M_PI });
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
		KVector3 vCameraPos = GetpCamera()->GetEyePt();
		KVector3 vPos = KVec3Add(m_vPos, KVector3{ vCameraPos.x, vCameraPos.y, 0 });
		m_TaskTransform.SetPosition(vPos);
	}
	// 現在のエフェクトを実行
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskBear::Draw()
{
	if (m_onEnable)
	{
		dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTipNum);
	}
}

/*****************************************************************************
	得点
*****************************************************************************/
void TaskBear::SetAppear(int iTipNum)
{
	/*****************************************************************************
		開始
	*****************************************************************************/
	if (m_isOnce)
	{
		// 得点に合ったグラフィックを表示
		const float fAddWaitTime = 2.0f;
		const float fDeductWaitTime = 1.0f;
		m_iTipNum = iTipNum;
		if (m_iTipNum < 3)
		{
			m_fWaitTime = fAddWaitTime;
		}
		else
		{
			m_fWaitTime = fDeductWaitTime;
		}
		m_onEnable = true;
		m_isOnce = false;
		this->SetFunc(&TaskBear::Appear);
	}
}

void TaskBear::Appear()
{
	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	if (m_iTipNum < 3)
	{
		// 上下移動
		static const float fPeriod = 1.0f; // 周期1秒
		static const int fRadius = 30; // 半径30px
		m_fAngle += GetDeltaTime() / fPeriod;
		float fSin = (float)sin((double)m_fAngle * 2 * M_PI);
		float fMovement = fSin * GetDeltaTime() * fRadius;
		m_vPos = KVec3Add(m_vPos, KVector3{ 0, fMovement, 0 });
	}
	m_fWaitTime -= GetDeltaTime();

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (m_fWaitTime <= 0 || m_isFinish)
	{
		// 初期位置に戻して非表示
		m_vPos = INITIAL_POS;
		m_fWaitTime = 0;
		m_fAngle = 0;
		m_onEnable = false;
		ExitFunc();
	}
}

/*****************************************************************************
	その他
*****************************************************************************/
void TaskBear::SetFunc(void (TaskBear::* pFunc)())
{
	// まだ以前のエフェクトを実行中なら強制終了して次のエフェクトをセット
	if (m_pFunc != NULL && !m_isFinish)
	{
		m_isFinish = true;
		(this->*m_pFunc)();
	}
	m_isOnce = false;
	m_isFinish = false;
	m_pFunc = pFunc;
}

void TaskBear::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}