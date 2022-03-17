#include "TaskNumber.h"
#include <assert.h>
#include "ScrapTexQuad.h"


TaskNumber::TaskNumber()
	: TaskBase(0, static_cast<int>(E_TaskDrawNum::UI), NULL)
	, m_pSprite(NULL)
	, m_iNumber(0)
	, m_vPos(KVector3{ 0, 0, 0 })
	, m_onEnable(false)
	, m_isOnce(true)
	, m_isFinish(true)
	, m_pFunc(NULL)
{
	// スプライト作成
	m_pSprite = new ScrapTexQuad(&m_TaskTransform);
	// テクスチャをセット
	TEXTURE_SCRAP_INFO texInfo;
	texInfo.iTipWidth = 168;
	texInfo.iTipHeight = 256;
	texInfo.iTipRow = 4;
	texInfo.iTipColumn = 3;
	dynamic_cast<ScrapTexQuad*>(m_pSprite)->SetTexture(504, 1024, L"Resources/img/number.png", &texInfo);
}

TaskNumber::~TaskNumber()
{

}

void TaskNumber::Update()
{
	if (m_onEnable == false) return;

	// カメラ座標に合わせる
	KVector3 vCameraPos = GetpCamera()->GetEyePt();
	KVector3 vPos = KVec3Add(m_vPos, KVector3{ vCameraPos.x, vCameraPos.y, 0 });
	m_TaskTransform.SetPosition(vPos);
	// 現在のエフェクトを実行
	if (m_pFunc != NULL) (this->*m_pFunc)();
}

void TaskNumber::Draw()
{
	if (m_onEnable == false) return;

	dynamic_cast<ScrapTexQuad*>(m_pSprite)->Draw(0, m_iNumber);
}

void TaskNumber::SetNumber(int iNumber, float posX, float posY)
{
	assert(0 <= iNumber && iNumber <= 10);

	m_iNumber = iNumber;
	m_vPos =  KVector3{ posX, posY, 0 };
	m_onEnable = true;
}

void TaskNumber ::Disable()
{
	m_onEnable = false;
}

/*****************************************************************************
	その他
*****************************************************************************/

void TaskNumber::SetFunc(void (TaskNumber::* pFunc)())
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

void TaskNumber::ExitFunc()
{
	m_isOnce = true;
	m_isFinish = true;
	m_pFunc = NULL;
}