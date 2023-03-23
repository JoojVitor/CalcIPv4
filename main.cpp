#include <iostream>
#include <string>
#include <sstream>
#include <math.h>
#include <regex>
#include <bitset>
#include <vector>

using namespace std;

bool validadeIPInput(string input) {
    regex rgx("^(0?[0-9]{1,2}|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.(0?[0-9]{1,2}|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}$");

    if (regex_match(input, rgx)) {
        return true;
    }
    return false;
}

int binaryToDecimal(string binary) {
    int result = 0;
    for (int i = 0; i < binary.length(); i++) {
        result += (binary[i] - '0') * pow(2, binary.length() - 1 - i);
    }
    return result;
}

string intToBinary(int n) {
    string binary = "";
    for(int i = 0; i < 8; i++) {
        if(n % 2 == 0) {
            binary = "0" + binary;
        } else {
            binary = "1" + binary;
        }
        n /= 2;
    }
    return binary;
}

void cidr_to_ipv4(int cidr) {
    vector<int> result;

    for (int i = 0; i < 4; i++) {
        if (cidr >= 8) {
            result.push_back(255);
            cidr -= 8;
        } else if (cidr > 0) {
            string binary = "";
            for (int j = 0; j < cidr; j++) {
                binary += '1';
            }
            for (int j = 0; j < 8 - cidr; j++) {
                binary += '0';
            }
            result.push_back(binaryToDecimal(binary));
            cidr = 0;
        } else {
            result.push_back(0);
        }
    }

    cout << "Endereço IPv4 correspondente: ";
    for (int i = 0; i < 4; i++) {
        cout << result[i];
        if (i < 3) {
            cout << ".";
        }
    }
    cout << endl;
}

string cidrToDecimal(int cidr) {
    int octet1 = 0, octet2 = 0, octet3 = 0, octet4 = 0;
    for (int i = 0; i < cidr; i++) {
        if (i < 8) {
            octet1 |= 1 << (7 - i);
        } else if (i < 16) {
            octet2 |= 1 << (15 - i);
        } else if (i < 24) {
            octet3 |= 1 << (23 - i);
        } else {
            octet4 |= 1 << (31 - i);
        }
    }
    return to_string(octet1) + "." + to_string(octet2) + "." + to_string(octet3) + "." + to_string(octet4);
}

void calcularIP(string ip_address, string subnet_mask) {
    
        // Convert IP address and subnet mask to binary
        stringstream s(ip_address);
        string octet;
        
        int ip[4], mask[4];
        int i = 0;
        while(getline(s, octet, '.')) {
            ip[i] = stoi(octet);
            i++;
        }
        stringstream s2(subnet_mask);
        i = 0;
        while(getline(s2, octet, '.')) {
            mask[i] = stoi(octet);
            i++;
        }

        // Calculate network address
        int network[4];
        for(int i = 0; i < 4; i++) {
            network[i] = ip[i] & mask[i];
        }

        // Print network address
        cout << "Network address: ";
        for(int i = 0; i < 4; i++) {
            cout << network[i];
            if(i < 3) {
                cout << ".";
            }
        }
        cout << endl;

        // Calculate broadcast address
        int broadcast[4];
        for(int i = 0; i < 4; i++) {
            broadcast[i] = ip[i] | (~mask[i] & 0xff);
        }

        // Print broadcast address
        cout << "Broadcast address: ";
        for(int i = 0; i < 4; i++) {
            cout << broadcast[i];
            if(i < 3) {
                cout << ".";
            }
        }
        cout << endl;

        // Calculate number of hosts
        int num_hosts = 1;
        for(int i = 0; i < 4; i++) {
            if(mask[i] != 255) {
                num_hosts *= (256 - mask[i]);
            }
        }
        num_hosts -= 2; // subtract network address and broadcast address

        // Print number of hosts
        cout << "Number of hosts: " << num_hosts << endl;

        // Calculate subnets
        int num_bits = 0;
        for(int i = 0; i < 4; i++) {
            string binary = intToBinary(mask[i]);
            for(int j = 0; j < 8; j++) {
                if(binary[j] == '0') {
                    num_bits++;
                }
            }
        }
}

string getIPAddressClass(string ipAddress) {
    // Obtem a primeira parte do endereço IP (os primeiros 8 bits)
    int firstOctet = stoi(ipAddress.substr(0, ipAddress.find('.')));

    // Verifica a classe do endereço IP
    if (firstOctet >= 1 && firstOctet <= 126) {
        return "Classe A";
    } else if (firstOctet >= 128 && firstOctet <= 191) {
        return "Classe B";
    } else if (firstOctet >= 192 && firstOctet <= 223) {
        return "Classe C";
    } else if (firstOctet >= 224 && firstOctet <= 239) {
        return "Classe D";
    } else if (firstOctet >= 240 && firstOctet <= 255) {
        return "Classe E";
    } else {
        return "Endereço IP inválido";
    }
}

void calcularClasseIP() {
    string ipAddress;
    cout << "Digite o endereço IPv4: ";
    cin >> ipAddress;

    string ipAddressClass = getIPAddressClass(ipAddress);
    cout << "O endereço " << ipAddress << " pertence à " << ipAddressClass << endl;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");

    int opcao;

    cout<< "Calculadora IPv4\n\n1 - Calcular IP com classes";
    cin>> opcao;

    switch (opcao)
    {
    case 1:
        calcularClasseIP();
        break;
    
    default:
        break;
    }

    return 0;
}
