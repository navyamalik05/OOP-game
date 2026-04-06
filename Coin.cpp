/**
 * @file Coin.cpp
 * @author Hartejas Sekhon
 * @brief Implementation of the CCoin class.
 */

#include "pch.h"
#include "Coin.h"
#include <wx/graphics.h>
#include "Game.h"

static const double DisplayDuration = 2.0; ///< Duration of collected animation
static const double FlySpeed        = -1000.0; ///< Upward fly speed when collected

CCoin::CCoin(CGame* game, const std::wstring& filename, double x, double y, int value)
    : CItem(game, filename, x, y), mValue(value)
{
   //Give 0.25s of spawn protection
   mSpawnTimer = 0.10;
}


void CCoin::SetCollected(bool collected)
{
    if (collected && !mCollected) {
        mCollected    = true;
        mAnimating    = true;
        mDisplayTimer = DisplayDuration;
        mFlyVelocity  = FlySpeed;
    }
}

void CCoin::Update(double elapsed)
{
    if (mSpawnTimer > 0.0)
    {
        mSpawnTimer -= elapsed;
    }

    if (mAnimating)
    {
        if (mDisplayTimer > 0.0)
            mDisplayTimer -= elapsed;

        // Fly upward
        SetLocation(GetX(), GetY() + mFlyVelocity * elapsed);

        // Stop animation when off top
        if (GetY() < -static_cast<double>(GetHeight()))
            mAnimating = false;
    }
    else
    {
        // Coin drift on level 2
        if (GetGame()->GetLevel() == 2)
        {
            const double driftSpeed = -10.0; ///< Pixels per second left
            SetLocation(GetX() + driftSpeed * elapsed, GetY());
        }
    }
}

void CCoin::Draw(wxGraphicsContext* graphics)
{
    //draw coin unless collected and done animating
    if (!mCollected || (mAnimating && mDisplayTimer > 0.0))
    {
        // CItem::Draw handles the camera offset for us
        CItem::Draw(graphics);
    }

    //if its animating, also draw the score
    if (mAnimating && mDisplayTimer > 0.0) {
        wxFont font(wxFontInfo(16).Bold());
        graphics->SetFont(font, *wxBLUE);

        wxString s = wxString::Format("%d", mValue);
        double tw = 0, th = 0;
        graphics->GetTextExtent(s, &tw, &th);
        graphics->DrawText(s, GetX() - tw / 2.0, GetY() - th / 2.0);
    }
}

