/**
 * @file Level.cpp
 * @author Benjamin Anderson, Jack Finlan-Hitt, Hartejas Sekhon
 * @brief Implementation of the CLevel class.
 */

#include "pch.h"
#include "Level.h"
#include "Game.h"

#include <wx/wx.h>
#include <wx/xml/xml.h>
#include <cstdlib>
#include <iostream>

#include "Background.h"
#include "Platform.h"
#include "ItemVisitor.h"
#include "UpdateItemVisitor.h"
#include "Coin.h"
#include "Enemy.h"
#include "Powerup.h"
#include "Wall.h"
#include "Goalpost.h"
#include "BossGoalPost.h"

/**
 * @brief Constructor, loads the level XML and creates declarations and items.
 */
CLevel::CLevel(CGame* game, const wxString& filename) : mGame(game)
{
    wxXmlDocument xmlDoc;
    if(!xmlDoc.Load(filename))
    {
        wxMessageBox(L"Unable to load level file: " + filename);
        return;  // Don't crash, just return
    }

    // General level info
    auto root = xmlDoc.GetRoot();
    mHeight = std::stoi(std::string(root->GetAttribute("height")));
    mWidth  = std::stoi(std::string(root->GetAttribute("width")));
    mStartX = std::stoi(std::string(root->GetAttribute("start-x")));
    mStartY = std::stoi(std::string(root->GetAttribute("start-y")));

    // declarations
    auto declarations = root->GetChildren();
    if (!declarations || declarations->GetName() != L"declarations")
    {
        std::cerr << "invalid level file: missing <declarations>" << std::endl;
        return;
    }

    for (auto declaration = declarations->GetChildren();
         declaration; declaration = declaration->GetNext())
    {
        CreateDeclaration(declaration);
    }

    // items
    auto items = declarations->GetNext();
    if (!items)
    {
        std::cerr << "invalid level file: missing <items>" << std::endl;
        return;
    }

    for (auto item = items->GetChildren(); item; item = item->GetNext())
    {
        CreateItem(item);
    }
}

/**
 * @brief Create a declaration object from XML node
 */
void CLevel::CreateDeclaration(wxXmlNode* Node)
{
    const wxString name = Node->GetName();
    const std::string id = std::string(Node->GetAttribute("id"));

    if (name == L"background")
    {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"platform")
    {
        auto decl = std::make_shared<CDeclaration>(
            Node->GetAttribute("left-image"),
            Node->GetAttribute("mid-image"),
            Node->GetAttribute("right-image"));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"coin")
    {
        auto decl = std::make_shared<CDeclaration>(
            Node->GetAttribute("image"),
            std::stoi(std::string(Node->GetAttribute("value"))));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"wall")
    {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"enemy")
    {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"power-up")
    {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
    }
    else if (name == L"goalpost")
        {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
        }
    else if (name == L"boss-goalpost")
    {
        auto decl = std::make_shared<CDeclaration>(Node->GetAttribute("image"));
        mDeclarations.insert({id, decl});
    }
}

/**
 * @brief Create an item object from XML node
 */
void CLevel::CreateItem(wxXmlNode* Node)
{
    const wxString nodeName = Node->GetName();
    const std::string id = std::string(Node->GetAttribute("id"));
    const auto declIt = mDeclarations.find(id);
    if (declIt == mDeclarations.end())
        return;

    const auto& decl = declIt->second;

    const double x = std::stod(std::string(Node->GetAttribute("x")));
    const double y = std::stod(std::string(Node->GetAttribute("y")));

    if (nodeName == L"background")
    {
        auto background = std::make_shared<CBackground>(
            mGame,
            decl->GetImageMid(),
            x, y);
        mItems.push_back(background);
    }
    else if (nodeName == L"coin")
    {
        auto coin = std::make_shared<CCoin>(
            mGame,
            decl->GetImageMid(),
            x, y,
            decl->GetValue());
        coin->EnableRepel(80.0);
        mItems.push_back(coin);
    }
    else if (nodeName == L"platform")
    {
        const int width  = std::stoi(std::string(Node->GetAttribute("width")));
        const int height = std::stoi(std::string(Node->GetAttribute("height")));

        auto platform = std::make_shared<CPlatform>(
            mGame,
            decl->GetImageLeft(),
            decl->GetImageMid(),
            decl->GetImageRight(),
            x, y, width, height);
        mItems.push_back(platform);
    }
    else if (nodeName == L"wall")
    {
        auto wall = std::make_shared<CWall>(
            mGame,
            decl->GetImageMid(), // std::wstring
            x, y
        );
        wall->SetSize(
            std::stoi(std::string(Node->GetAttribute("width"))),
            std::stoi(std::string(Node->GetAttribute("height")))
        );
        mItems.push_back(wall);
    }
    else if (nodeName == L"enemy")
    {
        EnemyType type = EnemyType::Patrol;
        if (Node->HasAttribute("type"))
        {
            std::string typeStr = std::string(Node->GetAttribute("type"));
            if (typeStr == "stationary")
                type = EnemyType::Stationary;
            else if (typeStr == "chaser")
                type = EnemyType::Chaser;
            else if (typeStr == "vertical")
                type = EnemyType::Vertical; // <-- optional new vertical type
        }

        auto enemy = std::make_shared<CEnemy>(
            mGame,
            decl->GetImageMid(),
            x, y,
            type);

        mItems.push_back(enemy);
    }
    else if (nodeName == L"power-up")
    {

        PowerType type = PowerType::Coin;
        if (Node->HasAttribute("type"))
        {
            std::string typeStr = std::string(Node->GetAttribute("type"));
            if (typeStr == "double-jump")
                type = PowerType::DoubleJump;
            else if (typeStr == "dash")
                type = PowerType::Dash;
            else if (typeStr == "coin-rush")
                type = PowerType::CoinRush;
        }

        auto powerup = std::make_shared<CPowerup>(
            mGame,
            decl->GetImageMid(),
            x, y, type);
        powerup->EnableRepel(60.0);

        if (Node->HasAttribute("width") && Node->HasAttribute("height"))
        {
            double width = std::stod(std::string(Node->GetAttribute("width")));
            double height = std::stod(std::string(Node->GetAttribute("height")));
            powerup->SetSize(width, height);
        }
        mItems.push_back(powerup);
    }
    else if (nodeName == L"goalpost")
    {
        // This creates the Goalpost object and adds it to the game
        auto goalpost = std::make_shared<CGoalpost>(
            mGame,
            decl->GetImageMid(), // Gets the image path from the declaration
            x, y
        );
        mItems.push_back(goalpost);
    }
    else if (nodeName == L"boss-goalpost")
    {
        auto boss = std::make_shared<CBossGoalpost>(
            mGame,
            decl->GetImageMid(),
            x, y
        );
        mItems.push_back(boss);
    }
}

