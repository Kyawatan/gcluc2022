#pragma once
#include <list>
#include "Definition.h"
#include "TaskKey.h"


class QTEUIManager
{
public:
	QTEUIManager();
	~QTEUIManager();

	// 入力キー番号を全て受け取り、キーUIを生成・表示
	void SetInputKeyList(std::list<int> iInputKeyList);

	// キーUI（Now, Next, NextNext）を次のキーに更新
	void SetNextInputKey();

	// 残っているキーをすべて終了させる
	void FinishRemainingInputKey();

	// 入力ミスしたNowキーを赤くする
	void SetFailure();

private:
	std::list<TaskBase*> m_pTaskKeyList;
};