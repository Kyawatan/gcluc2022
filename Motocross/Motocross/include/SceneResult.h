#pragma once
#include "Definition.h"
#include "CameraController.h"


class SceneResult : public SceneBase
{
public:
	SceneResult(int iTotalScore);
	~SceneResult();

	void Update() override;

private:
	CameraController	m_CameraController;
	TaskBase* m_pWindowEffect;
	TaskBase* m_pNumber;

	int		m_iTotalScore;
	float	m_fWaitTime;
	int		m_iCount;
	bool	m_isOnce;
};