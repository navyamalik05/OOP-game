/**
* @file ProjectApp.h
 * @author Elle Hyry
 * @brief Application entry class for the project.
 */

#ifndef PROJECT1_PROJECTAPP_H
#define PROJECT1_PROJECTAPP_H

#include <wx/wx.h>

/**
 * @class CProjectApp
 * @brief Main application class
 */
class CProjectApp : public wxApp
{
public:
    bool OnInit() override; ///< Initialize the app
};

#endif // PROJECT1_PROJECTAPP_H
