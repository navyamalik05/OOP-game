/**
 * @file Item.cpp
 * @author Navya Malik, Jack Finlan-Hitt
 * @brief Implementation of base Item class.
 */

#include "pch.h"
#include "Item.h"
#include "Game.h"
#include "Football.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "ImageCache.h"
#include <cmath>

CItem::CItem(CGame* game, const std::wstring& filename, double x, double y)
    : mGame(game), mX(x), mY(y)
{
    if (filename.empty())
    {
        wxLogError("Empty image filename passed to CItem. CWD: %s", wxGetCwd());
        return;
    }

    wxFileName fn(filename);
    if (!fn.FileExists())
    {
        wxLogError("Image not found: %ls (CWD: %s)", filename.c_str(), wxGetCwd());
        return;
    }

    mImage = LoadBitmapCached(filename);
    if (!mImage.IsOk())
    {
        wxLogError(L"Failed to load image: %ls", filename.c_str());
    }
}

void CItem::Draw(wxGraphicsContext* graphics)
{
    if (mImage.IsOk())
    {
        double width = mImage.GetWidth();
        double height = mImage.GetHeight();
        graphics->DrawBitmap(mImage, mX - width / 2, mY - height / 2, width, height);
    }
}

void CItem::Update(double elapsed)
{
    if (!mRepelFromPlayer || mRepelSpeed <= 0.0 || mGame == nullptr)
        return;

    auto player = mGame->GetFootball();
    if (!player) return;

    const double px = player->GetX();
    const double py = player->GetY();

    double dx = mX - px;
    double dy = mY - py;
    const double len = std::sqrt(dx*dx + dy*dy);
    if (len < 1e-6) return;

    dx /= len;
    dy /= len;

    mX += dx * mRepelSpeed * elapsed;
    mY += dy * mRepelSpeed * elapsed;
}
