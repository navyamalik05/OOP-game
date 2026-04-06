/**
 * @file CollisionVisitor.h
 * @author Hartejas Sekhon, Jack Finlan-Hitt
 * @brief Visitor for detecting collisions between the football and other items.
 */

#ifndef PROJECT1_COLLISIONVISITOR_H
#define PROJECT1_COLLISIONVISITOR_H

#include "ItemVisitor.h"

// Forward declarations
class CFootball;
class CPlatform;
class CBackground;
class CCoin;
class CGoalpost;
class CEnemy;
class CPowerup;
class CWall;
class CLevel;
class CDoubleJumpPowerup;
class CBossGoalpost;

/**
 * @class CCollisionVisitor
 * @brief Implements visitor pattern to check football collisions with game items.
 *
 * Tracks collisions with platforms, coins, walls, enemies, and powerups.
 * Stores results for query by the football after visiting all items.
 */
class CCollisionVisitor : public CItemVisitor
{
private:
    /// Pointer to football
    CFootball* mFootball = nullptr;
    /// Pointer to level (optional)
    CLevel* mLevel = nullptr;
    /// Predicted X for this frame
    double mFootballNextX = 0;
    /// Predicted Y for this frame
    double mFootballNextY = 0;
    /// Current Y velocity
    double mFootballVelY = 0;

    // Collision results
    /// Landed on platform
    bool mOnGround = false;
    /// Y coordinate of ground
    double mGroundY = 0;
    /// Coin hit (if any)
    CCoin* mHitCoin = nullptr;
    /// Did hit goalpost
    bool mHitGoalpost = false;
    /// Horizontal wall collision
    bool mHitWallX = false;
    /// Vertical wall collision
    bool mHitWallY = false;
    /// X correction after wall hit
    double mAdjustX = 0;
    /// Y correction after wall hit
    double mAdjustY = 0;
    /// Enemy collision
    bool mHitEnemy = false;
    /// Powerup hit (if any)
    CPowerup* mHitPowerup = nullptr;
    /// Double jump powerup hit
    CDoubleJumpPowerup* mHitDoubleJumpPowerup = nullptr;
    /// Boss goalpost hit (if any)
    CBossGoalpost* mHitBoss = nullptr;

public:
    /**
     * Constructor
     * @param football Pointer to football
     * @param nextX Predicted next X position
     * @param nextY Predicted next Y position
     * @param velY Current Y velocity
     */
    CCollisionVisitor(CFootball* football, double nextX, double nextY, double velY);

    // Visit overrides
    void VisitPlatform(CPlatform* platform) override;
    void VisitCoin(CCoin* coin) override;
    void VisitGoalpost(CGoalpost* goalpost) override;
    void VisitWall(CWall* wall) override;
    void VisitEnemy(CEnemy* enemy) override;
    void VisitPowerup(CPowerup* powerup) override;
    void VisitBossGoalpost(CBossGoalpost* boss) override;

    // --- Getters for collision results ---
    /// Check if football is on ground
    /// @return True if on ground
    bool IsOnGround() const { return mOnGround; }
    /// Get Y coordinate of ground
    /// @return Ground Y position
    double GetGroundY() const { return mGroundY; }
    /// Get pointer to hit coin
    /// @return Pointer to hit coin (nullptr if none)
    CCoin* HitCoin() const { return mHitCoin; }
    /// Check if goalpost was hit
    /// @return True if goalpost hit
    bool HitGoalpost() const { return mHitGoalpost; }
    /// Check for horizontal wall collision
    /// @return True if hit wall horizontally
    bool HitWallX() const { return mHitWallX; }
    /// Check for vertical wall collision
    /// @return True if hit wall vertically
    bool HitWallY() const { return mHitWallY; }
    /// Get adjusted X position after collision
    /// @return Corrected X position
    double GetAdjustX() const { return mAdjustX; }
    /// Get adjusted Y position after collision
    /// @return Corrected Y position
    double GetAdjustY() const { return mAdjustY; }
    /// Get pointer to hit powerup
    /// @return Pointer to hit powerup (nullptr if none)
    CPowerup* HitPowerup() const { return mHitPowerup; }
    /// Check if enemy was hit
    /// @return True if enemy hit
    bool HitEnemy() const { return mHitEnemy; }
    /// Get pointer to hit boss
    /// @return Pointer to hit boss (nullptr if none)
    CBossGoalpost* HitBoss() const { return mHitBoss; }
};

#endif // PROJECT1_COLLISIONVISITOR_H
