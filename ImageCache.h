/**
* @file ImageCache.h
* @author Navya Malik
*/

#pragma once
#include <wx/bitmap.h>
#include <string>

// Load a bitmap once and reuse it thereafter.
// Safe to copy: wxBitmap shares internal data (ref-counted).
wxBitmap LoadBitmapCached(const std::wstring& path);

// clear cache
void ClearBitmapCache();/**
 * @file ImageCache.h
 * @author Navya Malik
 * @brief Provides functions for loading and caching wxBitmap images.
 */

#pragma once
#include <wx/bitmap.h>
#include <string>

/**
 * @brief Load a bitmap from the specified path and cache it.
 *
 * The bitmap will be loaded only once and reused thereafter.
 * Safe to copy: wxBitmap shares internal data (ref-counted).
 *
 * @param path File path of the bitmap
 * @return Loaded wxBitmap
 */
wxBitmap LoadBitmapCached(const std::wstring& path);

/**
 * @brief Clears the bitmap cache
 */
void ClearBitmapCache();
