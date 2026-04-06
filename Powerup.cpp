/**
 * @file Powerup.cpp
 * @author Jack Finlan-Hitt, Hartejas Sekhon
 * @brief Implementation of the CPowerup class.
 *
 * Handles drawing, collection behavior, and animation for the
 * power-up item. Once collected, the power-up displays text and
 * moves upward for a limited duration before disappearing.
 */

#include "pch.h"
#include "Powerup.h"
#include <wx/graphics.h>
#include "Game.h"
#include "Football.h"

/// Duration (in seconds) the power-up animation is displayed.
static const double DisplayDuration = 3.5;

/// Vertical speed of the power-up animation (pixels per second).
static const double FlySpeed = -150.0;

void CPowerup::Draw(wxGraphicsContext* g)
{
    if (mAnimating && mDisplayTimer > 0.0)
    {
        wxFont font(wxFontInfo(24).Bold());
        g->SetFont(font, wxColour(24, 69, 59));

        // Display main effect text
        wxString s1 = wxString::Format("Power Up!");
        wxString s2 ;

        //Display text bases on powerup type
        switch (mType)
        {
        case PowerType::Coin:
            s2 = "Double Coins!";
            break;
        case PowerType::DoubleJump:
            s2 = "Double Jump! (Press [Space] when in the air";
            break;
        case PowerType::Dash:
            s2 = "Dash Power! (Press [Shift])";
            break;
        case PowerType::CoinRush:
            s2 = "Coin Rush Active!";
            break;
        }

        double tw1 = 0, th1 = 0;
        double tw2 = 0, th2 = 0;
        g->GetTextExtent(s1, &tw1, &th1);
        g->GetTextExtent(s2, &tw2, &th2);

        // Draw first line centered above
        g->DrawText(s1, GetX() - tw1 / 2.0, GetY() - th1 / 2.0 - 20);

        // Draw second line below
        wxFont smallFont(wxFontInfo(16).Bold());
        g->SetFont(smallFont, wxColour(24, 69, 59));
        g->DrawText(s2, GetX() - tw2 / 2.0, GetY() - th2 / 2.0 + 10);
    }
    else if (!mCollected || (mAnimating && mDisplayTimer <= 0.0))
    {
        //use if width and height have been set
        double w = (mWidth > 0) ? mWidth : mImage.GetWidth();
        double h = (mHeight > 0) ? mHeight : mImage.GetHeight();

        // Replicating CItem::Draw logic
        g->DrawBitmap(mImage, GetX() - w / 2, GetY() - h / 2, w, h);    }
}

void CPowerup::SetCollected(bool collected)
{
    if (collected && !mCollected)
    {
        mCollected    = true;
        mAnimating    = true;
        mDisplayTimer = DisplayDuration;
        mFlyVelocity  = FlySpeed;

        CGame* game = GetGame();
        if (game == nullptr) return;

        //Activate powerup bases on type
        switch (mType)
        {

        case PowerType::Coin:
            game->GetScoreboard()->AddScore(100);
            game->DoubleCoins();
            break;

        case PowerType::DoubleJump:
            {
                // This is your teammate's feature.
                // The CFootball::Update function already handles this call.
                game->GetScoreboard()->AddScore(300);
                break;
            }

        case PowerType::Dash:
            // Your teammate's feature.
            // The CFootball::Update function already handles this call.
            game->GetScoreboard()->AddScore(300);
            break;

        case PowerType::CoinRush:
            game->ActivateCoinRush(); // <-- We will add this to CGame
            game->GetScoreboard()->AddScore(500);
            break;
        }
    }
}


void CPowerup::Update(double elapsed)
{
    if (!mAnimating)
    {
        return;
    }
    if (mDisplayTimer > 0.0)
    {
        mDisplayTimer -= elapsed;
    }
    SetLocation(GetX(), GetY() + mFlyVelocity * elapsed);
    if (GetY() < -static_cast<double>(GetHeight()))
    {
        mAnimating = false;
    }
    // Move upward during animation
    SetLocation(GetX(), GetY() + mFlyVelocity * elapsed);

    // Stop animating once it moves off the top of the screen
    if (GetY() < -static_cast<double>(GetHeight()))
    {
        mAnimating = false;
    }
}
