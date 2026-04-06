/**
* @file MainFrame.h
 * @author Elle Hyry, Hartejas Sekhon
 * @brief Declaration of the MainFrame class, the main window frame for the game.
 */

#ifndef PROJECT1_MAINFRAME_H
#define PROJECT1_MAINFRAME_H

#include <wx/wx.h>
#include <wx/timer.h>
#include "Game.h"
#include "GameView.h"
#include <wx/icon.h>

/**
 * @class MainFrame
 * @brief The main window frame that manages rendering, input, and level control.
 */
class MainFrame : public wxFrame
{
private:
    std::unique_ptr<CGame> mGame;    ///< The main game instance.
    wxTimer mTimer;                  ///< Timer for updating and redrawing frames.
    wxStopWatch mStopWatch;          ///< Stopwatch for measuring frame delta time.

    void OnPaint(wxPaintEvent& event);   ///< Paint handler for rendering the game.
    void OnTimer(wxTimerEvent& event);   ///< Timer handler for periodic updates.
    void OnKeyDown(wxKeyEvent& event);   ///< Handle key press events.
    void OnKeyUp(wxKeyEvent& event);     ///< Handle key release events.
    void OnMouseClick(wxMouseEvent& event); ///< Handle mouse click focus events.

    void OnLoadLevel0(wxCommandEvent& event); ///< Load Level 0.
    void OnLoadLevel1(wxCommandEvent& event); ///< Load Level 1.
    void OnLoadLevel2(wxCommandEvent& event); ///< Load Level 2.
    void OnLoadLevel3(wxCommandEvent& event); ///< Load Level 3.
    void OnAbout(wxCommandEvent &event);
    void OnHelpAbout(wxCommandEvent& event);

    void OnCharHook(wxKeyEvent& event);  ///< Handle special key inputs like ESC.
    void OnExit(wxCommandEvent& event);  ///< Handle program exit.
    void OnClose(wxCloseEvent& event);   ///< Handle window close event.

public:
    /**
     * @brief Construct a new MainFrame object.
     * @param title The title for the game window.
     */
    explicit MainFrame(const wxString& title);
    /// Declare event table
    wxDECLARE_EVENT_TABLE();
};

#endif // PROJECT1_MAINFRAME_H
