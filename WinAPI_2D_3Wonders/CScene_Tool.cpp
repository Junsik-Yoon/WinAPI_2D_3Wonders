#include "pch.h"
#include "CScene_Tool.h"
#include "CTile.h"
#include "CScene.h"
#include "resource.h"
#include "CD2DImage.h"
#include "commdlg.h"
#include "CUI.h"
#include "CButtonUI.h"
#include "CPanelUI.h"
#include "CTileButton.h"
#include "CCollider.h"
#include "CBG1.h"

INT_PTR CALLBACK TileWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

CScene_Tool::CScene_Tool()
{
    m_pMap = nullptr;
    m_gTile = GROUP_TILE::NONE;
    //TODO: 카메라를 target지정했을 때 툴씬에서도 target이 안풀림
    m_hWnd = 0;
    m_iIdx = 0;
    m_velocity = 500;
    m_iTileX = 0;
    m_iTileY = 0;
}

CScene_Tool::~CScene_Tool()
{

}



void CScene_Tool::update()
{
    CScene::update();

    if (KEYDOWN(VK_RETURN))
    {
        CEventManager::getInst()->EventChangeScene(GROUP_SCENE::STAGE1);
    }
    if (KEY('A'))
    {
        CCameraManager::getInst()->Scroll(Vec2(-1, 0), m_velocity);
    }
    if (KEY('D'))
    {
        CCameraManager::getInst()->Scroll(Vec2(1, 0), m_velocity);
    }
    if (KEY('W'))
    {
        CCameraManager::getInst()->Scroll(Vec2(0, -1), m_velocity);
    }
    if (KEY('S'))
    {
        CCameraManager::getInst()->Scroll(Vec2(0, 1), m_velocity);
    }

    SetTileIdx();
    SetTileGroup();
}

void CScene_Tool::render()
{
    const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
    for (UINT i = 0; i < vecTile.size(); i++)
    {
        vecTile[i]->render();
    }

    PrintMap();
    PrintTileLine();
    PrintTileGroup();

    const vector<CGameObject*>& vecUI = GetGroupObject(GROUP_GAMEOBJ::UI);
    for (UINT i = 0; i < vecUI.size(); i++)
    {
        vecUI[i]->render();
    }
}

void buttonItemClicked(DWORD_PTR param1, DWORD_PTR param2)
{

}
void buttonMapClicked(DWORD_PTR param1, DWORD_PTR param2)
{

}

void CScene_Tool::Enter()
{
    //CBG1* pBG = new CBG1();
    //pBG->SetScale(Vec2(1.f, 1.f));
    //pBG->SetPos(Vec2(0.f, 0.f));
    //AddObject(pBG, GROUP_GAMEOBJ::BACKGROUND);

    m_hWnd = CreateDialog(hInst, MAKEINTRESOURCE(IDD_TILEBOX), hWnd, TileWndProc);
    ShowWindow(m_hWnd, SW_SHOW);
    CreateTile(10, 10);
    CreateTilePanel();

    CCameraManager::getInst()->SetLookAt(Vec2(float(WINSIZEX / 2.f), float(WINSIZEY / 2.f)));

}

void CScene_Tool::Exit()
{
    EndDialog(m_hWnd, IDOK);
    DeleteAll();
}


void CScene_Tool::SetTileIdx()
{
    if (KEY(VK_LBUTTON) || KEY(VK_RBUTTON))
    {
        Vec2 fptMousePos = MousePos();
        fptMousePos = CCameraManager::getInst()->GetRealPos(fptMousePos);

        int iTileX = m_iTileX;
        int iTileY = m_iTileY;

        int iCol = (int)fptMousePos.x / CTile::SIZE_TILE;
        int iRow = (int)fptMousePos.y / CTile::SIZE_TILE;

        if (fptMousePos.x < 0.f || iTileX <= iCol ||
            fptMousePos.y < 0.f || iTileY <= iRow)
        {
            return;		// 타일이 없는 위치 무시
        }

        UINT iIdx = iRow * iTileX + iCol;
        const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
        if (KEY(VK_LBUTTON))
            ((CTile*)vecTile[iIdx])->SetImgIdx(m_iIdx);
        else if (KEY(VK_RBUTTON))
            ((CTile*)vecTile[iIdx])->SetImgIdx(0);
    }
}
    
