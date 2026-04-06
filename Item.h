/**
 * @file Item.h
 * @author Navya Malik, Jack Finlan-Hitt, Hartejas Sekhon
 * @brief Base class for all items in the game.
 */

#ifndef PROJECT1_ITEM_H
#define PROJECT1_ITEM_H

#include <wx/wx.h>
#include <memory>
#include <string>
#include "ItemVisitor.h"

class CGame;

/**
 * @class CItem
 * @brief Abstract base class for all game items.
 */
class CItem
{
protected:
    CGame* mGame = nullptr;       ///< The game this item belongs to
    double mX = 0;                ///< X position
    double mY = 0;                ///< Y position
    wxBitmap mImage;              ///< Image representing this item
    bool mRepelFromPlayer = false;///< True if item moves away from player
    double mRepelSpeed = 0.0;     ///< Repel speed in pixels/sec

    double mWidth = 0;            ///< Scaled width (if 0, uses image width)
    double mHeight = 0;           ///< Scaled height (if 0, uses image height)

public:
    CItem(std::string) = delete;  ///< Disabled constructor
    CItem(wxBitmap) = delete;     ///< Disabled constructor

    /**
     * @brief Constructor
     * @param game Pointer to the game this item belongs to
     * @param filename Image file name to load
     * @param x X coordinate
     * @param y Y coordinate
     */
    CItem(CGame* game, const std::wstring& filename, double x, double y);

    virtual ~CItem() = default;

    /// @return Width of item
    virtual int GetWidth() const { return (mWidth > 0) ? mWidth : mImage.GetWidth(); }
    /// @return Height of item
    virtual int GetHeight() const { return (mHeight > 0) ? mHeight : mImage.GetHeight(); }
    /**
     * @brief Draw the item on screen
     * @param graphics wxWidgets graphics context
     */
    virtual void Draw(wxGraphicsContext* graphics);

    /**
     * @brief Enable repel from player
     * @param speed Speed in pixels/sec
     */
    void EnableRepel(double speed) { mRepelFromPlayer = true; mRepelSpeed = speed; }

    /// Disable repel behavior
    void DisableRepel() { mRepelFromPlayer = false; mRepelSpeed = 0.0; }

    /**
     * @brief Update the item state
     * @param elapsed Time elapsed since last update (seconds)
     */
    virtual void Update(double elapsed);

    /// @return X position
    double GetX() const { return mX; }

    /// @return Y position
    double GetY() const { return mY; }

    /**
 * @brief Set item location
 * @param x, y X,Y coordinate
 */
    void SetLocation(double x, double y) { mX = x; mY = y; }

    /// @return Pointer to owning game
    CGame* GetGame() const { return mGame; }

    /**
     * @brief Set the scaled size for this item
     * @param width Scaled width
     * @param height Scaled height
     */
    virtual void SetSize(double width, double height) { mWidth = width; mHeight = height; }

    /**
     *
     * @brief Accept a visitor (part of visitor pattern)
     * @param visitor Visitor to accept
     */
    virtual void Accept(CItemVisitor* visitor) = 0;
};

#endif // PROJECT1_ITEM_H
