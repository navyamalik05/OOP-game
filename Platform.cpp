/**
 * @file Platform.cpp
 * @author Hartejas Sekhon, ander
 * @brief Implementation of the CPlatform class for drawing segmented platforms.
 */

#include "pch.h"
#include "Platform.h"
#include <wx/graphics.h>

void CPlatform::Draw(wxGraphicsContext* graphics)
{
    const int leftWidth  = mImageLeft.GetWidth();
    const int midWidth   = mImageMid.GetWidth();
    const int rightWidth = mImageRight.GetWidth();

    const double topLeftX = GetX() - mWidth / 2.0;
    const double topLeftY = GetY() - mHeight / 2.0;

    // Draw left cap
    graphics->DrawBitmap(mImageLeft, topLeftX, topLeftY, leftWidth, mHeight);

    // Draw middle stretch
    double currentX = topLeftX + leftWidth;
    const double endX = topLeftX + mWidth - rightWidth;

    while (currentX < endX) {
        int widthToDraw = midWidth;
        if (currentX + widthToDraw > endX)
            widthToDraw = static_cast<int>(endX - currentX);

        graphics->DrawBitmap(mImageMid, currentX, topLeftY, widthToDraw, mHeight);
        currentX += widthToDraw;
    }

    // Draw right cap
    graphics->DrawBitmap(mImageRight, endX, topLeftY, rightWidth, mHeight);
}

CPlatform::CPlatform(CGame* game,
                     const std::wstring& leftFile,
                     const std::wstring& midFile,
                     const std::wstring& rightFile,
                     double x, double y,
                     int width, int height)
    : CItem(game, midFile, x, y),
      mWidth(width),
      mHeight(height),
      mImageLeftFile(leftFile),
      mImageMidFile(midFile),
      mImageRightFile(rightFile)
{
    mImageLeft  = wxBitmap(mImageLeftFile,  wxBITMAP_TYPE_ANY);
    mImageMid   = wxBitmap(mImageMidFile,   wxBITMAP_TYPE_ANY);
    mImageRight = wxBitmap(mImageRightFile, wxBITMAP_TYPE_ANY);

    // Log errors if any images fail to load
    if (!mImageLeft.IsOk())
        wxLogError(L"Platform left image failed: %ls",  mImageLeftFile.c_str());
    if (!mImageMid.IsOk())
        wxLogError(L"Platform mid image failed: %ls",   mImageMidFile.c_str());
    if (!mImageRight.IsOk())
        wxLogError(L"Platform right image failed: %ls", mImageRightFile.c_str());
}
