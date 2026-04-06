/**
 * @file Game.cpp
 * @author Hartejas Sekhon, Jack Finlan-Hitt
 * @brief Stub implementation for CGame class.
 */

#include "pch.h"
#include "Game.h"
#include "Item.h"
#include "Football.h"
#include "Level.h"
#include "GameView.h"
#include "ImageCache.h"
#include "Coin.h"
#include "ItemVisitor.h"
#include "Powerup.h"

#include <wx/wx.h>
#include <wx/filefn.h>   // wxFileExists, wxGetCwd
#include <memory>
#include <algorithm>
/**
 * Initialize the current level and create the player
 */
void CGame::Initialize()
{
    const wxString filename = wxString::Format("levels/level%d.xml", mCurrentLevel);

    CLevel level(this, filename);

    for (auto& item : level.GetItems())
    {
        mItems.push_back(item);
    }

    auto football = std::make_shared<CFootball>(this, level.GetStartX(), level.GetStartY());
    AddItem(football);
    mFootball = football;
}

CGame::CGame()
{
    mGameView = std::make_unique<CGameView>();

    // Load instruction icons
    mIconDash = wxBitmap(L"images/powerup-speedboost.png", wxBITMAP_TYPE_ANY);
    mIconJump = wxBitmap(L"images/powerup-doublejump.png", wxBITMAP_TYPE_ANY);
    mIconCoin = wxBitmap(L"images/powerup-coinrush.png", wxBITMAP_TYPE_ANY);
    // From your files, the boss is the "goalpost"
    mIconBoss = wxBitmap(L"images/sparty-fish.png", wxBITMAP_TYPE_ANY);
}

CGame::~CGame() = default;

void CGame::Load(int levelNum)
{
    mItems.clear();
    ClearBitmapCache();
    mFootball = nullptr;

    const wxString filename = wxString::Format("levels/level%d.xml", levelNum);

    if (!wxFileExists(filename))
    {
        wxMessageBox("Cannot find level file: " + filename + "\nCurrent directory: " + wxGetCwd());
        return;
    }

    CLevel level(this, filename);

    for (auto& item : level.GetItems())
    {
        mItems.push_back(item);
    }

    mFootball = std::make_shared<CFootball>(this, level.GetStartX(), level.GetStartY());
    mItems.push_back(mFootball);

    mScoreboard.Reset();

    // Check if this is the *first* time we are loading level 3
    bool isFirstLoadLevel3 = (levelNum == 3 && mCurrentLevel != 3);

    mCurrentLevel = levelNum; // Update current level

    if (mCurrentLevel == 3)
    {
        mGameState = GameState::Level3Start; // New state
        mCurrentMessage = "LEVEL 3: THE BOSS";
        mMessageTimer = 0.0; // Timer will count up
    }
    else
    {
        mGameState = GameState::LevelStart; // Existing state
        mCurrentMessage = wxString::Format("Level %d", mCurrentLevel);
        mMessageTimer = 0.0; // Timer will count up
    }
}

void CGame::AddItem(std::shared_ptr<CItem> item)
{
    mItems.push_back(std::move(item));
}

