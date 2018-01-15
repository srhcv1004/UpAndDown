#pragma once
#include "cGameNode.h"

class cMainMenu : public cGameNode
{
private:

public:
	cMainMenu();
	virtual ~cMainMenu();

	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

