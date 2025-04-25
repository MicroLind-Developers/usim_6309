
// === File: MainController.h ===
#pragma once

#include <string>
#include "CPUModel.h"
#include "MainView.h"

class MainController : public wxEvtHandler {
public:
    MainController(CPUModel& model, MainView& view);

private:
    void onLoad(wxCommandEvent&);
    void onStart(wxCommandEvent&);
    void onStop(wxCommandEvent&);
    void onExit(wxCommandEvent&);

    CPUModel& model;
    MainView& view;
    std::string hexfile;
};