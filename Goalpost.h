/**
* @file Goalpost.h
 * @author Navya Malik,Hartejas Sekhon
 * @brief Represents the goalpost item in the game.
 */

#ifndef PROJECT1_GOALPOST_H
#define PROJECT1_GOALPOST_H

#include "Item.h"

class CGame;

/**
 * @class CGoalpost
 * @brief Goalpost item that the player interacts with.
 */
class CGoalpost : public CItem
{
public:
    /**
     * @brief Constructor
     * @param game Pointer to the game
     * @param filename Image file for the goalpost
     * @param x X position
     * @param y Y position
     */
    CGoalpost(CGame* game, const wxString& filename, double x, double y) :
        CItem(game, filename.ToStdWstring(), x, y) {}

    /// Default constructor disabled
    CGoalpost() = delete;

    /**
     * @brief Accept a visitor
     * @param visitor Visitor object
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitGoalpost(this); }
};

#endif //PROJECT1_GOALPOST_H
