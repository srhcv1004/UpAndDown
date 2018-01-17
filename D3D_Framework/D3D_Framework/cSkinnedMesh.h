#pragma once
#include "cCharacter.h"

class cSkinnedMesh : public cCharacter
{
protected:
	D3DXFRAME*												m_pRootBone;

protected:
	ID3DXMesh*												m_pSphere;

protected:
	ID3DXAnimationController*								m_pAnimationController;

private:
	void SetupSkinnedMesh(ST_BONE* pBone);

private:
	void DestroyAll(ST_BONE* pBone);

private:
	void RenderSkinnedMesh(ST_BONE* pBone);
	void RenderBones(ST_BONE* pBone, D3DXMATRIXA16* pMatWorld);

public:
	cSkinnedMesh();
	virtual ~cSkinnedMesh();

	virtual void Setup(const CHAR* pFolderName, const CHAR* pFileName);
	virtual void Release();
	virtual void Update();
	virtual void Render();
};

