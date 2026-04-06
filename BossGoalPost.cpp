/**
* @file BossGoalPost.cpp
* @author Navya Malik
*/

#include "pch.h"
#include "BossGoalPost.h"      // match filename casing
#include "ItemVisitor.h"
#include "Game.h"
#include <algorithm>
#include <cmath>

// -----------------------------
// ctor
// -----------------------------
CBossGoalpost::CBossGoalpost(CGame* game,
                             const std::wstring& imageFilename,
                             double x, double y)
: CEnemy(game, imageFilename, x, y, EnemyType::Stationary),
  mCenterX(x), mCenterY(y)
{
    SetLocation(x, y);
}

// -----------------------------
// damage
// -----------------------------
void CBossGoalpost::TakeDamage(int amount)
{
    if (mHealth <= 0) return;

    mHealth = std::max(0, mHealth - std::max(1, amount));
    mHitFlashSec = mFlashDur;

    if (mHealth == 0) {
        OnDefeated();
    }
}

// -----------------------------
// update (circular movement + flash timer)
// -----------------------------
void CBossGoalpost::Update(double elapsed)
{
    // circular motion around (mCenterX, mCenterY)
    mTheta += mOmega * elapsed;
    const double nx = mCenterX + mRadius * std::cos(mTheta);
    const double ny = mCenterY + mRadius * std::sin(mTheta);
    SetLocation(nx, ny);

    if (mHitFlashSec > 0.0) {
        mHitFlashSec -= elapsed;
        if (mHitFlashSec < 0.0) mHitFlashSec = 0.0;
    }
}

// -----------------------------
// draw (no SetAlpha; flicker by skipping draw)
// -----------------------------
void CBossGoalpost::Draw(wxGraphicsContext* gc)
{
    // Simple flicker when hit: skip drawing on alternating frames
    const bool flashOff = (mHitFlashSec > 0.0) &&
                          (((int)(mHitFlashSec * 18.0)) % 2 == 1);

    if (!flashOff) {
        CItem::Draw(gc);   // Enemy->CItem pipeline draws centered sprite
    }
    DrawHealthBar(gc);
}

// -----------------------------
// health bar above sprite
// -----------------------------
void CBossGoalpost::DrawHealthBar(wxGraphicsContext* gc)
{
    const double w = GetWidth();
    const double x = GetX();
    const double y = GetY() - 12.0;

    const double barW = std::max(30.0, w * 0.6);
    const double barH = 6.0;
    const double pct  = (mMaxHealth > 0) ? (double)mHealth / (double)mMaxHealth : 0.0;

    gc->SetPen(*wxTRANSPARENT_PEN);

    // background
    gc->SetBrush(*wxBLACK_BRUSH);
    gc->DrawRectangle(x + (w - barW) / 2.0, y, barW, barH);

    // foreground
    gc->SetBrush(*wxWHITE_BRUSH);
    gc->DrawRectangle(x + (w - barW) / 2.0, y, barW * pct, barH);
}

// -----------------------------
// visitor entry
// -----------------------------
void CBossGoalpost::Accept(CItemVisitor* visitor)
{
    visitor->VisitBossGoalpost(this);
}

// -----------------------------
// defeat hook
// -----------------------------
void CBossGoalpost::OnDefeated()
{
    // Minimal: just log; external code can poll IsDefeated()
    wxLogMessage("Boss defeated!");


    // Or, if you already have LevelCompleted():
    if (auto* g = GetGame()) g->LevelCompleted();
}
