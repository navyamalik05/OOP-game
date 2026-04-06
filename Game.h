/**
 * @file Game.h
 * @author Hartejas Sekhon, Jack Finlan-Hitt
 * @brief The main game class that manages the current view, level, and scoreboard.
 */

#ifndef PROJECT1_GAME_H
#define PROJECT1_GAME_H

#include <memory>
#include <vector>
#include <wx/wx.h>
#include <wx/graphics.h>
#include <wx/timer.h>
#include <wx/bitmap.h>

#include "Item.h"
#include "GameView.h"
#include "Scoreboard.h"
#include "DoubleCoinsVisitor.h"

class CItem;

/**
 * Enum representing the current game state.
 */
enum class GameState
{
    Playing,        ///< Player is actively playing
    LevelStart,     ///<Player just started new level
    Level3Start,    ///<Player just starterd level 3
    LevelComplete,  ///< Player has completed the level
    YouLose         ///< Player has lost the level
};

/**
 * @class CGame
 * @brief Main game class.
 *
 * Manages the game state, view, items, player, and scoreboard.
 */
class CGame
{
private:
    /// Current game view
    std::unique_ptr<CGameView> mGameView;
    /// List of items in current level
    std::vector<std::shared_ptr<CItem>> mItems;
    /// Displays score and elapsed time
    CScoreboard mScoreboard;
    /// Timer driving updates
    wxTimer mTimer;
    /// Currently active level
    int mCurrentLevel = -1;
    /// Pointer to the player
    std::shared_ptr<CFootball> mFootball;
    /// Flag to restart level
    bool mRestartLevel = false;
    /// Flag for level completion
    bool mLevelComplete = false;
    /// Current game state
    GameState mGameState = GameState::Playing;
    /// Timer for messages
    double mMessageTimer = 0.0;
    /// Duration of messages in seconds
    double mMessageDuration = 2.0;

    //Coin rush state
    /// Is the coin rush active?
    bool mIsCoinRushActive = false;
    /// Coin rush timer
    double mCoinRushTimer = 0.0;
    /// Coin spawn timer
    double mCoinSpawnTimer = 0.0;
    /// Duration of coin rush
    const double mCoinRushDuration = 5.0; // 5 seconds
    /// Spawn rate of coins
    const double mCoinSpawnRate = 0.05; // Spawn coin every 0.05 seconds

    wxString mCurrentMessage; ///< Message to display ,e.g., "Level 1"

    // Bitmaps for Level 3 instructions
    wxBitmap mIconDash;
    wxBitmap mIconJump;
    wxBitmap mIconCoin;
    wxBitmap mIconBoss;

public:
    CGame();
    ~CGame();

    /**
     * Load a specific level
     * @param levelNum Level number to load
     */
    void Load(int levelNum);

    /**
     * Initialize game items for the current level
     */
    void Initialize();

    /**
     * Add a game item
     * @param item Item to add
     */
    void AddItem(std::shared_ptr<CItem> item);

    /**
     * Draw the game contents
     * @param graphics Graphics context
     * @param width Width of drawing area
     * @param height Height of drawing area
     */
    void OnDraw(wxGraphicsContext* graphics, int width, int height);

    /**
     * Handle key press events
     * @param event Key event
     */
    void OnKeyDown(wxKeyEvent& event);

    /**
     * Handle key release events
     * @param event Key event
     */
    void OnKeyUp(wxKeyEvent& event);

    /// @return Current score
    int GetScore() const;

    /// @return Elapsed time in current level
    double GetElapsedTime() const;

    /**
     * Update game logic
     * @param elapsed Time elapsed since last update (seconds)
     */
    void Update(double elapsed);

    /// @return Pointer to scoreboard
    CScoreboard* GetScoreboard() {return &mScoreboard;};

    /// @return Reference to item list
    std::vector<std::shared_ptr<CItem>>& GetItems() { return mItems; }

    /// @return Pointer to football player
    std::shared_ptr<class CFootball> GetFootball() const { return mFootball; }

    /// Restart the current level
    void RestartLevel()
    {
        mRestartLevel = true;
    }

    /// @return True if the level should restart
    bool ShouldRestartLevel() const { return mRestartLevel; }

    /// Clear the restart flag
    void ClearRestartFlag() { mRestartLevel = false; }

    /// Set level completion flag
    void LevelComplete() {mLevelComplete = true; }

    /// Double all coins in the current level
    void DoubleCoins();

    /// @return Current level number
    int GetLevel() { return mCurrentLevel; }

    /// Mark level as completed
    void LevelCompleted();

    /// Mark game as lost
    void YouLose();

    /// @return Current game state
    GameState GetGameState() const { return mGameState; }


    //Coin rush helpers
    /**
     *Activates coin rush powerup state
     *
     */
    void ActivateCoinRush() { mIsCoinRushActive = true; mCoinRushTimer = mCoinRushDuration; mCoinSpawnTimer = 0.0; }

    /**
     * Spawns a coin at the football's current location.
     *
     */
    void SpawnCoin(double x, double y);

    /**
     * Safely removes items from the main list.
     *
     */
    void RemoveItems(const std::vector<CItem*>& toRemove);
};

#endif //PROJECT1_GAME_H
