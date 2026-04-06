/**
* @file GameView.cpp
* @author Elle Hyry
 * @brief Implementation of CGameView class.
 */

#include "pch.h"
#include "GameView.h"
#include "Football.h"
#include "Background.h"

#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/dcbuffer.h>

#include "Platform.h"
#include "Level.h"

/**
 * Constructor
 */
CGameView::CGameView()
{

}

/**
 * Draw the current view (background + football)
 */
void CGameView::OnDraw(wxGraphicsContext* gc, int width, int height, const std::vector<std::shared_ptr<CItem>>& items)
{
    if (!gc)
    {
        wxLogError("Graphics context is null!");
        return;
    }

    // Save the original, unscrolled state
    gc->PushState();

    //
    // --- 1. DRAW BACKGROUNDS (with parallax) ---
    //
    gc->PushState(); // Save state for background scroll
    gc->Translate(-mXOffset * 0.5, 0); // Apply 50% scroll speed

    for (auto item : items)
    {
        // Check if this item is a CBackground
        if (dynamic_cast<CBackground*>(item.get()) != nullptr)
        {
            item->Draw(gc);
        }
    }
    gc->PopState(); // Restore from background scroll

    //
    // --- 2. DRAW FOREGROUND (platforms, player, coins, etc.) ---
    //
    gc->PushState(); // Save state for foreground scroll
    gc->Translate(-mXOffset, 0); // Apply 100% scroll speed

    for (auto item : items)
    {
        // Draw everything THAT IS NOT a CBackground
        if (dynamic_cast<CBackground*>(item.get()) == nullptr)
        {
            item->Draw(gc);
        }
    }
    gc->PopState(); // Restore from foreground scroll

    //
    // --- 3. RESTORE ORIGINAL STATE ---
    //
    // This is for drawing non-scrolling UI elements
    // (like the scoreboard, which is drawn back in Game.cpp)
    gc->PopState();
}


/**
 * Update the football's movement and animation
 */
void CGameView::Update(double elapsed)
{
    // The CGame class now handles updating all game items.
    // This function can be used later for view-specific animations if needed.
    // For now, it can be empty.
}

