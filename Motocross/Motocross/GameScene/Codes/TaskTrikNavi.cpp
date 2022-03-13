#include "TaskTrikNavi.h"
#define _USE_MATH_DEFINES // math.hインクルード前に記述必要
#include <math.h>
#include "ScrapTexQuad.h"


TaskTrikNavi::TaskTrikNavi(GameDirector* pGameDirector, int iTrikNum, KVector3 vPos)
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_pSprite(NULL)
	, m_pGameDirector(pGameDirector)
	, m_iTrikNum(iTrikNum)
	, m_fOpacity(1)
	, m_isOnce(true)
	, m_isFinish(true)
	, m_fEmphasizeCos(1)
	, m_fFadeOutOpacityWait(0)
	, m_pFunc(&TaskTrikNavi::Emphasize)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	m_TaskTransform.SetPosition(KVec3Add(vPos, KVector3{0, 50, 0}));
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 128;
	texInfo.iTipHeight = 128;
	texInfo.iTipRow = 1;
	texInfo.iTipColumn = 3;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(384, 128, L"GameScene/Images/trik_navi.png", &texInfo);
}

TaskTrikNavi::~TaskTrikNavi()
{

}

void TaskTrikNavi::Update()
{
	switch (m_pGameDirector->GetCurrentEventName())
	{
	case E_EventName::QTEStart:
		// 自分の難度のレーンか否かでエフェクトを変更
		int eDifficulty = static_cast<int>(m_pGameDirector->GetQTEDifficulty());
		if (eDifficulty == m_iTrikNum + 2)
		{
			FadeOut(true);
		}
		else
		{
			FadeOut(false);
		}
		break;
	}

	// 現在のエフェクトを実行
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskTrikNavi::Draw()
{
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iTrikNum);
}

void TaskTrikNavi::Emphasize()
{
	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	// 上下移動
	m_fEmphasizeCos += GetDeltaTime() / 2; // 周期2秒
	float fCos = (float)cos((double)m_fEmphasizeCos * 2 * M_PI);
	float fMovement = fCos * GetDeltaTime() * 20; // 半径20px
	m_TaskTransform.Translate(KVector3{ 0, fMovement, 0 });
}

/*****************************************************************************
	消滅
*****************************************************************************/
void TaskTrikNavi::FadeOut(bool isSelect)
{
	/*****************************************************************************
		開始
	*****************************************************************************/
	if (m_isOnce)
	{
		if (isSelect)
		{
			this->SetFunc(&TaskTrikNavi::SelectFadeOut);
		}
		else
		{
			this->SetFunc(&TaskTrikNavi::FadeOut);
		}
	}
}

void TaskTrikNavi::SelectFadeOut()
{
	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	// 上移動
	float fMovement = GetDeltaTime() * 60; // 1秒間60px
	m_TaskTransform.Translate(KVector3{ 0, fMovement, 0 });
	// 透明
	m_fFadeOutOpacityWait += fMovement;
	if (30 <= m_fFadeOutOpacityWait)
	{
		m_fOpacity -= GetDeltaTime() * 2; // 0.5秒
		m_pSprite->SetOpacity(m_fOpacity);
	}

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		m_pSprite->SetOpacity(0);
		SetTaskStateDying();
		ExitFunc();
	}
}

void TaskTrikNavi::FadeOut()
{
	/*****************************************************************************
		繰り返し
	*****************************************************************************/
	// 透明
	m_fOpacity -= GetDeltaTime() * 10; // 0.1秒
	m_pSprite->SetOpacity(m_fOpacity);

	/*****************************************************************************
		終了
	*****************************************************************************/
	if (m_fOpacity <= 0 || m_isFinish)
	{
		m_pSprite->SetOpacity(0);
		SetTaskStateDying();
		ExitFunc();
	}
}

/*****************************************************************************
	その他
*****************************************************************************/
void TaskTrikNavi::SetFunc(void (TaskTrikNavi::* pFunc)())
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

void TaskTrikNavi::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}