void CScene_Tool::SetTileGroup()
{
    if (KEY(VK_LBUTTON) || KEY(VK_RBUTTON))
    {
        Vec2 fptMousePos = MousePos();
        fptMousePos = CCameraManager::getInst()->GetRealPos(fptMousePos);

        int iTileX = m_iTileX;
        int iTileY = m_iTileY;

        int iCol = (int)fptMousePos.x / CTile::SIZE_TILE;
        int iRow = (int)fptMousePos.y / CTile::SIZE_TILE;

        if (fptMousePos.x < 0.f || iTileX <= iCol ||
            fptMousePos.y < 0.f || iTileY <= iRow)
        {
            return;		// 타일이 없는 위치 무시
        }

        UINT iIdx = iRow * iTileX + iCol;
        const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
        if (KEY(VK_LBUTTON))
            ((CTile*)vecTile[iIdx])->SetGroup(m_gTile);
        else if (KEY(VK_RBUTTON))
            ((CTile*)vecTile[iIdx])->SetGroup(GROUP_TILE::NONE);
    }
}





void CScene_Tool::CreateTile(UINT xSize, UINT ySize)
{
    DeleteGroup(GROUP_GAMEOBJ::TILE);

    m_iTileX = xSize;
    m_iTileY = ySize;

    CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"Tile", L"texture\\Tile\\tilemap.bmp");

    for (UINT y = 0; y < ySize; y++)
    {
        for (UINT x = 0; x < xSize; x++)
        {
            CTile* pTile = new CTile();
            pTile->SetPos(Vec2((float)(x * CTile::SIZE_TILE), (float)(y * CTile::SIZE_TILE)));
            pTile->SetX(x);
            pTile->SetY(y);
            pTile->SetD2DImage(pImg);
            AddObject(pTile, GROUP_GAMEOBJ::TILE);
        }
    }
}

void CScene_Tool::SaveTile(const wstring& strPath)
{
    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"wb");		// w : write, b : binary
    assert(pFile);

    UINT xCount = m_iTileX;
    UINT yCount = m_iTileY;
    UINT tileCount = 0;

    const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);

    for (UINT i = 0; i < vecTile.size(); i++)
    {
        CTile* pTile = (CTile*)vecTile[i];
        if (0 != pTile->GetIdx() || GROUP_TILE::NONE != pTile->GetGroup())
            tileCount++;
    }

    fwrite(&xCount, sizeof(UINT), 1, pFile);
    fwrite(&yCount, sizeof(UINT), 1, pFile);
    fwrite(&tileCount, sizeof(UINT), 1, pFile);

    for (UINT i = 0; i < vecTile.size(); i++)
    {
        CTile* pTile = (CTile*)vecTile[i];
        if (0 != pTile->GetIdx() || GROUP_TILE::NONE != pTile->GetGroup())
            ((CTile*)vecTile[i])->Save(pFile);
    }

    fclose(pFile);
}

void CScene_Tool::LoadTile(const wstring& strPath)
{
    DeleteGroup(GROUP_GAMEOBJ::TILE);

    FILE* pFile = nullptr;

    _wfopen_s(&pFile, strPath.c_str(), L"rb");      // w : write, b : binary
    assert(pFile);

    UINT xCount = 0;
    UINT yCount = 0;
    UINT tileCount = 0;

    fread(&xCount, sizeof(UINT), 1, pFile);
    fread(&yCount, sizeof(UINT), 1, pFile);
    fread(&tileCount, sizeof(UINT), 1, pFile);

    CreateTile(xCount, yCount);

    const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
    CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"Tile", L"texture\\tile\\tilemap.bmp");
    CTile* pTile = new CTile;

    for (UINT i = 0; i < tileCount; i++)
    {
        pTile->Load(pFile);
        UINT iIdx = pTile->GetY() * xCount + pTile->GetX();
        ((CTile*)vecTile[iIdx])->SetX(pTile->GetX());
        ((CTile*)vecTile[iIdx])->SetY(pTile->GetY());
        ((CTile*)vecTile[iIdx])->SetImgIdx(pTile->GetIdx());
        ((CTile*)vecTile[iIdx])->SetGroup(pTile->GetGroup());
    }

    fclose(pFile);
}

