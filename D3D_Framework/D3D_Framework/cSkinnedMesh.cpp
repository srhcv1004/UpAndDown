#include "stdafx.h"
#include "cSkinnedMesh.h"
#include "cAllocateHierarchy.h"

cSkinnedMesh::cSkinnedMesh()
	: m_pRootBone(NULL)
	, m_pAnimationController(NULL)
	, m_pSphere(NULL)
{
}

cSkinnedMesh::~cSkinnedMesh()
{
}

void cSkinnedMesh::Setup(const CHAR* pFolderName, const CHAR* pFileName)
{
	std::string strFullName = std::string(pFolderName);
	strFullName = strFullName + std::string("/")
		+ std::string(pFileName);

	cAllocateHierarchy cHierarchy;
	cHierarchy.SetFolderName(pFolderName);

	D3DXLoadMeshHierarchyFromX(strFullName.c_str(),
		D3DXMESH_MANAGED,
		D_DEVICE,
		&cHierarchy,
		NULL,
		&m_pRootBone,
		&m_pAnimationController);

	this->SetupSkinnedMesh((ST_BONE*)m_pRootBone);

	D3DXCreateSphere(D_DEVICE,
		0.05F,
		10,
		10,
		&m_pSphere,
		NULL);
}

void cSkinnedMesh::Release()
{
	this->DestroyAll((ST_BONE*)m_pRootBone);
}

void cSkinnedMesh::Update()
{
	UpdateMatWorldTM((ST_BONE*)m_pRootBone, &m_matWorldTM);
}

void cSkinnedMesh::Render()
{
	//this->RenderBones((ST_BONE*)m_pRootBone, NULL);
	//this->RenderSkinnedMesh((ST_BONE*)m_pRootBone);

	D3DXMATRIXA16 matW;
	D3DXMatrixIdentity(&matW);

	RenderBoneLines((ST_BONE*)m_pRootBone, NULL, &matW);
}

void cSkinnedMesh::SetupSkinnedMesh(ST_BONE* pBone)
{
	if (pBone->pMeshContainer)
	{
		ST_BONEMESH* pBoneMesh = (ST_BONEMESH*)pBone->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			DWORD dwBoneCount = pBoneMesh->pSkinInfo->GetNumBones();

			for (DWORD i = 0; i < dwBoneCount; i++)
			{
				ST_BONE* pTmpBone = (ST_BONE*)D3DXFrameFind(m_pRootBone,
					pBoneMesh->pSkinInfo->GetBoneName(i));

				if (pTmpBone)
				{
					pBoneMesh->ppMat[i] = &pTmpBone->matWorldTM;
				}
				else
				{
					pBoneMesh->ppMat[i] = NULL;
				}
			}
		}
	}

	if (pBone->pFrameSibling)
	{
		SetupSkinnedMesh((ST_BONE*)pBone->pFrameSibling);
	}
	if (pBone->pFrameFirstChild)
	{
		SetupSkinnedMesh((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::RenderSkinnedMesh(ST_BONE* pBone)
{
	if (pBone->pMeshContainer)
	{
		ST_BONEMESH* pBoneMesh = (ST_BONEMESH*)pBone->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			DWORD dwBoneCount = pBoneMesh->pSkinInfo->GetNumBones();

			for (DWORD i = 0; i < dwBoneCount; i++)
			{
				D3DXMatrixMultiply(&pBoneMesh->pMatCurrent[i],
					&pBoneMesh->pMatOffset[i],
					pBoneMesh->ppMat[i]);
			}

			BYTE* pDst = NULL;
			BYTE* pSrc = NULL;

			pBoneMesh->pMeshOrigin->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&pSrc);
			pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (LPVOID*)&pDst);

			pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
				pBoneMesh->pMatCurrent,
				NULL,
				pSrc,
				pDst);

			pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			pBoneMesh->pMeshOrigin->UnlockVertexBuffer();

			for (DWORD i = 0; i < pBoneMesh->dwAttributeCount; i++)
			{
				DWORD nAttributeID = pBoneMesh->pAttribute[i].AttribId;

				D_DEVICE->SetMaterial(&pBoneMesh->vecMaterial[nAttributeID]);
				D_DEVICE->SetTexture(0, pBoneMesh->vecTexture[nAttributeID]);

				pBoneMesh->MeshData.pMesh->DrawSubset(nAttributeID);
			}
		}
	}

	if (pBone->pFrameSibling)
	{
		RenderSkinnedMesh((ST_BONE*)pBone->pFrameSibling);
	}
	if (pBone->pFrameFirstChild)
	{
		RenderSkinnedMesh((ST_BONE*)pBone->pFrameFirstChild);
	}
}

