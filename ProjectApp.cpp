/**
* @file ProjectApp.cpp
 * @author Elle Hyry, Navya Malik
 * @brief Entry point and main frame creation for the project.
 *
 * The CProjectApp class initializes wxWidgets, loads image handlers,
 * and launches the game’s main frame window.
 */

#include "pch.h"
#include <wx/image.h>
#include "ProjectApp.h"
#include "MainFrame.h"

/**
 * @brief Initializes the game application.
 * @return True if initialization succeeded; false otherwise.
 *
 * This function initializes wxWidgets image handlers, creates the main
 * application window, and starts the event loop.
 */
bool CProjectApp::OnInit()
{
    if (!wxApp::OnInit()) return false;

    wxInitAllImageHandlers();
    auto* frame = new MainFrame("Spartan Touchdown - Rynearson");
    frame->Show(true);
    return true;
}

/// Implements the wxWidgets main application entry point.
wxIMPLEMENT_APP(CProjectApp);
