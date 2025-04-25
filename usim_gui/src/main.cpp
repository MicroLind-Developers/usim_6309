// === File: main.cpp ===
// #include <argparse/argparse.hpp>
#include <wx/wx.h>
#include "CPUModel.h"
#include "MainView.h"
#include "MainController.h"

class MyApp : public wxApp {
public:
    bool OnInit() override {
        // argparse::ArgumentParser prog("usim-gui");
        // prog.add_argument("-h", "--hexfile").help("Path to hex file");
        // try {
        //     prog.parse_args(argc, argv);
        //     if (prog.present("--hexfile"))
        //         
        // } catch(...) {}
        initial_hex = "../build/serial.hex"; // Default hex file path
        CPUModel model(initial_hex);
        MainView view;
        MainController ctrl(model, view);
        view.Show();
        return true;
    }

private:
    std::string initial_hex;
};

wxIMPLEMENT_APP(MyApp);