void CGame::OnDraw(wxGraphicsContext* graphics, int width, int height)
{
    if (!graphics || !mGameView) return;

    if (mFootball)
    {
        double offset = mFootball->GetX() - width / 2.0;
        if (offset < 0.0) offset = 0.0;
        mGameView->SetXOffset(offset);
    }

    mGameView->OnDraw(graphics, width, height, mItems);

    // Draw level start message
    if (mGameState == GameState::LevelStart)
    {
        wxFont font(wxFontInfo(48).Bold());
        graphics->SetFont(font, *wxWHITE); // You can change this color

        double tw, th;
        graphics->GetTextExtent(mCurrentMessage, &tw, &th);

        double x = (width - tw) / 2.0;
        double y = (height - th) / 2.0;

        graphics->SetBrush(wxBrush(wxColor(0, 0, 0, 180))); // Semi-transparent bg
        graphics->SetPen(*wxTRANSPARENT_PEN);
        graphics->DrawRectangle(x - 20, y - 20, tw + 40, th + 40);
        graphics->DrawText(mCurrentMessage, x, y);
    }
    else if (mGameState == GameState::Level3Start)
    {
        // Draw semi-transparent background
        graphics->SetBrush(wxBrush(wxColor(0, 0, 0, 220))); // Almost opaque
        graphics->SetPen(*wxTRANSPARENT_PEN);
        graphics->DrawRectangle(0, 0, width, height);

        // Draw Title
        wxFont titleFont(wxFontInfo(48).Bold());
        graphics->SetFont(titleFont, *wxRED);
        double tw, th;
        graphics->GetTextExtent(mCurrentMessage, &tw, &th);
        double titleX = (width - tw) / 2.0;
        double titleY = 100;
        graphics->DrawText(mCurrentMessage, titleX, titleY);

        //Level 3 instructions
        wxFont instructionFont(wxFontInfo(24));
        graphics->SetFont(instructionFont, *wxWHITE);

        // Center the text block
        double lineX = 200;
        double lineY = 250;
        double iconSize = 48;
        double padding = 20;

        // Line 1 - Dash powerup
        graphics->DrawBitmap(mIconDash, lineX, lineY, iconSize, iconSize);
        graphics->DrawText(L"Collect to UNLOCK DASH (SHIFT)", lineX + iconSize + padding, lineY + 10);
        lineY += iconSize + padding;

        // Line 2: Double Jump powerup
        graphics->DrawBitmap(mIconJump, lineX, lineY, iconSize, iconSize);
        graphics->DrawText(L"Collect to UNLOCK DOUBLE JUMP (SPACE)", lineX + iconSize + padding, lineY + 10);
        lineY += iconSize + padding;

        // Line 3- Coin Rush powerup
        graphics->DrawBitmap(mIconCoin, lineX, lineY, iconSize, iconSize);
        graphics->DrawText(L"Collect for COIN RUSH + BONUS", lineX + iconSize + padding, lineY + 10);
        lineY += iconSize + padding;

        // Line 4- Boss icon
        graphics->DrawBitmap(mIconBoss, lineX, lineY, iconSize, iconSize);
        graphics->DrawText(L"Find this BOSS...", lineX + iconSize + padding, lineY + 10);
        lineY += iconSize + padding + 10; // Extra padding

        // Line 5: Instructions
        wxFont boldInstructionFont = instructionFont.Bold();
        graphics->SetFont(boldInstructionFont, *wxWHITE);
        graphics->DrawText(L"HIT THE BOSS 3 TIMES WITH YOUR DASH TO WIN!", lineX, lineY + 30);

        //Loading...
        wxFont loadingFont(wxFontInfo(28).Bold());
        graphics->SetFont(loadingFont, *wxWHITE);

        wxString loadingText = L"LOADING...";
        double lw, lh;
        graphics->GetTextExtent(loadingText, &lw, &lh);

        double loadingX = (width - lw) / 2.0;
        double loadingY = height - 150; // positioned near bottom
        graphics->DrawText(loadingText, loadingX, loadingY);

    }

    // Draw level complete message
    else if (mGameState == GameState::LevelComplete)
    {
        wxFont font(wxFontInfo(48).Bold());
        graphics->SetFont(font, *wxGREEN);

        wxString message = "Level Complete!";
        double tw, th;
        graphics->GetTextExtent(message, &tw, &th);

        double x = (width - tw) / 2.0;
        double y = (height - th) / 2.0;

        graphics->SetBrush(wxBrush(wxColor(0, 0, 0, 180)));
        graphics->SetPen(*wxTRANSPARENT_PEN);
        graphics->DrawRectangle(x - 20, y - 20, tw + 40, th + 40);
        graphics->DrawText(message, x, y);
    }
    else if (mGameState == GameState::YouLose)
    {
        wxFont font(wxFontInfo(48).Bold());
        graphics->SetFont(font, *wxRED);

        wxString message = "You Lose!";
        double tw, th;
        graphics->GetTextExtent(message, &tw, &th);

        double x = (width - tw) / 2.0;
        double y = (height - th) / 2.0;

        graphics->SetBrush(wxBrush(wxColor(0, 0, 0, 180)));
        graphics->SetPen(*wxTRANSPARENT_PEN);
        graphics->DrawRectangle(x - 20, y - 20, tw + 40, th + 40);
        graphics->DrawText(message, x, y);
    }

    //Overlay for level 3
    if (mCurrentLevel == 3 && mFootball && mGameState == GameState::Playing)
    {
        // Get the football's position relative to the SCREEN (not the world)
        double footballScreenX = mFootball->GetX() - mGameView->GetXOffset();
        double footballScreenY = mFootball->GetY();
        double fogRadius = 300.0; // The radius of the visible circle

        wxGraphicsPath path = graphics->CreatePath();

        //  Add a giant rectangle covering the whole screen
        path.AddRectangle(0, 0, width, height);

        //  Add a circle "hole" at the football's position
        path.AddCircle(footballScreenX, footballScreenY, fogRadius);

        // Fill the path with semi-transparent black
        graphics->SetBrush(wxBrush(wxColour(0, 0, 0, 245))); // 220 alpha
        graphics->SetPen(*wxTRANSPARENT_PEN);
        graphics->FillPath(path, wxODDEVEN_RULE);

        if (mCurrentLevel == 3)
        {
            // We must re-apply the camera scroll
            graphics->PushState();
            double offset = mFootball ? mFootball->GetX() - mGameView->GetXOffset() : 0.0;
            if (offset < 0.0) offset = 0.0;
            graphics->Translate(-offset, 0);

            for (auto& item : mItems)
            {
                // Check if this item is a CPowerup
                if (auto powerup = dynamic_cast<CPowerup*>(item.get()))
                {
                    // If the cast is successful, *now* we can check IsCollected()
                    if (!powerup->IsCollected())
                    {
                        powerup->Draw(graphics); // Draw it again
                    }
                }
            }

            graphics->PopState();
        }

    }

    mScoreboard.Draw(graphics, width, height);


}



