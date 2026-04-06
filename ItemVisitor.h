/**
* @file ItemVisitor.h
 * @author Benjamin Anderson, Jack Finlan-Hitt, Hartejas Sekhon
 * @brief Abstract visitor class for all game items.
 */

#ifndef PROJECT1_ITEMVISITOR_H
#define PROJECT1_ITEMVISITOR_H

class CFootball;
class CPlatform;
class CBackground;
class CCoin;
class CEnemy;
class CPowerUp;
class CWall;
class CPowerup;
class CDoubleJumpPowerup;
class CGoalpost;
class CBossGoalpost;

/**
 * @class CItemVisitor
 * @brief Abstract visitor class for visiting game items.
 */
class CItemVisitor
{
protected:
    /// Constructor, ensures abstract class
    CItemVisitor() {}

public:
    virtual ~CItemVisitor() = default;
    /// Visit football
    /// @param football football
    virtual void VisitFootball(CFootball* football) {}
    /// Visit platform
    /// @param platform platform
    virtual void VisitPlatform(CPlatform* platform) {}
    /// Visit background
    /// @param background background
    virtual void VisitBackground(CBackground* background) {}
    /// Visit coin
    ///@param coin coin
    virtual void VisitCoin(CCoin* coin) {}
    /// Visit goalpost
    /// @param goal goal
    virtual void VisitGoalpost(CGoalpost* goal) {}
    /// Visit enemy
    /// @param enemy enemy
    virtual void VisitEnemy(CEnemy* enemy) {}
    /// Visit powerUp
    /// @param powerUp powerUp
    virtual void VisitPowerUp(CPowerUp* powerUp) {}
    /// Visit wall
    /// @param wall wall
    virtual void VisitWall(CWall* wall) {}
    /// Visit powerup
    /// @param powerup powerup
    virtual void VisitPowerup(CPowerup* powerup) {}
    /// Visit double jump powerup
    /// @param powerup powerup
    virtual void VisitDoubleJumpPowerup(CDoubleJumpPowerup* powerup) {}
    /// Visit boss goalpost
    /// @param boss boss
    virtual void VisitBossGoalpost(CBossGoalpost* boss) {}
};

#endif //PROJECT1_ITEMVISITOR_H
