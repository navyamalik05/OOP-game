/**
* @file Background.h
 * @author ander
 * @brief Class representing a static background item.
 */

#ifndef PROJECT1_BACKGROUND_H
#define PROJECT1_BACKGROUND_H

#include "Item.h"
#include "ItemVisitor.h"
#include <string>

/**
 * @class CBackground
 * @brief Represents a static background image in the game.
 */
class CBackground : public CItem
{
public:
    /**
     * Constructor
     * @param game Pointer to the game
     * @param filename Background image file
     * @param x X position
     * @param y Y position
     */
    CBackground(CGame* game, const std::wstring& filename, double x, double y)
        : CItem(game, filename, x, y) {}

    /**
     * Accept a visitor (Visitor Pattern)
     * @param visitor Pointer to item visitor
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitBackground(this); }
};

#endif // PROJECT1_BACKGROUND_H
