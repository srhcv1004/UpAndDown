#include "stdafx.h"
#include "cMainMenu.h"
#include "cImage.h"
#include "cSprite.h"

cMainMenu::cMainMenu()
{
}

cMainMenu::~cMainMenu()
{
}

void cMainMenu::Setup()
{
	D_SPRITEMANAGER->AddSprite("���θ޴�", "Images/Sample/Image2D/MainMenu.bmp", D_WINSIZEX, D_WINSIZEY);
}

void cMainMenu::Release()
{
}

void cMainMenu::Update()
{
	if (D_KEYMANAGER->IsOnceKeyDown(VK_SPACE))
	{
		D_SCENEMANAGER->ChangeScene("InGameScene");
	}
}

void cMainMenu::Render()
{
	D3DXMATRIXA16 matT;
	D3DXMatrixTranslation(&matT, 
		D_SPRITEMANAGER->FindSprite("���θ޴�")->GetWidth() / 2.F,
		D_SPRITEMANAGER->FindSprite("���θ޴�")->GetHeight() / 2.F, 
		0.F);

	D_SPRITEMANAGER->Render("���θ޴�", &matT);
}