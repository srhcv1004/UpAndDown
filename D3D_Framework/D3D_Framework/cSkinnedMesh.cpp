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
		0.5F,
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
	this->RenderSkinnedMesh((ST_BONE*)m_pRootBone);
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
