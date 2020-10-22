#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>

#ifdef __linux__
    const std::basic_string<char> configFilePath = "~/.temparatureSensor.cfg";
#elif _WIN32
    const std::basic_string<char> configFilePath;
#endif

// key -> name
// value -> address
std::map<std::string, std::string> loadConfig();

int main(int argc, char** argv)
{
    std::cout << "\x1b[?25l";

    auto sensorConfig = loadConfig();
    std::map<std::string, float> sensorData;
    char tempMode = 'C';

    std::thread displayThread([&](){
        std::cout << "\x1b[H";

        for(auto e : sensorData) {
            float temp;

            switch (tempMode)
            {
            case 'C':
                temp = e.second + 273;
                break;
            case 'K':
                temp = e.second;
                break;
            case 'F':
                temp = e.second * 9.f/5.f - 459.67;
            default:
                break;
            }

            std::cout << e.first << ":\t\t" << std::setprecision(2) << temp << ' ' << tempMode << "\n";
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });

    std::thread updateDataThread([&](){
#ifdef __linux__
        for(auto e : sensorConfig) {
            
        }
#elif _WIN32

#endif

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    });

    return 0;
}

std::map<std::string, std::string> loadConfig() {
    std::map<std::string, std::string> temp;
    std::ifstream file(configFilePath);
    std::string key, value;

    if(file.good() && file.is_open())
        while(file >> key >> value) {
            temp[key] = value;
        }

    return temp;
}