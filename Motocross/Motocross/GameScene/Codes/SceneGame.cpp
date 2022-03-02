#include "SceneGame.h"
#include "TaskPlayer.h"
#include "TaskBackGround.h"

SceneGame::SceneGame()
	: SceneBase()
	, m_pPlayer(NULL)
{
	// �J�����ʒu
	GetpCamera()->SetEyePt(KVector3{ -78, 0, -870 });

	// �w�i����
	TaskBackGround* back = new TaskBackGround();

	// �v���C���[����
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