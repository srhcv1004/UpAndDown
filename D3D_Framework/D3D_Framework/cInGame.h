#pragma once
#include "cGameNode.h"

class cCamera;
class iGrid;
class cXFileObj;
class cProgressBar;

class cInGame : public cGameNode
{
private:
	cCamera*												m_pCamera;

private:
	iGrid*													m_pLineGrid;
	cXFileObj*												m_pXFilePlayer;
	cProgressBar*											m_pProgressBar;

	float													m_fMax;
	float													m_fCur;

public:
	cInGame();
	virtual ~cInGame();

	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

