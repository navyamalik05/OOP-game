/**
 * @file Coin.h
 * @author Hartejas Sekhon
 * @brief Class representing a collectible coin item in the game.
 */

#ifndef PROJECT1_COIN_H
#define PROJECT1_COIN_H

#include "Item.h"
#include "ItemVisitor.h"
#include <string>

/**
 * @class CCoin
 * @brief Represents a coin that can be collected by the football.
 *
 * Handles animation for collection, value, and simple movement.
 */
class CCoin : public CItem
{
private:
    bool   mCollected    = false; ///< Whether the coin has been collected
    bool   mAnimating    = false; ///< Whether the coin is currently animating
    double mDisplayTimer = 0.0;   ///< Timer for displaying collected coin
    double mFlyVelocity  = 0.0;   ///< Velocity when coin flies upward
    int    mValue        = 0;     ///< Coin's value

    double mSpawnTimer = 0.0; ///<spawn protection timer needed during coin rush
    bool mIsDoubled = false; ///<Has this coin been doubled


public:
    /**
     * Constructor
     * @param game Pointer to the game
     * @param filename Image file for the coin
     * @param x Initial X position
     * @param y Initial Y position
     * @param value Coin value
     */
    CCoin(CGame* game, const std::wstring& filename, double x, double y, int value);

    /**
     * Set whether the coin has been collected.
     * Starts animation if collecting for the first time.
     * @param collected True if collected
     */
    void SetCollected(bool collected);

    /**
     * Update coin's state (animation or movement)
     * @param elapsed Time elapsed since last frame
     */
    void Update(double elapsed) override;

    /**
     * Draw the coin on the screen
     * @param graphics Pointer to the graphics context
     */
    void Draw(wxGraphicsContext* graphics) override;

    /// Check if coin is collected
    /// @return True if collected
    bool IsCollected() const { return mCollected; }

    /// Get coin value
    /// @return Coin value
    int GetValue() const { return mValue; }

    /**
    * Set coin value
    * @param value New coin value
    */
    void SetValue(int value) { mValue = value; }

    /**
     * Accept a visitor (Visitor Pattern)
     * @param visitor Pointer to item visitor
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitCoin(this); }

    /** @return True if the coin is spawn-protected */
    bool IsSpawnProtected() const { return mSpawnTimer > 0.0; }

    /// Mark this coin as doubled
    void SetDoubled() { mIsDoubled = true; }

    /// Check if this coin has been doubled
    bool IsDoubled() const { return mIsDoubled; }

};

#endif // PROJECT1_COIN_H
