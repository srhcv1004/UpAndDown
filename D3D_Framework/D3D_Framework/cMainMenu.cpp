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
	D_SPRITEMANAGER->AddSprite("메인메뉴", "Images/Sample/Image2D/MainMenu.bmp", D_WINSIZEX, D_WINSIZEY);
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
		D_SPRITEMANAGER->FindSprite("메인메뉴")->GetWidth() / 2.F,
		D_SPRITEMANAGER->FindSprite("메인메뉴")->GetHeight() / 2.F, 
		0.F);

	D_SPRITEMANAGER->Render("메인메뉴", &matT);
}