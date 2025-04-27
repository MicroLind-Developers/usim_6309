// === File: MainView.h ===
#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/stc/stc.h>
#include <wx/grid.h>
#include "mc6309.h"

class MainView : public wxFrame {
public:
    MainView();

    void displayHex(const std::string& hex);
    void displayRegisters(const mc6309::Registers& r);

    void bind(wxEvtHandler* h, int id, wxEventFunction f);
    int getMenuIdOpen() const;
    int getMenuIdStart() const;
    int getMenuIdStop() const;

private:
    wxNotebook*       notebook;
    wxStyledTextCtrl* hexView;
    wxGrid*           regGrid;
};