/**
* @file Football.h
* @author Navya Malik, Hartejas Sekhon
* @brief Football item that can move, jump, and animate
*/

#ifndef PROJECT1_FOOTBALL_H
#define PROJECT1_FOOTBALL_H

#include "Item.h"
#include "Vector.h"
#include <wx/wx.h>
#include <string>

/**
 * @class CFootball
 * @brief Football player controlled by the user.
 *
 * Supports walking, dashing, jumping, double jump, and animations.
 */
class CFootball : public CItem
{
private:
    /// Current velocity
    Vector mVelocity;
    /// Facing left?
    bool mFacingLeft = false;
    /// Moving left?
    bool mLeft = false;
    /// Moving right?
    bool mRight = false;
    /// Dashing right?
    bool mDashRight = false;
    /// Dashing left?
    bool mDashLeft = false;
    /// Is on ground/platform
    bool mOnGround = false;
    /// Currently jumping
    bool mJumping = false;
    /// Animation phase for small bounce while walking
    double mStepPhase = 0;
    /// Has double jump?
    bool mHasDoubleJump = false;
    /// Used double jump?
    bool mUsedDoubleJump = false;
    /// Has dash?
    bool mHasDash = false;

    // Image variants
    /// Left image
    wxBitmap mImageLeft;
    /// Left dash image
    wxBitmap mImageLeftDash;
    /// Right image
    wxBitmap mImageRight;
    /// Right dash image
    wxBitmap mImageRightDash;
    /// Middle image
    wxBitmap mImageMid;

public:
    /**
     * Constructor
     * @param game Pointer to CGame
     * @param x Initial X position
     * @param y Initial Y position
     */
    CFootball(CGame* game, double x, double y);

    /**
     * Update football physics and animations
     * @param elapsed Time elapsed since last update
     */
    void Update(double elapsed) override;

    /**
     * Handle key press events
     * @param event Key event
     */
    void OnKeyDown(wxKeyEvent& event);

    /**
     * Handle key release events
     * @param event Key event
     */
    void OnKeyUp(wxKeyEvent& event);

    void VisitGoalpost(CGoalpost* goalpost);

    /**
     * Accept a visitor
     * @param visitor the visitor we accept
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitFootball(this); }

    /**
     * Getter for Dash Left
     * @return
     */
    bool DashLeft() { return mDashLeft; }

    /**
     * Getter for Dash Right
     * @return
     */
    bool DashRight() { return mDashRight; }

    /// Enable double jump ability
    void EnableDoubleJump() {mHasDoubleJump = true;}
    /// Enable dash ability
    void EnableDash() {mHasDash = true;}
private:
    /// Update football image based on movement
    void UpdateImage();
};

#endif // PROJECT1_FOOTBALL_H
