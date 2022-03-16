#pragma once
#include "Definition.h"

class SceneResult : public SceneBase
{
public:
	SceneResult();
	~SceneResult();

	void Update() override;

private:
	TaskBase* m_pWindowEffect;

	bool m_isOnce;
};