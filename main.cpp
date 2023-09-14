#include "wifilib.hpp"

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