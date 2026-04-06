/**
 * @file Platform.h
 * @author Hartejas Sekhon
 * @brief Declaration of the CPlatform class, representing a drawable platform made up of three image segments.
 */

#ifndef PROJECT1_PLATFORM_H
#define PROJECT1_PLATFORM_H

#include "Item.h"
#include "ItemVisitor.h"
#include <string>

/**
 * @class CPlatform
 * @brief Represents a platform composed of left, middle, and right images for flexible sizing.
 *
 * The CPlatform class handles the drawing and sizing of multi-segment platforms in the game.
 * Each platform consists of a left cap, a stretchable middle section, and a right cap.
 */
class CPlatform : public CItem
{
private:
    int mWidth;   ///< Width of the platform in pixels.
    int mHeight;  ///< Height of the platform in pixels.

    wxBitmap mImageLeft;   ///< Bitmap for the left segment.
    wxBitmap mImageMid;    ///< Bitmap for the middle segment.
    wxBitmap mImageRight;  ///< Bitmap for the right segment.

    std::wstring mImageLeftFile;   ///< File path for the left image.
    std::wstring mImageMidFile;    ///< File path for the middle image.
    std::wstring mImageRightFile;  ///< File path for the right image.

public:
    CPlatform(wxBitmap, wxBitmap, wxBitmap) = delete; ///< Deleted constructor to prevent unsafe use.
    CPlatform(const std::string&, const std::string&, const std::string&) = delete; ///< Deleted constructor.

    /**
     * @brief Construct a new CPlatform object.
     * @param game The game this platform belongs to.
     * @param leftFile File path for the left image segment.
     * @param midFile File path for the middle image segment.
     * @param rightFile File path for the right image segment.
     * @param x X-coordinate of the platform center.
     * @param y Y-coordinate of the platform center.
     * @param width Width of the platform.
     * @param height Height of the platform.
     */
    CPlatform(CGame* game,
              const std::wstring& leftFile,
              const std::wstring& midFile,
              const std::wstring& rightFile,
              double x, double y,
              int width, int height);

    /**
     * @brief Draw the platform on the provided graphics context.
     * @param graphics The graphics context to draw to.
     */
    void Draw(wxGraphicsContext* graphics) override;

    /**
     * @brief Accept a visitor to perform operations on this platform.
     * @param visitor The visitor object.
     */
    void Accept(CItemVisitor* visitor) override { visitor->VisitPlatform(this); }

    /**
     * @brief Set the platform’s size.
     * @param width Platform width in pixels.
     * @param height Platform height in pixels.
     */
    void SetSize(int width, int height) { mWidth = width; mHeight = height; }

    /**
     * @brief Get the platform width.
     * @return Platform width in pixels.
     */
    int GetWidth() const override { return mWidth; }

    /**
     * @brief Get the platform height.
     * @return Platform height in pixels.
     */
    int GetHeight() const override { return mHeight; }
};

#endif // PROJECT1_PLATFORM_H
