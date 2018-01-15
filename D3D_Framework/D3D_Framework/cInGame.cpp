#include "stdafx.h"
#include "cInGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cSprite.h"
#include "cXFileObj.h"
#include "cXFilePlayer.h"
#include "cProgressBar.h"

cInGame::cInGame()
	: m_pCamera(NULL)
	, m_pLineGrid(NULL)
	, m_pXFilePlayer(NULL)
	, m_pProgressBar(NULL)
	, m_fMax(0.F)
	, m_fCur(0.F)
{
}

cInGame::~cInGame()
{
}

void cInGame::Setup()
{
	m_pCamera = new cCamera();
	m_pCamera->Setup();

	m_pLineGrid = new cLineGrid();
	m_pLineGrid->Setup();

	m_pProgressBar = new cProgressBar();
	m_pProgressBar->Setup("체력바_앞", "Images/Sample/Texture/frontBar.png",
		"체력바_뒤", "Images/Sample/Texture/backBar.png", 10, 10, 200, 20);

	m_fMax = 100.F;
	m_fCur = 100.F;
	m_pProgressBar->SetGuage(m_fCur, m_fMax);

	D_SOUNDMANAGER->AddSound("배경음악", "Sounds/하나땅-소나기.mp3", TRUE, TRUE);
	D_SOUNDMANAGER->Play("배경음악", cSoundManager::E_GROUP_BGM);
}

void cInGame::Release()
{
 	D_SAFE_RELEASE(m_pCamera);
	D_SAFE_DELETE(m_pCamera);

	D_SAFE_RELEASE(m_pLineGrid);
	D_SAFE_DELETE(m_pLineGrid);

	D_SAFE_RELEASE(m_pXFilePlayer);
	D_SAFE_DELETE(m_pXFilePlayer);
}

void cInGame::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	if (m_pLineGrid)
		m_pLineGrid->Update();

	static float fVolume = 1.F;
	D_SOUNDMANAGER->SetGroupVolume(cSoundManager::E_GROUP_BGM, fVolume);
	
	if (D_KEYMANAGER->IsOnceKeyDown(VK_F11))
	{
		fVolume -= 0.1F;
	}
	if (D_KEYMANAGER->IsOnceKeyDown(VK_F12))
	{
		fVolume += 0.1F;
	}

	if (D_KEYMANAGER->IsStayKeyDown(VK_F1))
	{
		m_fCur -= 0.5F;
	}
	if (D_KEYMANAGER->IsStayKeyDown(VK_F2))
	{
		m_fCur += 0.5F;
	}

	m_pProgressBar->SetGuage(m_fCur, m_fMax);
}

void cInGame::Render()
{
	if (m_pLineGrid)
		m_pLineGrid->Render();

	m_pProgressBar->Render();
}