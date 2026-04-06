/**
* @file ImageCache.cpp
 * @author Navya Malik
 * @brief Implementation of a simple bitmap image cache for loading and reusing images.
 *
 * This cache prevents reloading the same image multiple times by storing
 * already loaded wxBitmap objects in a static unordered_map.
 */

#include "pch.h"
#include "ImageCache.h"
#include <unordered_map>
#include <wx/log.h>

/**
 * @brief Global cache mapping file paths to wxBitmap objects.
 */
static std::unordered_map<std::wstring, wxBitmap> g_cache;

/**
 * @brief Load a bitmap from a file, using the cache if available.
 *
 * If the bitmap has already been loaded, returns the cached copy.
 * Otherwise, loads the bitmap from disk and stores it in the cache.
 *
 * @param path Path to the image file
 * @return wxBitmap Loaded bitmap
 */
wxBitmap LoadBitmapCached(const std::wstring& path)
{
    auto it = g_cache.find(path);
    if (it != g_cache.end())
        return it->second;

    wxBitmap bmp(path, wxBITMAP_TYPE_ANY);
    if (!bmp.IsOk())
        wxLogError(L"Failed to load bitmap: %ls", path.c_str());

    g_cache[path] = bmp;
    return bmp;
}

/**
 * @brief Clear all cached bitmaps from memory.
 *
 * This empties the internal cache, freeing the wxBitmap objects.
 */
void ClearBitmapCache()
{
    g_cache.clear();
}
