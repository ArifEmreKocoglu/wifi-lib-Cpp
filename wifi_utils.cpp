#include "wifilib.hpp"

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


int get_network_id() {
    FILE* pipe = popen("wpa_cli -i wlxf81a6723722a add_network", "r");
    if (!pipe) return -1;

    char buffer[128];
    std::string result = "";
    while (!feof(pipe)) {
        if (fgets(buffer, 128, pipe) != nullptr)
            result += buffer;
    }
    pclose(pipe);

    try {
        return std::stoi(result);
    } catch (...) {
        return -1;
    }
}

bool connect_to_network(const std::string& ssid, const std::string& password) {
    system("wpa_cli -i wlxf81a6723722a disconnect");
    
    int network_id = get_network_id();
    if (network_id == -1) {
        return false;
    }

    std::string setup_command = "wpa_cli -i wlxf81a6723722a set_network " + std::to_string(network_id) 
                                + " ssid '\""+ ssid +"\"' && "
                                + "wpa_cli -i wlxf81a6723722a set_network " + std::to_string(network_id)
                                + " psk '\""+ password +"\"' && "
                                + "wpa_cli -i wlxf81a6723722a select_network " + std::to_string(network_id);

    int result = system(setup_command.c_str());

    if(result != 0)
        get_current_connection();
    return result == 0;
}



bool disconnect_from_network() {
    // wpa_cli ile bağlantıyı kes
    int result = system("wpa_cli -i wlxf81a6723722a disconnect");
    if(result == 1)
        get_current_connection();
    return result == 0;
}

// bool connect_to_network(const std::string& ssid, const std::string& password) { // net ismi ve şifresi ile bağlantı sağlanır 
//     std::string command = "nmcli dev wifi connect '" + ssid + "' password '" + password + "'";
//     int result = system(command.c_str());
//     if(result == 1)
//         get_current_connection();
//     return result == 0; // Eğer komut başarıyla çalıştırıldıysa true döndür, değilse false döndür.
// }

// bool disconnect_from_network() {
//     int result = system("nmcli dev disconnect wlxf81a6723722a");
//     if(result == 1)
//         get_current_connection();
//     return result == 0;
// }