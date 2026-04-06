/**
* @file Enemy.cpp
 * @author Elle Hyry
 * @brief Implementation of CEnemy class
 */

#include "pch.h"
#include "Enemy.h"
#include "ItemVisitor.h"
#include "Football.h"

CEnemy::CEnemy(CGame* game, const std::wstring& filename, double x, double y, EnemyType type)
  : CItem(game, filename, x, y), mType(type)
{
    mStartX = x;
    mStartY = y;
    SetLocation(x, y);
}

void CEnemy::Update(double elapsed)
{
    switch(mType)
    {
        case EnemyType::Patrol:
            if(mMovingRight) { SetLocation(GetX()+mSpeed*elapsed, GetY()); if(GetX()>=mStartX+mPatrolRange)mMovingRight=false; }
            else { SetLocation(GetX()-mSpeed*elapsed, GetY()); if(GetX()<=mStartX)mMovingRight=true; }
            break;

        case EnemyType::Vertical:
            if(mMovingRight) { SetLocation(GetX(), GetY()-mSpeed*elapsed); if(GetY()<=mStartY-mPatrolRange)mMovingRight=false; }
            else { SetLocation(GetX(), GetY()+mSpeed*elapsed); if(GetY()>=mStartY)mMovingRight=true; }
            break;

        case EnemyType::Chaser: break;
        case EnemyType::Stationary: break;
    }
}

void CEnemy::Draw(wxGraphicsContext* graphics)
{
    if(!IsKilled()) CItem::Draw(graphics);
}

void CEnemy::Accept(CItemVisitor* visitor)
{
    visitor->VisitEnemy(this);
}

void CEnemy::Reset()
{
    SetLocation(mStartX, GetY());
    mMovingRight = true;
}

bool CEnemy::CheckCollision(CFootball* football)
{
    double left1=football->GetX(), right1=football->GetX()+football->GetWidth();
    double top1=football->GetY(), bottom1=football->GetY()+football->GetHeight();

    double left2=GetX(), right2=GetX()+GetWidth();
    double top2=GetY(), bottom2=GetY()+GetHeight();

    return !(left1>right2 || right1<left2 || top1>bottom2 || bottom1<top2);
}
