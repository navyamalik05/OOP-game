/**
 * @file MainFrame.cpp
 * @author Elle Hyry, Hartejas Sekhon
 * @brief Implementation of the MainFrame class for managing the main game window.
 */

#include "pch.h"
#include "MainFrame.h"
#include "ids.h"
#include "Game.h"
#include <wx/wx.h>
#include <wx/dcbuffer.h>
/**
 * Event table for Mainframe
 */
wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_PAINT(MainFrame::OnPaint)
    EVT_TIMER(wxID_ANY, MainFrame::OnTimer)
    EVT_KEY_DOWN(MainFrame::OnKeyDown)
    EVT_KEY_UP(MainFrame::OnKeyUp)
    EVT_LEFT_DOWN(MainFrame::OnMouseClick)
wxEND_EVENT_TABLE()
MainFrame::MainFrame(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title,
              wxDefaultPosition, wxSize(800, 600),
              wxDEFAULT_FRAME_STYLE | wxWANTS_CHARS),
      mTimer(this)
{

    // Load the icon from the images folder
    wxIcon icon(L"images/football.ico", wxBITMAP_TYPE_ICO);
    if (icon.IsOk())
    {
        SetIcon(icon);
    }

    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(*wxBLACK);

    mGame = std::make_unique<CGame>();

    // Initialize timers
    mStopWatch.Start();
    mTimer.Start(16); // ~60 FPS

    //
    // --- MENUS ---
    //
    auto menuBar = new wxMenuBar();

    // File menu
    auto fileMenu = new wxMenu();
    fileMenu->Append(wxID_ABOUT, L"&About\tCtrl+A", L"Show information about this game");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, L"&Exit\tCtrl+Q", L"Exit the game");
    menuBar->Append(fileMenu, L"&File");

    // Levels menu
    auto levelsMenu = new wxMenu();
    levelsMenu->Append(IDM_LEVEL_0, L"&Level 0", L"Load Level 0");
    levelsMenu->Append(IDM_LEVEL_1, L"&Level 1", L"Load Level 1");
    levelsMenu->Append(IDM_LEVEL_2, L"&Level 2", L"Load Level 2");
    levelsMenu->Append(IDM_LEVEL_3, L"&Level 3", L"Load Level 3");
    menuBar->Append(levelsMenu, L"&Levels");

    auto helpMenu = new wxMenu();
    helpMenu->Append(IDM_HELP_ABOUT, L"&About", L"Show app info");
    menuBar->Append(helpMenu, L"&Help");

    SetMenuBar(menuBar);

    //
    // --- EVENT BINDINGS ---
    //
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoadLevel0, this, IDM_LEVEL_0);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoadLevel1, this, IDM_LEVEL_1);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoadLevel2, this, IDM_LEVEL_2);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnLoadLevel3, this, IDM_LEVEL_3);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnExit, this, wxID_EXIT);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnClose, this);
    Bind(wxEVT_CHAR_HOOK, &MainFrame::OnCharHook, this);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &MainFrame::OnHelpAbout, this, IDM_HELP_ABOUT);


    // Load default level
    mGame->Load(1);
    SetFocus();

    // Ensure focus on window activation
    Bind(wxEVT_ACTIVATE, [this](wxActivateEvent& e){
         if (e.GetActive()) {
             this->SetFocus();
             this->SetFocusFromKbd();
         }
         e.Skip();
     });
}

/**
 * @brief Paint the game view on window refresh.
 * @param event the event that happens
 */
void MainFrame::OnPaint(wxPaintEvent& event)
{
    wxAutoBufferedPaintDC dc(this);
    wxGraphicsContext* gc = wxGraphicsContext::Create(dc);
    if (gc)
    {
        wxSize clientSize = GetClientSize();

        // Maintain consistent virtual height
        const int VirtualHeight = 1024;
        double scale = double(clientSize.GetHeight()) / VirtualHeight;
        int VirtualWidth = clientSize.GetWidth() / scale;

        gc->Scale(scale, scale);
        mGame->OnDraw(gc, VirtualWidth, VirtualHeight);
        delete gc;
    }
}

/**
 * @brief Handle frame updates and timing.
 * @param event the event that happens
 */
void MainFrame::OnTimer(wxTimerEvent& event)
{
    double elapsed = mStopWatch.Time() / 1000.0;
    mStopWatch.Start(0);

    if (mGame->ShouldRestartLevel())
    {
        mGame->Load(mGame->GetLevel());
        mGame->ClearRestartFlag();
    }

    mGame->Update(elapsed);
    Refresh();
    wxTheApp->Yield(true);
}
/// @param e event
void MainFrame::OnKeyDown(wxKeyEvent& e){ mGame->OnKeyDown(e); }
/// @param e event
void MainFrame::OnKeyUp(wxKeyEvent& e){ mGame->OnKeyUp(e); }

/**
 * @brief Handle mouse click to restore keyboard focus.
 * @param event the event that happens
 */
void MainFrame::OnMouseClick(wxMouseEvent& event)
{
    SetFocus();
    event.Skip();
}

/**
 * @brief Menu handler for Level 0.
 * @param event the event that happened
 */
void MainFrame::OnLoadLevel0(wxCommandEvent& event)
{
    mGame->Load(0);
    Refresh();
    SetFocus();
}

/**
 * @brief Menu handler for Level 1.
 * @param event the event that happened
 */
void MainFrame::OnLoadLevel1(wxCommandEvent& event)
{
    mGame->Load(1);
    Refresh();
    Raise();
    SetFocus();
}

/**
 * @brief Menu handler for Level 2.
 * @param event the event that happened
 */
void MainFrame::OnLoadLevel2(wxCommandEvent& event)
{
    mGame->Load(2);
    Refresh();
    Raise();
    SetFocus();
}

/**
 * @brief Menu handler for Level 3.
 * @param event the event that happened
 */
void MainFrame::OnLoadLevel3(wxCommandEvent& event)
{
    mGame->Load(3);
    Refresh();
    Raise();
    SetFocus();
}

/**
 * @brief Display the About dialog with project information.
 * @param event the event that happened
 */
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox(
        L"My Game Project\n\n"
        L"Developed by:\n"
        L"  • Elle Hyry\n"
        L"  • Navya Malik\n"
        L"  • Jack Finlan-Hitt\n"
        L"  • Hartejas\n"
        L"  • Benjamin Anderson\n\n"
        L"Michigan State University\n"
        L"CSE 335 - Project 1\n\n"
        L"(c) 2025 All Rights Reserved.",
        L"About My Game Project",
        wxOK | wxICON_INFORMATION,
        this
    );
}

/**
 * @brief Handle character hooks such as ESC key.
 * @param event the event that happened
 */
void MainFrame::OnCharHook(wxKeyEvent& event)
{
    if (event.GetKeyCode() == WXK_ESCAPE)
    {
        mTimer.Stop();
        Close(true);
        return;
    }

    mGame->OnKeyDown(event);
}

/**
 * @brief Handle application exit through File > Exit.
 * @param event the event that happened
 */
void MainFrame::OnExit(wxCommandEvent& event)
{
    mTimer.Stop();
    Close(true);
}

/**
 * @brief Handle window close event (clicking 'X').
 * @param event the event that happened
 */
void MainFrame::OnClose(wxCloseEvent& event)
{
    mTimer.Stop();
    Close(true);
}

/**
 * Handler for the Help - About menu item
 */
void MainFrame::OnHelpAbout(wxCommandEvent& event)
{
    wxMessageBox(L"Spartan-touchdown", L"About", wxOK | wxICON_INFORMATION);
}