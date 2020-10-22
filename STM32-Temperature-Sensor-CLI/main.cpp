#include <iostream>
#include <fstream>
#include <thread>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>
#include <cstdlib>

#include "boost/asio.hpp"
#include "serial_device.h"

using namespace boost;

#ifdef __linux__
    const std::basic_string<char> configFilePath = "~/.temparatureSensor.cfg";
#elif _WIN32
    const std::basic_string<char> configFilePath = std::string(getenv("APPDATA")) + "\\Temperature-Sensor-CLI\\temparatureSensor.cfg";
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
        for(auto e : sensorConfig) {
            asio::io_service io;
            asio::serial_port port(io);

            port.open(e.first);
            port.set_option(asio::serial_port::baud_rate(115200));

            std::string response;

            serial_device serialDevice(port);
            iostreams::stream<serial_device> serial;

            serial >> response;
        }
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