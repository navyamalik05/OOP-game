/**
 * @file CollisionVisitor.cpp
 * @author Hartejas Sekhon
 * @brief Implementation of the CCollisionVisitor class.
 */

#include "pch.h"
#include "CollisionVisitor.h"
#include <cmath> // For sqrt

// Include headers for all item types involved in collisions
#include "Football.h"
#include "Platform.h"
#include "Coin.h"
#include "Goalpost.h"
#include "Wall.h"
#include "Enemy.h"
#include "Game.h"
#include "Level.h"
#include "Powerup.h"
#include "BossGoalPost.h"

// Add includes for CEnemy, CPowerUp, CWall when needed

/**
 * Constructor. Stores the football and its predicted next state.
 */
CCollisionVisitor::CCollisionVisitor(CFootball* football, double nextX, double nextY, double velY) :
    mFootball(football), mFootballNextX(nextX), mFootballNextY(nextY), mFootballVelY(velY)
{
}

/**
 * Visit a CPlatform object. Check for collision where the football lands on top.
 * @param platform The CPlatform object being visited.
 */
void CCollisionVisitor::VisitPlatform(CPlatform* platform)
{
    // // Prevent detecting collision with the platform we might *already* be slightly inside of
    // if (mFootballVelY < 0)
    // {
    //     return; // Skip collision checks if we're moving upward
    // }

    // platoform boundaries and dimensions
    double pWidth = platform->GetWidth();
    double pHeight = platform->GetHeight();
    double pLeft = platform->GetX() - pWidth / 2.0;
    double pRight = platform->GetX() + pWidth / 2.0;
    double pTop = platform->GetY() - pHeight / 2.0;
    double pBottom = platform->GetY() + pHeight / 2.0;


    // Football boundaries (using predicted next position)
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX - fWidth / 2.0;
    double fRight = mFootballNextX + fWidth / 2.0;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY + fHeight / 2.0;



    //current football boundary from previous frame
    double fCurrentLeft = mFootball->GetX() - fWidth / 2.0;
    double fCurrentRight = mFootball->GetX() + fWidth / 2.0;
    double fCurrentTop = mFootball->GetY() - fHeight / 2.0;
    double fCurrentBottom = mFootball->GetY() + fHeight / 2.0;

    //checking if landing on top
    bool isFalling = mFootballVelY >= 0;
    bool wasAbove = fCurrentBottom <= pTop + 2.0; // 1px tolerance
    bool willBeBelow = fBottom >= pTop - 2.0;

    bool overlapX = (fRight > pLeft && fLeft < pRight); // Horizontally aligned

    if (isFalling && wasAbove && willBeBelow && overlapX)
    {
        // This is a "land on top" collision.
        if (!mHitWallY) // Only if we haven't already hit a lower ceiling
        {
            mOnGround = true;
            mGroundY = pTop;
        }
        return; // We landed. We are done.
    }

    // (If we aren't landing, we check for a head bonk)

    bool isJumping = mFootballVelY < 0;
    bool wasBelow = fCurrentTop >= pBottom - 1.0; // 1px tolerance
    bool willBeAbove = fTop <= pBottom;
    // (use the same overlapX)

    if (isJumping && wasBelow && willBeAbove && overlapX)
    {
        // This is a "head bonk" collision.
        mHitWallY = true;
        mAdjustY = pBottom + fHeight / 2.0;
        return; // We bonked. We are done.
    }

    // (If we didn't land or bonk, we check for side hits)

    // Check for vertical overlap (are we aligned Y-wise?)
    bool overlapY = (fBottom > pTop && fTop < pBottom);
    if (!overlapX || !overlapY)
    {
        return; // No collision at all
    }

    // Check hitting the LEFT side of the platform (football moves left)
    bool wasRight = fCurrentLeft >= pRight - 1.0; // 1px tolerance
    bool willBeLeft = fLeft <= pRight;
    if (wasRight && willBeLeft)
    {
        mHitWallX = true;
        mAdjustX = pRight + fWidth / 2.0;
        return; // Hit left side. Done.
    }

    // Check hitting the RIGHT side of the platform (football moves right)
    bool wasLeft = fCurrentRight <= pLeft + 1.0; // 1px tolerance
    bool willBeRight = fRight >= pLeft;
    if (wasLeft && willBeRight)
    {
    mHitWallX = true;
    mAdjustX = pLeft - fWidth / 2.0;
    return; // Hit right side. Done.
    }
}




