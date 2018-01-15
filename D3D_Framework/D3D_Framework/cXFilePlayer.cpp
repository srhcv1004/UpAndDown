#include "stdafx.h"
#include "cXFilePlayer.h"

cXFilePlayer::cXFilePlayer()
{
}

cXFilePlayer::~cXFilePlayer()
{
}

void cXFilePlayer::Setup(const CHAR* pXFileName)
{
	cXFileObj::Setup(pXFileName);
}

void cXFilePlayer::Release()
{
	cXFileObj::Release();
}

void cXFilePlayer::Update()
{
	cXFileObj::Update();

	this->KeyControlPlayer();
}

void cXFilePlayer::Render()
{
	cXFileObj::Render();
}

void cXFilePlayer::KeyControlPlayer()
{
	if (D_KEYMANAGER->IsStayKeyDown(VK_LEFT))
	{
		m_fRotY -= 0.1F;
	}
	else if (D_KEYMANAGER->IsStayKeyDown(VK_RIGHT))
	{
		m_fRotY += 0.1F;
	}

	if (D_KEYMANAGER->IsStayKeyDown(VK_UP))
	{
		m_pAnimationController->AdvanceTime(D_TIMEMANAGER->GetElapsedTime(), NULL);
		m_vPosition += m_vDirection * 0.2F;
	}
	else if (D_KEYMANAGER->IsStayKeyDown(VK_DOWN))
	{
		m_pAnimationController->AdvanceTime(D_TIMEMANAGER->GetElapsedTime(), NULL);
		m_vPosition -= m_vDirection * 0.2F;
	}

	D3DXMATRIXA16 matR, matT, matS;

	D3DXMatrixScaling(&matS, 0.05F, 0.05F, 0.05F);

	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	D3DXMatrixRotationY(&matR, m_fRotY);
	m_vDirection = D3DXVECTOR3(1.F, 0.F, 0.F);
	D3DXVec3TransformNormal(&m_vDirection, &m_vDirection, &matR);

	m_matWorldTM = matS * matR * matT;
}