void CGame::Update(double elapsed)
{
    if (mGameState == GameState::LevelComplete || mGameState == GameState::YouLose)
    {
        mMessageTimer += elapsed;
        if (mMessageTimer >= mMessageDuration)
        {
            mMessageTimer = 0.0;
            //store whether we won or not
            bool wasLevelComplete = (mGameState == GameState::LevelComplete);

            // reset the game state
            mGameState = GameState::Playing;

            // Check the stored state
            if (wasLevelComplete)
            {
                mCurrentLevel++;
                if (mCurrentLevel > 3) mCurrentLevel = 3;
            }
            Load(mCurrentLevel);
        }
        return;
    }

    if (mGameState == GameState::LevelStart || mGameState == GameState::Level3Start)
    {
        mMessageTimer += elapsed;

        // Level 3 instructions last 10 seconds, others last 2 seconds
        double duration = (mGameState == GameState::Level3Start) ? 10.0 : mMessageDuration;

        if (mMessageTimer >= duration)
        {
            mGameState = GameState::Playing;
            mMessageTimer = 0.0;
        }
        // By returning here, we freeze the game
        return;
    }

    //Coin Rush Timer and Spawning coins
    if (mIsCoinRushActive)
    {
        mCoinRushTimer -= elapsed;
        mCoinSpawnTimer -= elapsed;

        if (mCoinRushTimer <= 0.0)
        {
            mIsCoinRushActive = false; // Deactivate
        }
        else if (mCoinSpawnTimer <= 0.0 && mFootball)
        {
            // Spawn coin slightly above the football
            SpawnCoin(mFootball->GetX(), mFootball->GetY() - 20);
            mCoinSpawnTimer = mCoinSpawnRate;
        }
    }


    constexpr double MaxElapsed = 0.016;
    while (elapsed > MaxElapsed)
    {
        for (auto& item : mItems) { item->Update(MaxElapsed); }
        mScoreboard.Update(MaxElapsed);
        elapsed -= MaxElapsed;
    }
    if (elapsed > 0.0)
    {
        for (auto& item : mItems) { item->Update(elapsed); }
        mScoreboard.Update(elapsed);
    }
}

void CGame::OnKeyDown(wxKeyEvent& event)
{
    if (mFootball) mFootball->OnKeyDown(event);
}

void CGame::OnKeyUp(wxKeyEvent& event)
{
    if (mFootball) mFootball->OnKeyUp(event);
}

int CGame::GetScore() const { return mScoreboard.GetScore(); }
double CGame::GetElapsedTime() const { return mScoreboard.GetElapsedTime(); }

void CGame::DoubleCoins()
{
    CDoubleCoinsVisitor visitor;
    for (auto& item : mItems) visitor.DoubleCoin(item);
}

void CGame::LevelCompleted()
{
    if (mGameState == GameState::Playing)
    {
        mGameState = GameState::LevelComplete;
        mMessageTimer = 0.0;
    }
}

void CGame::YouLose()
{
    if (mGameState == GameState::Playing)
    {
        mGameState = GameState::YouLose;
        mMessageTimer = 0.0;
    }
}

/**
 * @brief Spawns a coin at the given position.
 * @param x X location to spawn
 * @param y Y location to spawn
 */
void CGame::SpawnCoin(double x, double y)
{
    // with id "i008" from your level2.xml.
    // A safer way is to use the image path directly
    const std::wstring coinImage = L"images/coin100.png";
    const int coinValue = 100;

    auto coin = std::make_shared<CCoin>(this, coinImage, x, y, coinValue);

    // The spawned coin will just appear and fall (or fly up if CCoin::Update has logic)
    AddItem(coin);
}

/**
 * @brief Safely remove a list of items from the game (for Dash-Kill/Boss)
 * @param toRemove A vector of raw CItem pointers to remove
 */
void CGame::RemoveItems(const std::vector<CItem*>& toRemove)
{
    for (CItem* itemToRemove : toRemove)
    {
        // Find shared_ptr corresponding to the raw pointer
        auto it = std::find_if(mItems.begin(), mItems.end(),
            [itemToRemove](const std::shared_ptr<CItem>& p) {
                return p.get() == itemToRemove;
            });

        // If found, erase from the main vector
        if (it != mItems.end())
        {
            mItems.erase(it);
        }
    }
}
