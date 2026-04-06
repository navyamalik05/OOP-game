/**
* @file Enemy.h
 * @author Elle
 * @brief Represents an enemy in the game.
 */

#ifndef PROJECT1_ENEMY_H
#define PROJECT1_ENEMY_H

#include "Item.h"
#include <wx/graphics.h>
#include <string>

/**
 * Types of enemies
 */
enum class EnemyType
{
    Patrol,      ///< Moves horizontally
    Stationary,  ///< Does not move
    Chaser,      ///< (Future) Chases player
    Vertical     ///< Moves vertically
};

/**
 * @class CEnemy
 * @brief Represents a game enemy
 */
class CEnemy : public CItem
{
private:
    double mSpeed = 100.0;       ///< Movement speed
    double mPatrolRange = 200.0; ///< Patrol range for horizontal/vertical movement
    double mStartX = 0.0;        ///< Initial X position
    double mStartY = 0.0;        ///< Initial Y position
    bool mMovingRight = true;    ///< Direction of patrol
    bool mIsKilled = false;      ///< Flag if enemy is killed
    EnemyType mType = EnemyType::Patrol; ///< Enemy type

public:
    /**
     * Constructor
     * @param game Pointer to game
     * @param filename Image file for enemy
     * @param x Initial X
     * @param y Initial Y
     * @param type Enemy type
     */
    CEnemy(CGame* game, const std::wstring& filename, double x, double y, EnemyType type);

    /// @return Enemy type
    EnemyType GetType() const { return mType; }

    /// Update enemy movement
    /// @param elapsed elapsed time
    void Update(double elapsed) override;

    /// Draw enemy if alive
    /// @param graphics graphics
    void Draw(wxGraphicsContext* graphics) override;

    /// Accept a visitor
    /// @param visitor visitor
    void Accept(CItemVisitor* visitor) override;

    /// Reset enemy to start position
    void Reset();

    /**
     * Check collision with football
     * @param football Pointer to football
     * @return True if colliding
     */
    bool CheckCollision(CFootball* football);

    /// @return True if enemy is killed
    bool IsKilled() { return mIsKilled; }

    /// Mark enemy as killed
    void SetKilled() { mIsKilled = true; }
};

#endif // PROJECT1_ENEMY_H
