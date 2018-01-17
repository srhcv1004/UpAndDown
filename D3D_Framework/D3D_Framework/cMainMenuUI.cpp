#include "stdafx.h"
#include "cMainMenuUI.h"
#include "cUIButton.h"

cMainMenuUI::cMainMenuUI()
	: m_pRootUI(NULL)
{
}

cMainMenuUI::~cMainMenuUI()
{
}

void cMainMenuUI::Setup()
{
	this->SetupUI();
}

void cMainMenuUI::Release()
{
}

void cMainMenuUI::Update()
{
	if (m_pRootUI)
		m_pRootUI->Update();
}

void cMainMenuUI::Render()
{
	if (m_pRootUI)
		m_pRootUI->Render();
}


void cMainMenuUI::SetupUI()
{
	cUIButton* pBody = new cUIButton();
	pBody->Setup("tera",
		"tera",
		"tera",
		D3DXVECTOR3(D_WINSIZEX / 2.F, D_WINSIZEY / 2.F - 200.F, 0.F), 
		E_TAG_NONE);
	m_pRootUI = pBody;

	cUIButton* pStart = new cUIButton();
	pStart->Setup("start",
		"start",
		"start",
		D3DXVECTOR3(0, 300, 0), 
		E_TAG_INGAME);
	m_pRootUI->AddChild(pStart);

	cUIButton* pOption = new cUIButton();
	pOption->Setup("option",
		"option",
		"option",
		D3DXVECTOR3(0, 400, 0),
		E_TAG_NONE);
	m_pRootUI->AddChild(pOption);

	cUIButton* pMaptool = new cUIButton();
	pMaptool->Setup("maptool",
		"maptool",
		"maptool",
		D3DXVECTOR3(0, 500, 0),
		E_TAG_MAPTOOL);
	m_pRootUI->AddChild(pMaptool);
}
