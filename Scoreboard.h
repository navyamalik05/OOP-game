/**
* @file Scoreboard.h
* @author Jack Finlan-Hitt
* @brief Manages the game scoreboard including timer and score display.
*/

#pragma once

#include <wx/wx.h>
#include <wx/graphics.h>

/**
 * @class CScoreboard
 * @brief Tracks and displays time and score
 */
class CScoreboard
{
private:
    double mTime = 0.0;      ///< Elapsed time in seconds
    int mScore = 0;                 ///< Current score
    double mTimeSinceLastDecay = 0.0; ///< Time accumulator for score decay
    const double mDecayInterval = 1.0; ///< Decay score every 1 second
    const int mDecayAmount = 1;     ///< Amount to decrease score per interval

    bool mIsDoubleCoins = false; ///<Is Double coin power up active
public:
    CScoreboard();

    /**
     * Update the scoreboard
     * @param elapsed Time elapsed since last update
     */
    void Update(double elapsed);

    /**
     * Draw the scoreboard on the screen
     * @param gc Graphics context to draw on
     * @param width Window width
     * @param height Window height
     */
    void Draw(wxGraphicsContext* gc, int width, int height);

    /**
     * Add points to the score (e.g., when collecting a coin)
     * @param points Points to add
     */
    void AddScore(int points);

    /**
     * Get the current score
     * @return Current score value
     */
    int GetScore() const { return mScore; }

    /**
     * Get elapsed time in seconds
     * @return Elapsed time
     */
    double GetElapsedTime() const { return mTime; }


    /**
     * Activate the double coins powerup
     */
    void ActivateDoubleCoins() { mIsDoubleCoins = true; }

    /**
     * Reset the scoreboard for a new level
     */
    void Reset();

};
