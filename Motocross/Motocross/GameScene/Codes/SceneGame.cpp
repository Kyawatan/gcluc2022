#include "SceneGame.h"
#include "TaskPlayer.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_MainCamera()
	, m_LaneManager()
	, m_CollisionDetector()
	, m_CourseGenerator(&m_LaneManager, &m_CollisionDetector) // コース生成
	, m_pPlayer(NULL)
{
	// プレイヤー生成
	m_pPlayer = new TaskPlayer(&m_LaneManager);
	// メインカメラ位置
	m_MainCamera.SetCourseStart();
}

SceneGame::~SceneGame()
{

}

void SceneGame::Update()
{
	// メインカメラ
	m_MainCamera.FollowPlayer(dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCameraMovement());
	
	// プレイヤーとジャンプ台（QTE開始と終了）の当たり判定
	KVector2 vPlayerPoint = dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCollisionPoint();
	int eCollisionNum = m_CollisionDetector.CollisionDetection(vPlayerPoint);

	switch (eCollisionNum)
	{
	case static_cast<int>(E_CollisionName::StartQTE):
		dynamic_cast<TaskPlayer*>(m_pPlayer)->SetEvent();
		break;

	case static_cast<int>(E_CollisionName::EndQTE):
		dynamic_cast<TaskPlayer*>(m_pPlayer)->Jump();
		break;

	default:
		break;
	}
}