void CScene_Tool::SaveTileData()
{
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
    ofn.hwndOwner = hWnd;					// 부모 윈도우 지정.
    wchar_t szName[256] = {};
    ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
    ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
    ofn.lpstrFilter = L"ALL\0*.*\0tile\0*.tile"; // 필터 설정
    ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
    ofn.lpstrFileTitle = nullptr; // 타이틀 바
    ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
    wstring strTileFolder = CPathManager::getInst()->GetContentPath();
    strTileFolder += L"tile";
    ofn.lpstrInitialDir = strTileFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

    if (GetSaveFileName(&ofn))
    {
        SaveTile(szName);
    }
}

void CScene_Tool::LoadTileData()
{
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
    ofn.hwndOwner = hWnd; // 부모 윈도우 지정.
    wchar_t szName[256] = {};
    ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
    ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
    ofn.lpstrFilter = L"ALL\0*.*\0tile\0*.tile"; // 필터 설정
    ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
    ofn.lpstrFileTitle = nullptr; // 타이틀 바
    ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
    wstring strTileFolder = CPathManager::getInst()->GetContentPath();
    strTileFolder += L"tile";
    ofn.lpstrInitialDir = strTileFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

    if (GetOpenFileName(&ofn))
    {
        LoadTile(szName);
    }
}

void CScene_Tool::LoadMap()
{
    OPENFILENAME ofn = {};

    ofn.lStructSize = sizeof(OPENFILENAME);  // 구조체 사이즈.
    ofn.hwndOwner = hWnd; // 부모 윈도우 지정.
    wchar_t szName[256] = {};
    ofn.lpstrFile = szName; // 나중에 완성된 경로가 채워질 버퍼 지정.
    ofn.nMaxFile = sizeof(szName); // lpstrFile에 지정된 버퍼의 문자 수.
    ofn.lpstrFilter = L"ALL\0*.*\0Image\0*.png"; // 필터 설정
    ofn.nFilterIndex = 0; // 기본 필터 세팅. 0는 all로 초기 세팅됨. 처음꺼.
    ofn.lpstrFileTitle = nullptr; // 타이틀 바
    ofn.nMaxFileTitle = 0; // 타이틀 바 문자열 크기. nullptr이면 0.
    wstring strTileFolder = CPathManager::getInst()->GetContentPath();
    strTileFolder += L"map";
    ofn.lpstrInitialDir = strTileFolder.c_str(); // 초기경로. 우리는 타일 저장할거기 때문에, content->tile 경로로 해두자.
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST; // 스타일

    if (GetOpenFileName(&ofn))
    {
        m_pMap = CResourceManager::getInst()->LoadToolD2DImage(szName);
    }
}
void ClickTileGroupButton(DWORD_PTR param1, DWORD_PTR param2)
{
    // param1 : Scene_tool
    // param2 : CButtonUI
    CScene_Tool* scene_tool = (CScene_Tool*)param1;
    CButtonUI* button = (CButtonUI*)param2;
    scene_tool->ClickTileGroup(button);
}
void CScene_Tool::ClickTileGroup(CButtonUI* button)
{//TODO:수정
    if (m_gTile == GROUP_TILE::NONE)
    {
        m_gTile = GROUP_TILE::GROUND;
        button->SetText(L"GROUND");
    }
    else if (m_gTile == GROUP_TILE::GROUND)
    {
        m_gTile = GROUP_TILE::WALL;
        button->SetText(L"WALL");
    }
    else if (m_gTile == GROUP_TILE::WALL)
    {
        m_gTile = GROUP_TILE::PLATFORM;
        button->SetText(L"PLATFORM");
    }
    else if (m_gTile == GROUP_TILE::PLATFORM)
    {
        m_gTile = GROUP_TILE::SLOPE;
        button->SetText(L"SLOPE");
    }
    else if (m_gTile == GROUP_TILE::SLOPE)
    {
        m_gTile = GROUP_TILE::SLOPEUP;
        button->SetText(L"SLOPE_UP");
    }
    else if (m_gTile == GROUP_TILE::SLOPEUP)
    {
        m_gTile = GROUP_TILE::SLOPEDOWN;
        button->SetText(L"SLOPE_DOWN");
    }
    else if (m_gTile == GROUP_TILE::SLOPEDOWN)
    {
        m_gTile = GROUP_TILE::NONE;
        button->SetText(L"NONE");
    }
}

