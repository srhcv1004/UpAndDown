#include "stdafx.h"
#include "cAllocateHierarchy.h"

cAllocateHierarchy::cAllocateHierarchy()
{
}

cAllocateHierarchy::~cAllocateHierarchy()
{
}

HRESULT cAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
{
	ST_BONE* pNewBone = new ST_BONE();
	ZeroMemory(pNewBone, sizeof(ST_BONE));

	D3DXMatrixIdentity(&pNewBone->matWorldTM);
	D3DXMatrixIdentity(&pNewBone->TransformationMatrix);

	if (Name)
	{
		int nNameLength = strlen(Name) + 1;
		pNewBone->Name = new CHAR[nNameLength]();
		strncpy_s(pNewBone->Name, nNameLength,
			Name, nNameLength);
	}

	(*ppNewFrame) = pNewBone;

	return S_OK;
}

HRESULT cAllocateHierarchy::CreateMeshContainer(LPCSTR Name, CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD NumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	ST_BONEMESH* pNewBoneMesh = new ST_BONEMESH();
	ZeroMemory(pNewBoneMesh, sizeof(ST_BONEMESH));

	pNewBoneMesh->MeshData.pMesh = pMeshData->pMesh;
	pNewBoneMesh->MeshData.Type = pMeshData->Type;
	pNewBoneMesh->pMeshOrigin = pMeshData->pMesh;

	pMeshData->pMesh->AddRef();
	IDirect3DDevice9* pDevice = NULL;
	pMeshData->pMesh->GetDevice(&pDevice);

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		D3DXMATERIAL sNewMaterial;
		ZeroMemory(&sNewMaterial, sizeof(D3DXMATERIAL));

		memcpy_s(&sNewMaterial, sizeof(D3DXMATERIAL), 
			pMaterials, sizeof(D3DXMATERIAL));
		pNewBoneMesh->vecMaterial.push_back(sNewMaterial.MatD3D);

		IDirect3DTexture9* pNewTexture = NULL;
		if (sNewMaterial.pTextureFilename)
		{
			std::string strFullName = std::string("XFile/Texture/") +
				std::string(sNewMaterial.pTextureFilename);
			D3DXCreateTextureFromFile(D_DEVICE, strFullName.c_str(), &pNewTexture);
		}
		pNewBoneMesh->vecTexture.push_back(pNewTexture);
	}

	if (pSkinInfo)
	{
		pNewBoneMesh->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMeshData->pMesh->GetFVF(),
			pDevice,
			&pNewBoneMesh->MeshData.pMesh);

		pMeshData->pMesh->GetAttributeTable(NULL, &pNewBoneMesh->ulAttributeCount);
		pNewBoneMesh->pAttributeInfo = new D3DXATTRIBUTERANGE[pNewBoneMesh->ulAttributeCount]();
		pMeshData->pMesh->GetAttributeTable(pNewBoneMesh->pAttributeInfo, NULL);

		int nBoneCount = pSkinInfo->GetNumBones();

		pNewBoneMesh->pMatOffset = new D3DXMATRIXA16[nBoneCount]();
		pNewBoneMesh->pMatCurrent = new D3DXMATRIXA16[nBoneCount]();

		for (int i = 0; i < nBoneCount; i++)
		{
			pNewBoneMesh->pMatOffset[i] = (*(pSkinInfo->GetBoneOffsetMatrix(i)));
		}
	}

	(*ppNewMeshContainer) = pNewBoneMesh;

	return S_OK;
}

HRESULT cAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	D_SAFE_DELETE_ARR(pFrameToFree->Name);
	D_SAFE_DELETE(pFrameToFree);
	return S_OK;
}

HRESULT cAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONEMESH* pBoneMesh = (ST_BONEMESH*)pMeshContainerToFree;

	D_SAFE_DELETE_ARR(pBoneMesh->ppMat);
	D_SAFE_DELETE_ARR(pBoneMesh->pMatOffset);
	D_SAFE_DELETE_ARR(pBoneMesh->pMatCurrent);

	//D_SAFE_DELETE_ARR(pBoneMesh->Name);
	D_SAFE_DELETE_ARR(pBoneMesh->pAttributeInfo);

	//D_SAFE_DELETE_ARR(pBoneMesh->pAdjacency);
	//D_SAFE_DELETE_ARR(pBoneMesh->pMaterials);
	//D_SAFE_DELETE_ARR(pBoneMesh->pEffects);
	//D_SAFE_DELETE_ARR(pBoneMesh->pNextMeshContainer);

	D_SAFE_RELEASE(pBoneMesh->pSkinInfo);
	D_SAFE_RELEASE(pBoneMesh->pMeshOrigin);
	D_SAFE_RELEASE(pBoneMesh->MeshData.pMesh);

	for (size_t i = 0; i < pBoneMesh->vecTexture.size(); i++)
	{
		D_SAFE_RELEASE(pBoneMesh->vecTexture[i]);
	}
	pBoneMesh->vecTexture.clear();
	pBoneMesh->vecMaterial.clear();

	pBoneMesh->NumMaterials = 0;
	pBoneMesh->ulAttributeCount = 0;

	return S_OK;
}
