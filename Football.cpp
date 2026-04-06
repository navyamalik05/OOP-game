/**
 * @file Football.cpp
 * @author Navya Malik, Hartejas Sekhon, Jack Finlan-Hitt
 * @brief Implementation of CFootball player
 */

#include "pch.h"
#include "Football.h"
#include "Game.h"
#include "Platform.h"
#include "CollisionVisitor.h"
#include "Wall.h"
#include "Coin.h"
#include <cmath>
#include "Powerup.h"
#include "BossGoalPost.h"

/// Gravity in pixels per second^2
const double Gravity = 1000.0;

/// Horizontal speed in pixels per second
const double HorizontalSpeed = 500.0;

/// Dash speed in pixels per second
const double DashSpeed = 5000.0;

/// Upward jump speed (negative = up)
const double BounceSpeed = -800.0;

/// Small step animation height
const double StepHeight = 4.0;

/// Step animation frequency (speed of bouncing while walking)
const double StepFrequency = 10.0;



CFootball::CFootball(CGame* game, double x, double y)
    : CItem(game, L"images/footballMid.png", x, y),
      mLeft(false), mRight(false), mOnGround(false),
      mJumping(false), mStepPhase(0), mUsedDoubleJump(false)  // Initialize all member variables
{
    // Load images for different directions
    mImageMid = wxBitmap(L"images/footballMid.png", wxBITMAP_TYPE_ANY);
    mImageLeft = wxBitmap(L"images/footballLeft.png", wxBITMAP_TYPE_ANY);
    mImageLeftDash = wxBitmap(L"images/footballLeftDash.png", wxBITMAP_TYPE_ANY);
    mImageRight = wxBitmap(L"images/footballRight.png", wxBITMAP_TYPE_ANY);
    mImageRightDash = wxBitmap(L"images/footballRightDash.png", wxBITMAP_TYPE_ANY);

    // Start with idle mid image
    mImage = mImageMid;
    mVelocity = Vector(0, 0);
}

void CFootball::OnKeyDown(wxKeyEvent& event)
{
    switch (event.GetKeyCode())
    {
    case WXK_SHIFT:
        if (mFacingLeft && !mDashLeft && mHasDash)
        {
            mVelocity.SetX(-1*DashSpeed);
            mDashLeft = true;
            mDashRight = false;
            break;
        }
        if (!mDashRight && mHasDash)
        {
            mVelocity.SetX(DashSpeed);
            mDashRight = true;
            mDashLeft = false;
            break;
        }
        break;
    case WXK_RIGHT:
    case 'D':
        mRight = true;
        mFacingLeft = false;
        break;

    case WXK_LEFT:
    case 'A':
        mLeft = true;
        mFacingLeft = true;
        break;

    case WXK_SPACE:
    //case WXK_UP:
    case 'W':
        if (mOnGround)
        {
            mVelocity.SetY(BounceSpeed);
            mOnGround = false;
        }
        else if (mHasDoubleJump && !mUsedDoubleJump)
        {
            mVelocity.SetY(BounceSpeed);
            mUsedDoubleJump = true;
        }
        break;

    default:
        break;
    }
    UpdateImage();

}


//
// debugging code
// void CFootball::OnKeyDown(wxKeyEvent& e){
//     // ... your switch ...
//     wxLogMessage("FB DOWN: L=%d R=%d", (int)mLeft, (int)mRight);
//     UpdateImage();
// }

void CFootball::OnKeyUp(wxKeyEvent& event)
{
    switch (event.GetKeyCode())
    {
    case WXK_RIGHT:
    case 'D':
        mRight = false;
        break;

    case WXK_LEFT:
    case 'A':
        mLeft = false;
        break;

    default:
        break;
    }

    UpdateImage();
}

/**
 * Visit a CGoalpost object
 * @param goalpost Goalpost we are visiting
 */
void CFootball::VisitGoalpost(CGoalpost* goalpost)
{
    // Tell the game object to complete the level.
    GetGame()->LevelCompleted();
}

