#include "SceneGame.h"
#include "TaskPlayer.h"
#include "CameraController.h"


SceneGame::SceneGame()
	: SceneBase()
	, m_MainCamera()
	, m_LaneManager()
	, m_CollisionDetector()
	, m_CourseGenerator(&m_LaneManager, &m_CollisionDetector) // コース生成
	, m_QTEController()
	, m_pPlayer(NULL)
{
	// プレイヤー生成
	m_pPlayer = new TaskPlayer(&m_LaneManager);
	m_QTEController.SetPlayerInstance(dynamic_cast<TaskPlayer*>(m_pPlayer));
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
	
	// プレイヤーとイベント発生ポイントの当たり判定
	KVector2 vPlayerPoint = dynamic_cast<TaskPlayer*>(m_pPlayer)->GetCollisionPoint();
	int eCollisionNum = m_CollisionDetector.CollisionDetection(vPlayerPoint);
	// イベント発生
	switch (eCollisionNum)
	{
	case static_cast<int>(E_CollisionName::StartQTE):
		// QTE開始
		m_QTEController.StartQTE(E_TrikDifficulty::Intermediate);
		break;

	case static_cast<int>(E_CollisionName::EndQTE):
		// QTE終了
		m_QTEController.FinishQTE();
		break;

	case static_cast<int>(E_CollisionName::CourseGoal):
		// ゴール

		break;

	case static_cast<int>(E_CollisionName::ScoringGate):
		// 加点

		break;
	}

	m_QTEController.Update();
}