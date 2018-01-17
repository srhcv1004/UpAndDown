#pragma once
#include "cGameNode.h"

class cMapTool : public cGameNode
{
private:
	void Save();
	void Load();

public:
	cMapTool();
	virtual ~cMapTool();

	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

