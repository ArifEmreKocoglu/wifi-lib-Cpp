#ifndef WIFILIB_H
# define WIFILIB_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>
#include <cstdio>
#include <string>


bool is_wifi_enabled();
bool toggle_wifi(bool enable);
std::vector<std::string> scan_wifi_networks();
std::string get_current_connection();
std::map<std::string, std::string> get_speed_values();
std::map<std::string, std::string> get_network_details();
bool connect_to_network(const std::string& ssid, const std::string& password);
bool disconnect_from_network();
int get_network_id();
int main(int argc, char* argv[]);

#endif