void ClickTileButton(DWORD_PTR param1, DWORD_PTR param2)
{
    // param1 : Scene_tool
    // param2 : CTileButton
    CScene_Tool* scene_tool = (CScene_Tool*)param1;
    CTileButton* button = (CTileButton*)param2;
    scene_tool->ClickTile(button);
}


void CScene_Tool::ClickTile(CTileButton* button)
{
    SetIdx(button->GetIdx());
}
void CScene_Tool::CreateTilePanel()
{
    CPanelUI* panelTile = new CPanelUI;
    panelTile->SetName(L"panelTile");
    panelTile->SetScale(Vec2(300.f, 300.f));
    panelTile->SetPos(Vec2(50.f,50.f));

    CD2DImage* pImg = CResourceManager::getInst()->LoadD2DImage(L"Tile", L"texture\\tile\\tilemap.bmp");
    for (UINT y = 0; y < 5; y++)
    {
        for (UINT x = 0; x < 5; x++)
        {
            CTileButton* btnTile = new CTileButton;
            btnTile->SetScale(Vec2(CTile::SIZE_TILE, CTile::SIZE_TILE));
            btnTile->SetPos(Vec2((float)x * CTile::SIZE_TILE, (float)y * CTile::SIZE_TILE));
            btnTile->SetPos(btnTile->GetPos() + Vec2(8.f, 8.f));
            btnTile->SetImage(pImg);
            btnTile->SetIdx(y * 5 + x);
            btnTile->SetClickedCallBack(ClickTileButton, (DWORD_PTR)this, (DWORD_PTR)btnTile);
            panelTile->AddChild(btnTile);
        }
    }

    CButtonUI* btnTileGroup = new CButtonUI;
    btnTileGroup->SetScale(Vec2(100.f, 50.f));
    btnTileGroup->SetPos(Vec2(50.f, 250.f));
    btnTileGroup->SetText(L"NONE");
    btnTileGroup->SetClickedCallBack(ClickTileGroupButton, (DWORD_PTR)this, (DWORD_PTR)btnTileGroup);
    panelTile->AddChild(btnTileGroup);

    AddObject(panelTile, GROUP_GAMEOBJ::UI);
}

void CScene_Tool::PrintMap()
{
    if (nullptr == m_pMap)
        return;

    Vec2 pos = CCameraManager::getInst()->GetLookAt();
    pos = pos - Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);

    CRenderManager::getInst()->RenderImage(
        m_pMap,
        0 - pos.x,
        0 - pos.y,
        m_pMap->GetWidth() - pos.x,
        m_pMap->GetHeight() - pos.y
    );
}

void CScene_Tool::PrintTileLine()
{
    Vec2 pos = CCameraManager::getInst()->GetLookAt();
    pos = pos - Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);

    // 가로줄 출력
    for (UINT y = 0; y <= m_iTileY; y++)
    {
        CRenderManager::getInst()->RenderLine(
            Vec2(0 - pos.x, y * CTile::SIZE_TILE - pos.y),
            Vec2(CTile::SIZE_TILE * m_iTileX - pos.x, y * CTile::SIZE_TILE - pos.y)
        );
    }

    // 세로줄 출력
    for (UINT x = 0; x <= m_iTileX; x++)
    {
        CRenderManager::getInst()->RenderLine(
            Vec2(x * CTile::SIZE_TILE - pos.x, 0 - pos.y),
            Vec2(x * CTile::SIZE_TILE - pos.x, CTile::SIZE_TILE * m_iTileY - pos.y)
        );
    }
}

