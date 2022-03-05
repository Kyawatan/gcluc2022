#include "CameraController.h"

#define EYEPT_Z				-870
#define CAMERA_START_POS_X	650
#define CAMERA_GOAL_POS_X	COURSE_LENGTH - CAMERA_START_POS_X


CameraController::CameraController()
	: m_pCamera(NULL)
{
	m_pCamera = GetpCamera();
}

CameraController::~CameraController()
{

}

void CameraController::SetDefault()
{
	m_pCamera->SetEyePt(KVector3{ 0, 0, EYEPT_Z });
}

void CameraController::SetCourseStart()
{
	m_pCamera->SetEyePt(KVector3{ CAMERA_START_POS_X, 0, EYEPT_Z });
}

void CameraController::SetCourseGoal()
{
	m_pCamera->SetEyePt(KVector3{ CAMERA_GOAL_POS_X, 0, EYEPT_Z });
}

void CameraController::FollowPlayer(KVector3 vMovement)
{
	KVector3 vCameraPos = KVec3Add(m_pCamera->GetEyePt(), vMovement);
	if (vCameraPos.x < CAMERA_GOAL_POS_X)
	{
		m_pCamera->SetEyePt(vCameraPos);
	}
	else
	{
		SetCourseGoal(); // コース外を写さないようにする
	}
}