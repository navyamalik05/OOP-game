/**
 * @file UpdateItemVisitor.h
 * @author Benjamin Anderson
 * @brief Visitor class for updating item properties from XML data.
 *
 * The CUpdateItemVisitor class implements a visitor that updates
 * game items (such as platforms and backgrounds) based on information
 * read from an XML node. It is part of the visitor pattern architecture
 * used throughout the game.
 */

#ifndef PROJECT1_UPDATEITEMVISITOR_H
#define PROJECT1_UPDATEITEMVISITOR_H

#include <map>
#include <memory>
#include <string>
#include <wx/xml/xml.h>

#include "Item.h"
#include "ItemVisitor.h"

class CPlatform;
class CBackground;

/**
 * @class CUpdateItemVisitor
 * @brief A visitor used to update game items from XML nodes.
 *
 * This visitor updates item properties such as position and size
 * by reading their values from an XML node. It supports platforms
 * and backgrounds, and can be extended to handle other item types.
 */
class CUpdateItemVisitor : public CItemVisitor
{
private:
    /// Pointer to the collection of items being updated
    std::map<std::string, std::shared_ptr<CItem>>* mItems = nullptr;

    /// Pointer to the XML node describing the item
    wxXmlNode* mItemsNode = nullptr;

    /// The platform currently being visited (if any)
    CPlatform* mThisPlatform = nullptr;

    /// The background currently being visited (if any)
    CBackground* mThisBackground = nullptr;

public:
    /**
     * @brief Default constructor.
     *
     * Initializes internal pointers to nullptr.
     */
    CUpdateItemVisitor() : mThisPlatform(nullptr), mThisBackground(nullptr) {}

    /**
     * @brief Visit a platform item.
     * @param platform Pointer to the platform being visited.
     */
    void VisitPlatform(CPlatform* platform) override { mThisPlatform = platform; }

    /**
     * @brief Visit a background item.
     * @param background Pointer to the background being visited.
     */
    void VisitBackground(CBackground* background) override { mThisBackground = background; }

    /**
     * @brief Update an item using XML data.
     * @param item The item to update.
     * @param node The XML node containing the item’s properties.
     *
     * Updates position and size attributes depending on the item type.
     */
    void UpdateItem(std::shared_ptr<CItem> item, wxXmlNode* node);
};

#endif // PROJECT1_UPDATEITEMVISITOR_H
