#include "CameraController.h"

#define EYEPT_Z			-870
#define PLAYER_OFFSET	370


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

void CameraController::FollowPlayer(KVector3 vPlayerPos)
{
	m_pCamera->SetEyePt(KVector3{ vPlayerPos.x + PLAYER_OFFSET, 0, EYEPT_Z });
}