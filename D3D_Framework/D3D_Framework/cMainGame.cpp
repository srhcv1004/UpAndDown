#include "stdafx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cInGame.h"
#include "cMainMenu.h"

cMainGame::cMainGame()
	: m_pInGame(NULL)
	, m_pMainMenu(NULL)
{
}

cMainGame::~cMainGame()
{
}

void cMainGame::Setup()
{
	cGameNode::Setup();

	this->AddScenes();
	this->AddFonts();
	this->AddSprites();

	D_SCENEMANAGER->ChangeScene("MainMenuScene");
}

void cMainGame::Release()
{
	D_SAFE_RELEASE(m_pInGame);
	D_SAFE_DELETE(m_pInGame);

	D_SAFE_RELEASE(m_pMainMenu);
	D_SAFE_DELETE(m_pMainMenu);

	cGameNode::Release();
}

void cMainGame::Update()
{
	cGameNode::Update();

	D_SCENEMANAGER->Update();
}

void cMainGame::Render()
{
	cGameNode::Render();
	D_TIMEMANAGER->Render();

	D_DEVICE->BeginScene();
	
	D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	static int x = 0;
	static float fRot = 0.F;

	x++;
	if (x >= 13) x = 0;

	fRot += 0.05F;

	D_SCENEMANAGER->Render();


	D3DXMATRIXA16 matWorld, matR, matT, matS;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixIdentity(&matR);
	D3DXMatrixIdentity(&matT);
	D3DXMatrixIdentity(&matS);

	D3DXMatrixScaling(&matS, 0.5F, 0.5F, 0.5F);
	D3DXMatrixTranslation(&matT, D_WINSIZEX / 2, D_WINSIZEY / 2, 0.F);
	D3DXMatrixRotationZ(&matR, fRot);

	matWorld = matS * matR;

	D_SPRITEMANAGER->Render("±Ëø¨æ∆", &matWorld);

	D_DEVICE->EndScene();
	D_DEVICE->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::AddScenes()
{
	m_pMainMenu = new cMainMenu();
	D_SCENEMANAGER->AddScene("MainMenuScene", m_pMainMenu);

	m_pInGame = new cInGame();
	D_SCENEMANAGER->AddScene("InGameScene", m_pInGame);
}

void cMainGame::AddFonts()
{
	D_FONTMANAGER->AddFont("TimerFont", "1»∆«œæ·∞ÌæÁ¿Ã R", 14, 7);
}

void cMainGame::AddSprites()
{
	D_SPRITEMANAGER->AddSprite("±Ëø¨æ∆", "ya.jpg"); 
	D_SPRITEMANAGER->AddFrameSprite("πÃªÁ¿œ", "missile.bmp", 13, 1);
}

void cMainGame::AddSounds()
{
}
