#include "CameraController.h"

#define EYEPT_Z				-870
#define CAMERA_START_POS_X	-2230


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

void CameraController::FollowPlayer(KVector3 vMovement)
{
	KVector3 vCameraPos = KVec3Add(m_pCamera->GetEyePt(), vMovement);
	m_pCamera->SetEyePt(vCameraPos);
}