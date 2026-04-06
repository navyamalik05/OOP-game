/**
* @file Wall.cpp
 * @author Hartejas Sekhon
 * @brief Implementation of the CWall class.
 */

#include "pch.h"
#include "Wall.h"
#include <wx/graphics.h>


void CWall::Draw(wxGraphicsContext* g)
{
    if (!mImage.IsOk()) return;

    // Original bitmap dimensions
    const double bmpW = mImage.GetWidth();
    const double bmpH = mImage.GetHeight();

    // Logical dimensions (if set)
    const double drawW = (mWidth  > 0 ? mWidth  : bmpW);
    const double drawH = (mHeight > 0 ? mHeight : bmpH);

    // Draw centered at (mX, mY)
    g->DrawBitmap(mImage, GetX() - drawW / 2.0, GetY() - drawH / 2.0, drawW, drawH);
}