void cSkinnedMesh::RenderBones(ST_BONE* pBone, D3DXMATRIXA16* pMatWorld)
{
	D_DEVICE->SetTransform(D3DTS_WORLD, &pBone->matWorldTM);
	m_pSphere->DrawSubset(0);

	if (pBone->pFrameSibling)
	{
		RenderBones((ST_BONE*)pBone->pFrameSibling, pMatWorld);
	}
	if (pBone->pFrameFirstChild)
	{
		RenderBones((ST_BONE*)pBone->pFrameFirstChild, pMatWorld);
	}
}

void cSkinnedMesh::RenderBoneLines(ST_BONE* pBone, ST_BONE* pParent, D3DXMATRIXA16* matWorld)
{
   if (!pBone) pBone = (ST_BONE*)m_pRootBone;

   D3DXMATRIXA16   r, s, t;
   D3DXMatrixRotationYawPitchRoll(&r, -D3DX_PI * 0.5f, 0.0f, 0.0f);

   if (pParent && pBone->Name && pParent->Name)
   {
      D_DEVICE->SetRenderState(D3DRS_LIGHTING, false);
      s = r * pBone->matWorldTM * (*matWorld);
      D3DXMatrixTranslation(&t, s(3, 0), s(3, 1), s(3, 2));
	  D_DEVICE->SetTransform(D3DTS_WORLD, &t);
      m_pSphere->DrawSubset(0);

      D3DXMATRIXA16 w1 = pBone->matWorldTM;
	  D3DXMATRIXA16 w2 = pParent->matWorldTM;

      D3DXVECTOR3 thisBone = D3DXVECTOR3(w1(3, 0), w1(3, 1), w1(3, 2));
      D3DXVECTOR3 parentBone = D3DXVECTOR3(w2(3, 0), w2(3, 1), w2(3, 2));

	  D_DEVICE->SetTransform(D3DTS_WORLD, matWorld);

	  ST_PC_VERTEX vertex[] = { ST_PC_VERTEX(parentBone, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f)),
		  ST_PC_VERTEX(thisBone, D3DXCOLOR(0.f, 0.f, 0.f, 1.0f)) };

	  D_DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	  D_DEVICE->SetFVF(ST_PC_VERTEX::FVF);
	  D_DEVICE->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, &vertex[0], sizeof(ST_PC_VERTEX));
   }

   if (pBone->pFrameSibling)
   {
	   RenderBoneLines((ST_BONE*)pBone->pFrameSibling, pParent, matWorld);
   }
   if (pBone->pFrameFirstChild)
   {
	   RenderBoneLines((ST_BONE*)pBone->pFrameFirstChild, pBone, matWorld);
   }
}

void cSkinnedMesh::DestroyAll(ST_BONE* pBone)
{
	cAllocateHierarchy cHierarchy;

	if (pBone->pFrameSibling)
	{
		DestroyAll((ST_BONE*)pBone->pFrameSibling);
	}
	if (pBone->pFrameFirstChild)
	{
		DestroyAll((ST_BONE*)pBone->pFrameFirstChild);
	}

	cHierarchy.DestroyFrame((D3DXFRAME*)pBone);
	cHierarchy.DestroyMeshContainer((D3DXMESHCONTAINER*)pBone->pMeshContainer);
}
