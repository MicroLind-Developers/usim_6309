// === File: MainController.cpp ===
#include "MainController.h"
#include <wx/filedlg.h>
#include <wx/log.h>

MainController::MainController(CPUModel& m, MainView& v)
  : model(m), view(v)
{
    view.bind(this, view.getMenuIdOpen(),  (wxEventFunction)&MainController::onLoad);
    view.bind(this, view.getMenuIdStart(), (wxEventFunction)&MainController::onStart);
    view.bind(this, view.getMenuIdStop(),  (wxEventFunction)&MainController::onStop);
    view.bind(this, wxID_EXIT,             (wxEventFunction)&MainController::onExit);
}

void MainController::onLoad(wxCommandEvent&) {
    wxFileDialog dlg(nullptr, "Open HEX", "", "", "HEX Files (*.hex)|*.hex", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() != wxID_OK) return;
    hexfile = dlg.GetPath().ToStdString();
}

void MainController::onStart(wxCommandEvent&) {
    if (!model.init()) {
        wxLogError("Failed to initialize CPU with %s", hexfile);
        return;
    }
    view.displayHex("Loaded: " + hexfile);
    view.displayRegisters(model.getRegisters());
    model.startRunLoop();
}

void MainController::onStop(wxCommandEvent&) {
    model.stop();
    view.displayRegisters(model.getRegisters());
}

void MainController::onExit(wxCommandEvent&) {
    model.stop();
    wxTheApp->ExitMainLoop();
}