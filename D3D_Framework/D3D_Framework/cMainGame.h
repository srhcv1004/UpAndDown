#pragma once
#include "cGameNode.h"

class cInGame;
class cMainMenu;

class cMainGame : public cGameNode
{
private:
	cInGame*												m_pInGame;
	cMainMenu*												m_pMainMenu;

private:

public:
	cMainGame();
	virtual ~cMainGame();

	virtual void Setup();
	virtual void Release();
	virtual void Update();
	virtual void Render();

	void AddScenes();
	void AddFonts();
	void AddSprites();
	void AddSounds();
};

