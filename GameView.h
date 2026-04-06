/**
* @file GameView.h
 * @author Elle Hyry
 * @brief Handles drawing and managing items within the current level.
 */

#ifndef PROJECT1_GAMEVIEW_H
#define PROJECT1_GAMEVIEW_H

#include <vector>
#include <memory>
#include <wx/wx.h>
#include <wx/graphics.h>
#include "Scoreboard.h"

class CItem;
class CFootball;

/**
 * @class CGameView
 * @brief Manages rendering of game items for the current level.
 */
class CGameView
{
private:
    std::unique_ptr<wxBitmap> mBackground; ///< Background image
    double mXOffset = 0.0;                 ///< Horizontal scroll offset

public:
    /// Constructor
    CGameView();

    /// Set horizontal scroll offset
    /// @param offset offset
    void SetXOffset(double offset) { mXOffset = offset; }

    /// @return Current horizontal scroll offset
    double GetXOffset() const { return mXOffset; }

    /**
     * @brief Update the view (used for animations or scrolling)
     * @param elapsed Time elapsed since last update (seconds)
     */
    void Update(double elapsed);

    /**
     * @brief Draw the current game view
     * @param gc Graphics context
     * @param width Width of drawing area
     * @param height Height of drawing area
     * @param items Vector of game items to render
     */
    void OnDraw(wxGraphicsContext* gc, int width, int height, const std::vector<std::shared_ptr<CItem>>& items);
};

#endif //PROJECT1_GAMEVIEW_H
