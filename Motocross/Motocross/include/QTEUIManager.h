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

	// リストに残っているキーをすべて削除
	void DeleteInputKeyList();

private:
	std::list<TaskBase*> m_pTaskKeyList;
};