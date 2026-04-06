/**
* @file UpdateItemVisitor.cpp
 * @author Benjamin Anderson
 * @brief Implementation of the CUpdateItemVisitor class.
 */

#include "pch.h"
#include "UpdateItemVisitor.h"
#include "Platform.h"
#include "Background.h"


void CUpdateItemVisitor::UpdateItem(std::shared_ptr<CItem> item, wxXmlNode* node)
{
    item->Accept(this);

    if (mThisPlatform != nullptr)
    {
        // Set platform data from XML
        mThisPlatform->SetLocation(
            std::stoi(std::string(node->GetAttribute("x"))),
            std::stoi(std::string(node->GetAttribute("y")))
        );
        mThisPlatform->SetSize(
            std::stoi(std::string(node->GetAttribute("width"))),
            std::stoi(std::string(node->GetAttribute("height")))
        );

        mThisPlatform = nullptr;
    }
    else if (mThisBackground != nullptr)
    {
        // Set background data from XML
        mThisBackground->SetLocation(
            std::stoi(std::string(node->GetAttribute("x"))),
            std::stoi(std::string(node->GetAttribute("y")))
        );

        mThisBackground = nullptr;
    }
}