/**
 * Visit a CCoin object. Check for simple overlap collision.
 * @param coin The CCoin object being visited.
 */
void CCollisionVisitor::VisitCoin(CCoin* coin)
{
    // Log entry and current state
    wxLogDebug(L"VisitCoin: Checking coin at (%f, %f). IsCollected=%d",
               coin->GetX(), coin->GetY(), (int)coin->IsCollected());

    if (coin->IsCollected()) {
        wxLogDebug(L"  -> VisitCoin: Already collected. Skipping.");
        return;
    }

    // If the coin was just spawned, ignore it for collision
    if (coin->IsSpawnProtected())
    {
        return;
    }


    //Coin boundaries and dimensions
    double cWidth = coin->GetWidth();
    double cHeight = coin->GetHeight();
    double cLeft = coin->GetX() - cWidth / 2;
    double cRight = coin->GetX() + cWidth / 2;
    double cTop = coin->GetY() - cHeight / 2;
    double cBottom = coin->GetY() + cHeight / 2;

    //football dimensions and boundaries using predicted next position
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX- fWidth / 2;
    double fRight = mFootballNextX+ fWidth / 2;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY+ fHeight / 2;

    // chcek for overlap
    bool overlapX = (fRight>cLeft && fLeft<cRight);
    bool overlapY = (fTop < cBottom && fBottom > cTop);

    if (overlapX && overlapY)
    {
        //Collision detected, store the pointer to hit coin
        //store only the first coin hit here
        if (mHitCoin == nullptr)
        {
            mHitCoin = coin;
        }
    }
}



/**
 * Visit a CGoalpost object. Check for distance-based collision.
 * @param goalpost The CGoalpost object being visited.
 */

void CCollisionVisitor::VisitGoalpost(CGoalpost* goalpost)
{
    double dx = mFootballNextX - goalpost->GetX();
    double dy = mFootballNextY - goalpost->GetY();
    double distance = std::sqrt(dx * dx + dy * dy);

    const double CollisionDistance = 50.0;

    if (distance < CollisionDistance)
    {
        mHitGoalpost = true;
    }
}
/// Error tolerance for collision detection
double const Error = 2.0;

/**
 * Visit a CWall object. Check for overlap and determine collision side.
 * @param wall The CWall object being visited.
 */
void CCollisionVisitor::VisitWall(CWall* wall)
{
    // Wall boundaries
    double wWidth = wall->GetWidth();
    double wHeight = wall->GetHeight();
    double wLeft = wall->GetX() - wWidth / 2.0;
    double wRight = wall->GetX() + wWidth / 2.0;
    double wTop = wall->GetY() - wHeight / 2.0;
    double wBottom = wall->GetY() + wHeight / 2.0;

    // Football boundaries (using predicted next position)
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX - fWidth / 2.0;
    double fRight = mFootballNextX + fWidth / 2.0;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY + fHeight / 2.0;

    // Check for overlap
    bool overlapX = (fRight > wLeft && fLeft < wRight);
    bool overlapY = (fBottom > wTop && fTop < wBottom);

    if (overlapX && overlapY)
    {
        // Collision detected, determining the side

        // Calculating penetration depths
        double penLeft = fRight - wLeft;
        double penRight = wRight - fLeft;
        double penTop = fBottom - wTop;
        double penBottom = wBottom - fTop;

        // Find the minimum penetration depth
        double minPenX = std::min(penLeft, penRight);
        double minPenY = std::min(penTop, penBottom);

        // If horizontal penetration is less, it's an X collision
        if (minPenX < minPenY) {
            mHitWallX = true;
            if (penLeft < penRight) {
                mAdjustX = wLeft - fWidth / 2.0;
            } else {
                mAdjustX = wRight + fWidth / 2.0;
            }
        }
        // or, it's a Y collision (hitting top or bottom)
        else
        {
            // A wall is *only* a bonk object. It *never* sets mOnGround.
            mHitWallY = true;
            if (penTop < penBottom) {
                // Hit the top of the wall
                mAdjustY = wTop - fHeight / 2.0;
            } else {
                // Hit the bottom of the wall (head bonk)
                mAdjustY = wBottom + fHeight / 2.0;
            }
        }
    }
}

