#include <fstream>
#include <iostream>
#include <Windows.h>
#include "json.hpp"

using json = nlohmann::json;
using string = std::string;

int main() {
    // Check if test.json exists in the current executable directory
    std::ifstream file("config.json");
    if (!file.good()) {
        file.close();
        std::cout << "Generating config file" << std::endl;
        // Create a JSON file within the executable directory
        std::ofstream o("config.json");
        o << R"({"processName":"Firefox.exe"})";
        o.close();
    } else {
        // Send a message to the console saying that the config file exists
        std::cout << "Config file exists" << std::endl;
    }
    file.close();


    std::ifstream fJson("config.json");
    std::stringstream buffer;
    buffer << fJson.rdbuf();
    auto json = nlohmann::json::parse(buffer.str());
    // Get process name from the JSON file
    string processName = json["processName"];


    std::cout << "Application running. Press shift + d to close " + processName + " automatically." << std::endl;
    bool pressedD = false;
    bool pressedShift = false;

    while (true) {
        // Check if shift is currently pressed
        if (GetAsyncKeyState(VK_SHIFT) & 0x8000) pressedShift = true;
        else pressedShift = false;
        // Check if b is currently pressed
        if (GetKeyState('D') & 0x8000) pressedD = true;
        else pressedD = false;

        if (pressedD && pressedShift) {
            std::cout << "Killing application" << std::endl;
            string command = "taskkill /f /t /im " + processName;
            system(command.c_str());
            std::cout << "Successfully killed " + processName << std::endl;
            pressedD = false;
            pressedShift = false;
        }
    }

    return 0;
}
