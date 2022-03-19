#pragma once
#include "Definition.h"
#include "CameraController.h"
#include "ScoreController.h"


class SceneResult : public SceneBase
{
public:
	SceneResult(int iTotalScore);
	~SceneResult();

	void Update() override;

private:
	CameraController	m_CameraController;
	ScoreController		m_ScoreController;
	TaskBase* m_pWindowEffect;
	TaskBase* m_pNumber;

	const int m_iTotalScore;
	float	m_fWaitTime;
	int		m_iCount;
};