void CScene_Tool::PrintTileGroup()
{
    Vec2 pos = CCameraManager::getInst()->GetLookAt();
    pos = pos - Vec2(WINSIZEX / 2.f, WINSIZEY / 2.f);

    const vector<CGameObject*>& vecTile = GetGroupObject(GROUP_GAMEOBJ::TILE);
    CTile* pTile;

    for (UINT i = 0; i < vecTile.size(); i++)
    {
        pTile = (CTile*)vecTile[i];
        if (GROUP_TILE::GROUND == pTile->GetGroup())
        {
            CRenderManager::getInst()->RenderEllipse(
                pTile->GetPos().x + CTile::SIZE_TILE / 2.f - pos.x,
                pTile->GetPos().y + CTile::SIZE_TILE / 2.f - pos.y,
                CTile::SIZE_TILE / 2.f,
                CTile::SIZE_TILE / 2.f,
                RGB(255, 0, 0),
                3.f
            );
        }
        else if (GROUP_TILE::WALL == pTile->GetGroup())
        {
            CRenderManager::getInst()->RenderEllipse(
                pTile->GetPos().x + CTile::SIZE_TILE / 2.f - pos.x,
                pTile->GetPos().y + CTile::SIZE_TILE / 2.f - pos.y,
                CTile::SIZE_TILE / 2.f,
                CTile::SIZE_TILE / 2.f,
                RGB(0, 255, 0),
                3.f
            );
        }
        else if (GROUP_TILE::PLATFORM == pTile->GetGroup())
        {
            CRenderManager::getInst()->RenderEllipse(
                pTile->GetPos().x + CTile::SIZE_TILE / 2.f - pos.x,
                pTile->GetPos().y + CTile::SIZE_TILE / 2.f - pos.y,
                CTile::SIZE_TILE / 2.f,
                CTile::SIZE_TILE / 2.f,
                RGB(0, 0, 0),
                3.f
            );
        }
        else if (GROUP_TILE::SLOPEUP == pTile->GetGroup())
        {
            CRenderManager::getInst()->RenderEllipse(
                pTile->GetPos().x + CTile::SIZE_TILE / 2.f - pos.x,
                pTile->GetPos().y + CTile::SIZE_TILE / 2.f - pos.y,
                CTile::SIZE_TILE / 2.f,
                CTile::SIZE_TILE / 2.f,
                RGB(0, 0, 255),
                3.f
            );
        }
        else if (GROUP_TILE::SLOPEDOWN == pTile->GetGroup())
        {
            CRenderManager::getInst()->RenderEllipse(
                pTile->GetPos().x + CTile::SIZE_TILE / 2.f - pos.x,
                pTile->GetPos().y + CTile::SIZE_TILE / 2.f - pos.y,
                CTile::SIZE_TILE / 2.f,
                CTile::SIZE_TILE / 2.f,
                RGB(0, 255, 255),
                3.f
            );
        }
    }
}

INT_PTR CALLBACK TileWndProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SAVE)
        {
            CScene* pCurScene = CSceneManager::getInst()->GetCurScene();

            CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
            assert(pToolScene);

            pToolScene->SaveTileData();

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_LOAD)
        {
            CScene* pCurScene = CSceneManager::getInst()->GetCurScene();

            CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
            assert(pToolScene);

            pToolScene->LoadTileData();

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_MAP)
        {
            CScene* pCurScene = CSceneManager::getInst()->GetCurScene();

            CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
            assert(pToolScene);

            pToolScene->LoadMap();

            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDC_BUTTON_SIZE)
        {
            int x = GetDlgItemInt(hDlg, IDC_EDIT_SIZEX, nullptr, false);
            int y = GetDlgItemInt(hDlg, IDC_EDIT_SIZEY, nullptr, false);

            CScene* pCurScene = CSceneManager::getInst()->GetCurScene();

            CScene_Tool* pToolScene = dynamic_cast<CScene_Tool*>(pCurScene);
            assert(pToolScene);

            pToolScene->DeleteGroup(GROUP_GAMEOBJ::TILE);
            pToolScene->CreateTile(x, y);
        }
        break;
    }
    return (INT_PTR)FALSE;
}