#pragma once
#include "CScene.h"

class CD2DImage;

class CScene_Tool :
    public CScene
{
private:

    CD2DImage* m_pMap;
    GROUP_TILE m_gTile;

    HWND m_hWnd;
    UINT m_iIdx;
    float m_velocity;

    UINT m_iTileX;
    UINT m_iTileY;

public:
    virtual void Enter();
    virtual void Exit();
    virtual void update();
    virtual void render();

    void SetIdx(UINT idx){ m_iIdx = idx; }
    void SetTileIdx();  // 마우스와 상호작용해서 타일을 바꿈.
    void SetGroup(GROUP_TILE group){ m_gTile = group; }
    void SetTileGroup();
    void CreateTile(UINT xSize, UINT ySize);
    void SaveTile(const wstring& strPath);
    void LoadTile(const wstring& strPath);
    void SaveTileData();
    void LoadTileData();

private:
    void PrintTileLine();
    void PrintTileGroup();

public:
    CScene_Tool();
    ~CScene_Tool();
};
