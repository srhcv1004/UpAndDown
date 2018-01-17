#pragma once

class cUI;

class cMainMenuUI
{
private:
	cUI*													m_pRootUI;

private:
	void SetupUI();

public:
	cMainMenuUI();
	~cMainMenuUI();

	void Setup();
	void Release();
	void Update();
	void Render();
};

