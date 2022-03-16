#pragma once
#include "Definition.h"
#include "ScoreUIManager.h"

enum class E_Point
{
	TrikBiginner		= 30,
	TrikIntermediate	= 50,
	TrikAdvanced		= 100,
	DamageRock			= -10,
	PointGate			= 10,
};

class ScoreController
{
public:
	friend class SceneGame;

	ScoreController();
	~ScoreController();

	void AddPoint(E_Point ePoint);

private:
	ScoreUIManager	m_ScoreUIManager;
	int				m_iTotalScore;

	// SceneGameだけが呼び出す
	void Update();
	const int GetTotalScore();
};