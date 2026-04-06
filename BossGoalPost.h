/**
* @file BossGoalPost.h
* @author Navya Malik
*/

#ifndef PROJECT1_BOSSGOALPOST_H
#define PROJECT1_BOSSGOALPOST_H

#include "Enemy.h"

class CItemVisitor;

/**
 * @class CBossGoalpost
 * @brief Boss enemy that orbits and takes damage from dashes
 */
class CBossGoalpost final : public CEnemy
{
public:
    /**
 * Constructor
 * @param game, imageFilename, x, y Pointer to game, filename, xpos, ypos
 */
    CBossGoalpost(CGame* game,
                  const std::wstring& imageFilename,
                  double x, double y);

    // Called by collision visitor when the player dash hits the boss
    /**
     * Apply damage to boss
     * @param amount Damage amount
     */
    void TakeDamage(int amount = 1);

    // CItem/CEnemy overrides
    /**
    * Update boss movement and flash timer
    * @param elapsed Time elapsed since last update
    */
    void Update(double elapsed) override;
    /**
    * Draw boss and health bar
    * @param gc Graphics context
    */
    void Draw(wxGraphicsContext* gc) override;
    /**
    * Accept a visitor
    * @param visitor Pointer to item visitor
    */
    void Accept(CItemVisitor* visitor) override;

    // For HUD/logic checks
    /// Get current health
    /// @return Current health value
    int  GetHealth()   const { return mHealth; }
    /// Get maximum health
    /// @return Maximum health value
    int  GetMaxHealth() const { return mMaxHealth; }
    /// Check if defeated
    /// @return Health
    bool IsDefeated()  const { return mHealth <= 0; } /// @return Health

private:
    // Orbit center (don’t rely on Enemy’s private start coords)
    double mCenterX; /// Orbit center X coordinate
    double mCenterY; /// Orbit center Y coordinate

    // Boss config
    int    mMaxHealth = 3; /// Maximum health (needs 3 hits)
    int    mHealth    = 3; /// Current health
    double mRadius    = 120.0; /// Orbit radius in pixels
    double mOmega     = 1.2; /// Angular velocity in radians/sec

    // FX
    double mTheta        = 0.0;  /// Current angle in orbit
    double mHitFlashSec  = 0.0;  /// counts down after a hit
    double mFlashDur     = 0.12; /// seconds

    // Helpers
    /**
     * Draw health bar above boss
     * @param gc Graphics context
     */
    void DrawHealthBar(wxGraphicsContext* gc);
    /// Handle boss defeat logic
    void OnDefeated();           // set defeated state / optional notify game
};

#endif //PROJECT1_BOSSGOALPOST_H
