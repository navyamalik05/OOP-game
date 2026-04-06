/**
 * @file Powerup.h
 * @author ander, Hartejas Sekhon
 *
 *
 */
#ifndef PROJECT1_POWERUP_H
#define PROJECT1_POWERUP_H

#include "Item.h"
#include "ItemVisitor.h"
#include <string>

/**
 * @enum PowerType
 * @brief Types of powerups available
 */
enum class PowerType
{
    Coin,
    DoubleJump,
    Dash,
    CoinRush
};

/**
 * @class CPowerup
 * @brief Collectible powerup item
 */
class CPowerup : public CItem
{
private:
    /// Is it collected
    bool mCollected = false;
    /// Is it animating
    bool   mAnimating    = false;
    /// timer time
    double mDisplayTimer = 0.0;
    /// flying velocity
    double mFlyVelocity  = 0.0;
    /// type of the powerup
    PowerType mType = PowerType::Coin;
    /// Scaled width (if 0, uses image width)
    double mWidth = 0;
    /// Scaled height (if 0, uses image height)
    double mHeight = 0;


public:
    /**
 * Constructor
 * @param game Pointer to game
 * @param filename Image file
 * @param x X position
 * @param y Y position
 */
    CPowerup(CGame* game, const std::wstring& filename, double x, double y)
        : CItem(game, filename, x, y) {}
    /**
 * Constructor with type
 * @param game Pointer to game
 * @param filename Image file
 * @param x X position
 * @param y Y position
 * @param type Powerup type
 */
    CPowerup(CGame* game, const std::wstring& filename, double x, double y, PowerType type)
        : CItem(game, filename, x, y), mType(type) {}
    /// @param visitor visitor
    void Accept(CItemVisitor* visitor) override { visitor->VisitPowerup(this); }
    /// @return width greater than 0
    int GetWidth() const override { return (mWidth > 0) ? mWidth : mImage.GetWidth(); }
    /// @return height greater than 0
    int GetHeight() const override { return (mHeight > 0) ? mHeight : mImage.GetHeight(); }
    /// @return is collected
    bool IsCollected() const { return mCollected; } /// Check if collected
    /// @param collected is it collected
    void SetCollected(bool collected);
    /// @param g graphics
    void Draw(wxGraphicsContext* g) override;
    /// @param elapsed elapsed time
    void Update(double elapsed) override;
    /// @return type
    PowerType GetType() { return mType; } /// Get powerup type
    /// @param width, height width and height
    void SetSize(double width, double height) override { mWidth = width; mHeight = height; }
};

#endif // PROJECT1_POWERUP_H