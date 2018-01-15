#pragma once
#include "cXFileObj.h"

class cXFilePlayer : public cXFileObj
{
private:
	void KeyControlPlayer();

public:
	cXFilePlayer();
	virtual ~cXFilePlayer();

	virtual void Setup(const CHAR * pXFileName);
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

