#pragma once
#include "Definition.h"
#include "CameraController.h"


class SceneTitle : public SceneBase
{
public:
	SceneTitle();
	~SceneTitle();

	void Update() override;

private:
	CameraController	m_CameraController;
	TaskBase* m_pWindowEffect;
	TaskBase* m_pMenuPointer;

	bool m_isOnce;
	float m_fWaitTime;

	void Finish();
};