void CFootball::Update(double elapsed)
{
    if (GetGame()->GetGameState() != GameState::Playing)
    {
        return;
    }
    //  Calculate Potential Movement
    // Horizontal movement based on mLeft/mRight flags
    double vx= 0;

    if (mDashRight)
    {
        vx = mVelocity.X() - Gravity * 20 * elapsed;
        if (vx <= 0)
        {
            vx = 0;
            mDashRight = false;
        }
    }
    if (mDashLeft)
    {
        vx = mVelocity.X() + Gravity * 20 * elapsed;
        if (vx >= 0)
        {
            vx = 0;
            mDashLeft = false;
        }
    }

    if (mLeft && !mDashRight && !mDashLeft && (vx >= 0)) vx -= HorizontalSpeed;
    if (mRight && !mDashRight && !mDashLeft && (vx <= 0)) vx += HorizontalSpeed;

    // Apply gravity to vertical velocity
    double vy = mVelocity.Y() + Gravity * elapsed;

    // Calculate potential new position for this frame
    double newX = mX + vx * elapsed;
    double newY = mY + vy * elapsed;

    //storing state before visitor runs
    bool wasOnGround = mOnGround;
    double oldY = mY;

    mOnGround = false; // Reset ground state before collision checks

    // perform Collision Detection using Visitor
    // Create a visitor, passing the football and its *potential* next state
    CCollisionVisitor visitor(this, newX, newY, vy);

    // Visit all items in the game's item list
    auto& items = GetGame()->GetItems();
    for (auto& item : items) {
        // Skip visiting self (the football)
        if (item.get() == this) continue;
        item->Accept(&visitor); // Double dispatch happens here
    }

    // Process collision results
    mOnGround = visitor.IsOnGround(); // Get ground status from visitor

    //Failsafe block
    // if (!mOnGround && wasOnGround && vy >= 0)
    // {
    //     mOnGround = true; // Override the visitor
    //     vy = 0;           // Stop gravity
    //     newY = oldY;      // Snap back to our previous stable Y-position
    // }

    // Adjust vertical position and velocity if we landed
    if (mOnGround) {
        vy = 0; // Stop falling
        // Place football exactly on the ground surface found by the visitor
        newY = visitor.GetGroundY() - GetHeight() / 2.0;
        mUsedDoubleJump = false;
    }

    // Check if the goalpost was hit
    if (visitor.HitGoalpost()) {
        GetGame() -> LevelCompleted();
    }

    //Checking for boss hit
    CBossGoalpost* hitBoss = visitor.HitBoss();
    if (hitBoss != nullptr)
    {
        // We hit the boss with a dash!
        hitBoss->TakeDamage(1);

        // Optional: Add a small bounce-back effect
        double bounceX = (GetX() < hitBoss->GetX()) ? -150.0 : 150.0;
        mVelocity.SetX(-bounceX); // Bounce back
        mVelocity.SetY(-300.0); // Bounce up slightly

        // Stop the dash
        mDashLeft = false;
        mDashRight = false;
    }

    CPowerup* hitPowerup = visitor.HitPowerup();
    if (hitPowerup != nullptr && !hitPowerup->IsCollected())
    {
        if (hitPowerup->GetType() == PowerType::Coin)
        {
            GetGame()->DoubleCoins();
        }
        if (hitPowerup->GetType() == PowerType::DoubleJump)
        {
            EnableDoubleJump();
        }
        if (hitPowerup->GetType() == PowerType::Dash)
        {
            EnableDash();
        }
        if (hitPowerup->GetType() == PowerType::CoinRush)
        {
            GetGame()->ActivateCoinRush();
            GetGame()->GetScoreboard()->AddScore(500); // Bonus points
        }
        hitPowerup->SetCollected(true);
    }

    // Check if a coin was hit
    CCoin* hitCoin = visitor.HitCoin();
    if (hitCoin != nullptr) // Simplified check for debug
    {
        if (!hitCoin->IsCollected())
        {
            GetGame()->GetScoreboard()->AddScore(hitCoin->GetValue());
            hitCoin->SetCollected(true);
        } else {
        }
    }

    // --- Wall Collision Reaction ---
    if (visitor.HitWallX()) { // Check horizontal collision flag
        vx = 0;              // Stop horizontal velocity
        newX = visitor.GetAdjustX(); // Apply corrected X position from visitor
    }
    else if (visitor.HitWallY()) { // Check vertical collision flag
        vy = 0;              // Stop vertical velocity
        newY = visitor.GetAdjustY(); // Apply corrected Y position from visitor
    }

    const double DeathPlaneY = 2048;
    if (newY > DeathPlaneY)
    {
        GetGame()->YouLose();
    }

    if (visitor.HitEnemy())
    {
        GetGame()->YouLose();
    }


    // Update velocity based on collision results
    mVelocity = Vector(vx, vy);

    // Apply the final, potentially adjusted position
    mX = newX;
    mY = newY; // Position is now finalized after collision adjustments

    // Apply walking animation offset *after* physics calculations
    if (mOnGround && (mLeft || mRight)) {
        mStepPhase += StepFrequency * elapsed;
        // Temporarily adjust Y for visual bounce, doesn't affect mVelocity or next frame's physics
        //mY -= StepHeight * fabs(sin(mStepPhase));
    } else {
        mStepPhase = 0; // Reset animation phase
    }

    // Update the football's visual image
    UpdateImage();
}

void CFootball::UpdateImage()
{
    // Change image based on movement direction
    if (mDashRight)
    {
        mImage = mImageRightDash;
        return;
    }
    if (mDashLeft)
    {
        mImage = mImageLeftDash;
        return;
    }

    if (mLeft)
        mImage = mImageLeft;
    else if (mRight)
        mImage = mImageRight;
    else
        mImage = mImageMid;
}

