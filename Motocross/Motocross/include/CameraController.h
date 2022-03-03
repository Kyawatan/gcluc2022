#pragma once
#include "Definition.h"


class CameraController
{
public:
	CameraController();
	~CameraController();

	void SetDefault();
	void SetCourseStart();
	void FollowPlayer(KVector3 vMovement);

private:
	KawataCamera* m_pCamera;
};