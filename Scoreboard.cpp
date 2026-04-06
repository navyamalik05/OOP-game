/**
 * @file Scoreboard.cpp
 * @author Jack Finlan-Hitt
 * @brief Implementation of the CScoreboard class.
 *
 * The scoreboard tracks elapsed game time and the player’s score.
 * It handles timed score decay and renders both the timer and score
 * on the screen using wxWidgets graphics functions.
 */

#include "pch.h"
#include "Scoreboard.h"

/**
 * @brief Constructor. Initializes and resets the scoreboard.
 */
CScoreboard::CScoreboard()
{
    Reset();
}


void CScoreboard::Update(double elapsed)
{
    // Update elapsed time
    mTime += elapsed;

    // Accumulate time for score decay
    mTimeSinceLastDecay += elapsed;

    // Decay score periodically (every mDecayInterval seconds)
    while (mTimeSinceLastDecay >= mDecayInterval)
    {
        mTimeSinceLastDecay -= mDecayInterval;

        // Decrease score, but never below zero
        mScore = std::max(0, mScore - mDecayAmount);
    }
}


void CScoreboard::Draw(wxGraphicsContext* gc, int width, int height)
{
    if (!gc) return;

    // Set up font for scoreboard text
    wxFont font(wxFontInfo(24).Bold());
    gc->SetFont(font, *wxRED);

    // Draw timer in upper left corner
    int totalSeconds = static_cast<int>(mTime);
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    wxString timeStr = wxString::Format("%02d:%02d", minutes, seconds);
    double tw, th;
    gc->GetTextExtent(timeStr, &tw, &th);
    gc->DrawText(timeStr, 20, 20);

    // Draw score in upper right corner
    wxString scoreStr = wxString::Format("%d", mScore);
    double sw, sh;
    gc->GetTextExtent(scoreStr, &sw, &sh);
    gc->DrawText(scoreStr, width - sw - 20, 20);
}


void CScoreboard::AddScore(int points)
{
    mScore += points;
}

/**
 * @brief Reset the scoreboard.
 *
 * Resets time, score, and decay tracking variables to initial values.
 */
void CScoreboard::Reset()
{
    mTime = 0.0;
    mScore = 0;
    mTimeSinceLastDecay = 0.0;
    mIsDoubleCoins = false;
}
