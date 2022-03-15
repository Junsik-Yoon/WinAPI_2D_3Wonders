#include "pch.h"
#include "Map_Start.h"
#include "CD2DImage.h"

Map_Start::Map_Start()
{
    pimg = CResourceManager::getInst()->LoadD2DImage(L"test", L"texture\\map\\Yoshis Island 2.png");
    SetPos(Vec2(0, 0));
    SetScale(Vec2(pimg->GetWidth() * 2.f, pimg->GetHeight() * 2.f));
}

Map_Start::~Map_Start()
{
}

Map_Start* Map_Start::Clone()
{
    return nullptr;
}

void Map_Start::update()
{

}

void Map_Start::render()
{
    Vec2 pos = GetPos();
    Vec2 scale = GetScale();
    pos = CCameraManager::getInst()->GetRenderPos(pos);

    CRenderManager::getInst()->RenderImage(
        pimg,
        pos.x,
        pos.y,
        pos.x + scale.x,
        pos.y + scale.y
    );
}