// === File: MainView.cpp ===
#include "MainView.h"

MainView::MainView()
  : wxFrame(nullptr, wxID_ANY, "6809 Simulator (MVC)", wxDefaultPosition, wxSize(800, 600))
{
    wxMenu* fileMenu = new wxMenu;
    fileMenu->Append(wxID_OPEN, "&Load Hex...\tCtrl+L");
    fileMenu->Append(wxID_HIGHEST+1, "&Start\tCtrl+R");
    fileMenu->Append(wxID_HIGHEST+2, "&Stop\tCtrl+S");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "E&xit\tAlt-X");

    wxMenuBar* mb = new wxMenuBar;
    mb->Append(fileMenu, "&File");
    SetMenuBar(mb);
    CreateStatusBar();

    notebook = new wxNotebook(this, wxID_ANY);
    hexView = new wxStyledTextCtrl(notebook, wxID_ANY);
    hexView->SetReadOnly(true);
    notebook->AddPage(hexView, "Hex View");

    regGrid = new wxGrid(notebook, wxID_ANY);
    regGrid->CreateGrid(10, 2);
    regGrid->SetColLabelValue(0, "Reg");
    regGrid->SetColLabelValue(1, "Value");
    const char* regs[] = {"A","B","CC","DP","X","Y","S","U","PC","EA"};
    for(int i=0;i<10;i++) {
        regGrid->SetCellValue(i,0,regs[i]);
        regGrid->SetReadOnly(i,0);
    }
    notebook->AddPage(regGrid, "Registers");

    SetSizer(new wxBoxSizer(wxVERTICAL));
    GetSizer()->Add(notebook, 1, wxEXPAND);
}

void MainView::displayHex(const std::string& hex) {
    hexView->SetText(hex);
}

void MainView::displayRegisters(const mc6309::Registers& r) {
    regGrid->SetCellValue(0,1, wxString::Format("0x%02X", r.A));
    regGrid->SetCellValue(1,1, wxString::Format("0x%02X", r.B));
    regGrid->SetCellValue(2,1, wxString::Format("0x%02X", r.CC));
    regGrid->SetCellValue(3,1, wxString::Format("0x%02X", r.DP));
    regGrid->SetCellValue(4,1, wxString::Format("0x%04X", r.X));
    regGrid->SetCellValue(5,1, wxString::Format("0x%04X", r.Y));
    regGrid->SetCellValue(6,1, wxString::Format("0x%04X", r.S));
    regGrid->SetCellValue(7,1, wxString::Format("0x%04X", r.U));
    regGrid->SetCellValue(8,1, wxString::Format("0x%04X", r.PC));
    regGrid->SetCellValue(9,1, wxString::Format("0x%04X", r.EA));
}

void MainView::bind(wxEvtHandler* h, int id, wxEventFunction f) {
    Bind(wxEVT_MENU, f, h, id);
}

int MainView::getMenuIdOpen()  const { return wxID_OPEN; }
int MainView::getMenuIdStart() const { return wxID_HIGHEST+1; }
int MainView::getMenuIdStop()  const { return wxID_HIGHEST+2; }

