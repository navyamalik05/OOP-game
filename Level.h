/**
 * @file Level.h
 * @author Benjamin Anderson, Jack Finlan-Hitt
 * @brief Represents a game level, including items, platforms, enemies, and their declarations.
 */
#ifndef PROJECT1_LEVEL_H
#define PROJECT1_LEVEL_H

#include <wx/xml/xml.h>
#include <map>
#include <memory>
#include <string>
#include "Item.h"

// Forward declaration to avoid include loop
class CGame;

/**
 * @class CLevel
 * @brief Represents a game level and manages all items and declarations within it.
 */
class CLevel
{
private:
    /// Owning game (needed to construct items)
    CGame* mGame = nullptr;
    /// Level height
    int mHeight = 0;
    /// Level width
    int mWidth  = 0;
    /// Football start Y
    int mStartY = 0;
    /// Football start X
    int mStartX = 0;
    /// Items in the level
    std::vector<std::shared_ptr<CItem>> mItems;

    /**
     * @class CDeclaration
     * @brief Represents a visual or coin declaration used to construct items.
     */
    class CDeclaration
    {
    private:
        /// Left image
        std::wstring mImageLeft;
        /// Middle image
        std::wstring mImageMid;
        /// RIght image
        std::wstring mImageRight;
        /// Value
        int mValue = 0;

    public:
        /// Platform constructor (left/mid/right)
        /// @param imageLeft, imageMid, imageRight left middle and right images
        CDeclaration(const wxString& imageLeft,
                     const wxString& imageMid,
                     const wxString& imageRight)
            : mImageLeft(L"images/" + imageLeft.ToStdWstring()),
              mImageMid(L"images/" + imageMid.ToStdWstring()),
              mImageRight(L"images/" + imageRight.ToStdWstring()) {}

        /// Single image constructor (background, wall, enemy, powerup, etc.)
        /// @param imageMid middle image
        explicit CDeclaration(const wxString& imageMid)
            : mImageMid(L"images/" + imageMid.ToStdWstring()) {}

        /// Coin constructor (image + value)
        /// @param imageMid, value middle image and value
        CDeclaration(const wxString& imageMid, int value)
            : mImageMid(L"images/" + imageMid.ToStdWstring()), mValue(value) {}
        /// @return left image
        const std::wstring& GetImageLeft()  const { return mImageLeft; }
        /// @return middle image
        const std::wstring& GetImageMid()   const { return mImageMid; }
        /// @return right image
        const std::wstring& GetImageRight() const { return mImageRight; }
        /// @return value
        int GetValue() const { return mValue; }
    };
    /// Map of declaration IDs to their objects
    std::map<std::string, std::shared_ptr<CDeclaration>> mDeclarations;

public:
    /**
     * @brief Constructor, loads level from XML file
     * @param game Pointer to owning game
     * @param filename XML level filename
     */
    CLevel(CGame* game, const wxString& filename);
    /// @return start y
    int GetStartY() const { return mStartY; }
    /// @return start x
    int GetStartX() const { return mStartX; }

    /**
     * @brief Create a declaration from an XML node
     * @param Node XML node containing declaration info
     */
    void CreateDeclaration(wxXmlNode* Node);

    /**
     * @brief Create an item from an XML node
     * @param Node XML node containing item info
     */
    void CreateItem(wxXmlNode* Node);

    /// @return Reference to the vector of items
    std::vector<std::shared_ptr<CItem>>& GetItems() { return mItems; }
    /// Get level width
    /// @return width
    int GetWidth()  const { return mWidth; }
    /// Get level height
    /// @return height
    int GetHeight() const { return mHeight; }
};

#endif // PROJECT1_LEVEL_H