/**
 * Visit an enemy and handle football-enemy collision logic.
 * @param enemy Pointer to enemy
 */
void CCollisionVisitor::VisitEnemy(CEnemy* enemy)
{
    // --- AABB (rectangle) collision check ---
    double eWidth = enemy->GetWidth();
    double eHeight = enemy->GetHeight();
    double eLeft = enemy->GetX() - eWidth / 2.0;
    double eRight = enemy->GetX() + eWidth / 2.0;
    double eTop = enemy->GetY() - eHeight / 2.0;
    double eBottom = enemy->GetY() + eHeight / 2.0;

    // Football predicted bounds
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX - fWidth / 2.0;
    double fRight = mFootballNextX + fWidth / 2.0;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY + fHeight / 2.0;

    bool overlapX = (fRight > eLeft && fLeft < eRight);
    bool overlapY = (fBottom > eTop && fTop < eBottom);

    if (overlapX && overlapY)
    {
        if (!mFootball->DashLeft() && !mFootball->DashRight() && !enemy->IsKilled())
        {
            mFootball->GetGame()->YouLose();
        }
        else
        {
            enemy->SetKilled();
        }
    }
}

/**
 * Visit a powerup and store pointer if collision detected.
 * @param powerup Pointer to powerup
 */
void CCollisionVisitor::VisitPowerup(CPowerup* powerup)
{
    if (powerup->IsCollected()) {
        return;
    }


    double pWidth = powerup->GetWidth();
    double pHeight = powerup->GetHeight();
    double pLeft = powerup->GetX() - pWidth / 2.0;
    double pRight = powerup->GetX() + pWidth / 2.0;
    double pTop = powerup->GetY() - pHeight / 2.0;
    double pBottom = powerup->GetY() + pHeight / 2.0;

    // Football boundaries (using predicted next position)
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX - fWidth / 2.0;
    double fRight = mFootballNextX + fWidth / 2.0;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY + fHeight / 2.0;

    // Check for overlap (standard AABB collision)
    bool overlapX = (fRight > pLeft && fLeft < pRight);
    bool overlapY = (fBottom > pTop && fTop < pBottom);

    if (overlapX && overlapY)
    {
        if (mHitPowerup == nullptr) {
            mHitPowerup = powerup;
        }
    }
}


/**
 * Visit the boss and check for a dash collision.
 * @param boss Pointer to the boss
 */
void CCollisionVisitor::VisitBossGoalpost(CBossGoalpost* boss)
{
    // Simple AABB overlap check
    double eWidth = boss->GetWidth();
    double eHeight = boss->GetHeight();
    double eLeft = boss->GetX() - eWidth / 2.0;
    double eRight = boss->GetX() + eWidth / 2.0;
    double eTop = boss->GetY() - eHeight / 2.0;
    double eBottom = boss->GetY() + eHeight / 2.0;

    // Football predicted bounds
    double fWidth = mFootball->GetWidth();
    double fHeight = mFootball->GetHeight();
    double fLeft = mFootballNextX - fWidth / 2.0;
    double fRight = mFootballNextX + fWidth / 2.0;
    double fTop = mFootballNextY - fHeight / 2.0;
    double fBottom = mFootballNextY + fHeight / 2.0;

    bool overlapX = (fRight > eLeft && fLeft < eRight);
    bool overlapY = (fBottom > eTop && fTop < eBottom);

    if (overlapX && overlapY)
    {
        // We have a collision!
        // Now, check if the football is dashing
        if (mFootball->DashLeft() || mFootball->DashRight())
        {
            // It's a dash-hit! Store the boss pointer.
            mHitBoss = boss;
        }
        else
        {
            // It's a regular hit. Treat it like a wall
            mHitWallX = true;
            if (mFootball->GetX() < boss->GetX())
            {
                mAdjustX = eLeft - fWidth / 2.0; // Bounce left
            }
            else
            {
                mAdjustX = eRight + fWidth / 2.0; // Bounce right
            }
        }
    }
}

