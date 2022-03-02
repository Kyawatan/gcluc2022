#pragma once
#include "Definition.h"


class SceneGame : public SceneBase
{
public:
	SceneGame();
	~SceneGame();

	void Update() override;

private:
	TaskBase* m_pPlayer;
};