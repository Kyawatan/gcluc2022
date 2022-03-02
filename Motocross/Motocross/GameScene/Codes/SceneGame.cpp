#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskBackGround.h"

SceneGame::SceneGame()
	: SceneBase()
	, m_pPlayer(NULL)
{
	// カメラ位置
	GetpCamera()->SetEyePt(KVector3{ -78, 0, -870 });

	// 背景生成
	TaskBackGround* back = new TaskBackGround();

	// プレイヤー生成
	m_pPlayer = new TaskPlayer();
	m_pPlayer->m_TaskTransform.SetTransform
	(
		KVector3{ -550, 0, 0 },			// pos
		KVector3{ 0, 0, 0 },			// rot
		KVector3{ 1, 1, 1 }				// scale
	);
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{

}