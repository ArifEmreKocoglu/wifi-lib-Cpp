#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>

bool is_wifi_enabled() {
    char buffer[128];
    FILE* fp = popen("nmcli radio wifi", "r");
    if (fp) {
        fgets(buffer, sizeof(buffer), fp);
        pclose(fp);
        std::string status(buffer);
        return status.find("enabled") != std::string::npos;
    }
    return false;
}

bool toggle_wifi(bool enable) {
    if (enable) {
        system("nmcli radio wifi on");
        return true;
    } else {
        system("nmcli radio wifi off");
        return false;
    }
}


std::vector<std::string> scan_wifi_networks() {
    std::vector<std::string> networks;
    char line[256];
    FILE *fp = popen("iwlist wlxf81a6723722a scan | grep ESSID", "r");
    
    if (fp) {
        std::cout << "test"<<std::endl;
        while (fgets(line, sizeof(line), fp)) {
            std::string str(line);
            size_t start = str.find("\"");
            size_t end = str.rfind("\"");
            if (start != std::string::npos && end != std::string::npos) {
                networks.push_back(str.substr(start + 1, end - start - 1));
            }
        }
        pclose(fp);
    }
    return networks;
}

std::string get_current_connection() {
    char line[256];
    std::string ssid = "";
    FILE *fp = popen("iwgetid -r", "r");
    if (fp) {
        if (fgets(line, sizeof(line), fp)) {
            ssid = line;
            // Yeni satır karakterini kaldır
            if (!ssid.empty() && ssid[ssid.size() - 1] == '\n') {
                ssid.erase(ssid.size() - 1);
            }
        }
        pclose(fp);
    }

    if (ssid.empty()) {
        std::cout << "şuan bir ağa bağlı değilsiniz" << std::endl; 
        return "0";
    } else {
        return ssid;
    }
}

std::map<std::string, std::string> get_speed_values() {
    std::map<std::string, std::string> values;
    char line[512];
    
    FILE* fp = popen("speedtest-cli", "r");
    if (!fp) {
        std::cerr << "Hata: Komut çalıştırılamadı." << std::endl;
        return values;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        std::string str_line(line);

        if (str_line.find("Download:") != std::string::npos) {
            size_t start_pos = str_line.find("Download:") + strlen("Download:");
            std::string download_value = str_line.substr(start_pos);
            values["Download"] = download_value;
        } else if (str_line.find("Upload:") != std::string::npos) {
            size_t start_pos = str_line.find("Upload:") + strlen("Upload:");
            std::string upload_value = str_line.substr(start_pos);
            values["Upload"] = upload_value;
        }
    }

    pclose(fp);
    return values;
}


std::map<std::string, std::string> get_network_details() {
    std::map<std::string, std::string> details;

    // IP adresi almak için
    FILE* fp = popen("ip addr show wlxf81a6723722a | grep 'inet ' | awk '{print $2}'", "r");
    char line[256];
    if(fp) {
        if (fgets(line, sizeof(line), fp)) {
            details["IP"] = std::string(line);
        }
        pclose(fp);
    }

    // DNS sunucuları almak için
    fp = popen("cat /etc/resolv.conf | grep nameserver | awk '{print $2}'", "r");
    std::string dns_servers;
    while (fgets(line, sizeof(line), fp)) {
        dns_servers += std::string(line);
    }
    details["DNS"] = dns_servers;
    pclose(fp);

    // Diğer kablosuz bağlantı detayları için
    fp = popen("iwconfig wlxf81a6723722a", "r");
    std::string iwconfig_output;
    while (fgets(line, sizeof(line), fp)) {
        iwconfig_output += std::string(line);
    }
    details["Connection Details"] = iwconfig_output;
    pclose(fp);

    return details;
}



bool connect_to_network(const std::string& ssid, const std::string& password) { // net ismi ve şifresi ile bağlantı sağlanır 
    std::string command = "nmcli dev wifi connect '" + ssid + "' password '" + password + "'";
    int result = system(command.c_str());
    if(result == 1)
        get_current_connection();
    return result == 0; // Eğer komut başarıyla çalıştırıldıysa true döndür, değilse false döndür.
}

bool disconnect_from_network() {
    int result = system("nmcli dev disconnect wlxf81a6723722a");
    if(result == 1)
        get_current_connection();
    return result == 0;
}


int main(int argc, char* argv[]) {
    if(argc > 1) {
        std::string command = argv[1];
        
        if(command == "scan") {
            std::vector<std::string> networks = scan_wifi_networks();
            for (const std::string &net : networks) {
                std::cout << net << std::endl;
            }
        }
        else if(command == "connect" && argc == 4) {
            bool result = connect_to_network(argv[2], argv[3]);
            std::cout << "bağladı mı:" << result << std::endl;
            std::cout << (result ? "Connected" : "Failed to connect") << std::endl;
        }
        else if(command == "disconnect") {
            bool result = disconnect_from_network();
            std::cout << (result ? "Disconnected" : "Failed to disconnect") << std::endl;
        }
        else if(command == "currentnet"){
            std::string nownet = get_current_connection();
            std::cout << "bağlı olunan ağ:" << nownet << std::endl;
        }
        else if(command == "netdetails"){
                 auto details = get_network_details();
                 for (const auto& [key, value] : details) {
                    std::cout << key << ": " << value << std::endl;
                }
        }
        else if(command == "speedtest"){
                std::map<std::string, std::string> speeds = get_speed_values();
                
                if(speeds.find("Download") != speeds.end()) {
                    std::cout << "Download Hızı: " << speeds["Download"] << std::endl;
                }
                
                if(speeds.find("Upload") != speeds.end()) {
                    std::cout << "Upload Hızı: " << speeds["Upload"] << std::endl;
                }
        }
        else if(command == "wifistatus"){
            bool flag = is_wifi_enabled();
            if(flag == true)
                std::cout << "wifi açık" << std::endl;
            else
                std::cout << "wifi kapalı" << std::endl;
        }
        else if(command == "openwifi"){
            bool flag = toggle_wifi(true);
            if(flag == true)
                std::cout << "wifi açıldı" << std::endl;
        }
        else if(command == "closewifi"){
            bool flag = toggle_wifi(false);
            if(flag == false)
                std::cout << "wifi kapandı" << std::endl;
        }
    }
    else {
        std::cout << "Invalid command" << std::endl;
    }
    
    return 0;
}

// int main() {
//     /*  İNTERNET BAĞLANTI TESTİ.
//     bool flag;
//     flag = connect_to_network("a iPhone\xE2\x80\x99u", "arif0707");
//     std::cout << flag << std::endl;
//     */
//     std::vector<std::string> networks = scan_wifi_networks();
//     for (const std::string &net : networks) {
//         std::cout << net << std::endl;
//     }
//     return 0;
// }