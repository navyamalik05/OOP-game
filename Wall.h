/**
 * @file Wall.h
 * @author Hartejas Sekhon
 * @brief Class that implements a wall item in the game.
 *
 * The CWall class represents a wall object that can be drawn in the game.
 * The wall has configurable width and height values that can be set based
 * on XML level data. It extends the CItem class and implements visitor
 * acceptance for the visitor design pattern.
 */

#ifndef PROJECT1_WALL_H
#define PROJECT1_WALL_H

#include "Item.h"
#include "ItemVisitor.h"
#include <string>

/**
 * @class CWall
 * @brief A class representing a wall object in the game.
 *
 * The wall is an immovable item that can be drawn to a specified size.
 * It supports visitor interaction and allows its logical size to be
 * overridden from XML data, rather than relying on the bitmap dimensions.
 */
class CWall : public CItem
{
private:
    /// Width of the wall in pixels (set from level XML)
    int mWidth = 0;

    /// Height of the wall in pixels (set from level XML)
    int mHeight = 0;

public:
    /// Deleted default string constructor (unsafe)
    CWall(const std::string&) = delete;

    /// Deleted wxBitmap constructor (unsafe)
    CWall(wxBitmap) = delete;

    /**
     * @brief Construct a new wall item.
     * @param game Pointer to the game object that owns this wall.
     * @param filename The image filename for the wall texture.
     * @param x X-coordinate of the wall’s center position.
     * @param y Y-coordinate of the wall’s center position.
     */
    CWall(CGame* game, const std::wstring& filename, double x, double y)
        : CItem(game, filename, x, y) {}

    /**
     * @brief Set the logical size of the wall.
     * @param width Width of the wall in pixels.
     * @param height Height of the wall in pixels.
     */
    void SetSize(int width, int height) { mWidth = width; mHeight = height; }

    /**
     * @brief Get the width of the wall.
     * @return The width, using logical size if available; otherwise the bitmap width.
     */
    int GetWidth() const override { return mWidth > 0 ? mWidth : CItem::GetWidth(); }

    /**
     * @brief Get the height of the wall.
     * @return The height, using logical size if available; otherwise the bitmap height.
     */
    int GetHeight() const override { return mHeight > 0 ? mHeight : CItem::GetHeight(); }

    /**
     * @brief Draw the wall on the given graphics context.
     * @param g The graphics context to draw on.
     *
     * If a logical size (width/height) has been set, the wall is stretched
     * to that size; otherwise, it is drawn at its bitmap’s native size.
     */
    void Draw(wxGraphicsContext* g) override;

    /**
     * @brief Accept a visitor for this wall item.
     * @param visitor The visitor to accept.
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitWall(this); }
};

#endif // PROJECT1_WALL_H
