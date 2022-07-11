#include <fstream>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <cstdint>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// from: https://stackoverflow.com/a/56107709
uint64_t timeSinceEpochMillisec() {
    using namespace chrono;
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}


int main() {
    // Check if config.json exists in the current executable directory
    ifstream file("config.json");
    if (!file.good()) {
        file.close();
        cout << "Generating config file" << endl;
        // Create the config file within the executable directory
        ofstream o("config.json");
        o << R"({"processName":"Firefox.exe", "hidden": false})";
        o.close();
    } else {
        // Send a message to the console saying that the config file exists
        cout << "Config file exists" << endl;
    }
    file.close();

    ifstream fJson("config.json");
    stringstream buffer;
    buffer << fJson.rdbuf();
    auto json = nlohmann::json::parse(buffer.str());
    // Get process name from the config file
    string processName = json["processName"];
    bool hide = json["hidden"];
    bool hidden = false;
    // Set timestamp
    uint64_t timestamp;

    if(hide) {
        hidden = true;
        // Hide the console
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }

    // Get processes by a specific name


    cout << "Application running. Press esc + d to close " + processName + " automatically." << endl;
    bool pressedD = false;
    bool pressedEsc = false;
    // Used for showing the console
    bool pressedS = false;

    while (true) {
        // Check if shift is currently pressed
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) pressedEsc = true;
        else pressedEsc = false;
        // Check if b is currently pressed
        if (GetKeyState('D') & 0x8000) pressedD = true;
        else pressedD = false;

        if (GetKeyState('S') & 0x8000) pressedS = true;
        else pressedS = false;

        if (pressedD && pressedEsc) {
            cout << "Killing application" << endl;
            string command = "taskkill /f /t /im " + processName;
            system(command.c_str());
            cout << "Successfully killed " + processName << endl;
            pressedD = false;
            pressedEsc = false;
        } else if(pressedS && pressedEsc) {
            // Set current timestamp
            uint64_t currentTimestamp = timeSinceEpochMillisec();
            // fyi: all of this timestamp stuff was written by copilot soooo

            // If the timestamp is not set, set it
            if (timestamp == 0) timestamp = currentTimestamp;
            else {
                // If the timestamp is set, check if it has been more than half a second since the last timestamp
                if (currentTimestamp - timestamp > 500) {
                    // If it has been more than half a second, set the timestamp to the current timestamp
                    timestamp = currentTimestamp;
                    // Do stuff
                    if(hidden) {
                        cout << "Showing console" << endl;
                        ShowWindow(GetConsoleWindow(), SW_SHOW);
                        hidden = false;
                    } else {
                        cout << "Hiding console" << endl;
                        ShowWindow(GetConsoleWindow(), SW_HIDE);
                        hidden = true;
                    }
                    pressedS = false;
                    pressedEsc = false;
                }
            }
        }
    }

    return 0;
}
