#include <iostream>
#include <locale.h>
#include <string>
#include <sstream>
#include <math.h>
#include <regex>
#include <bitset>
#include <vector>

using namespace std;

vector<string> split(string s, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(s);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool is_valid_ip(string input) {
    regex rgx("^(0?[0-9]{1,2}|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])(\\.(0?[0-9]{1,2}|1[0-9]{1,2}|2[0-4][0-9]|25[0-5])){3}$");

    if (regex_match(input, rgx)) {
        return true;
    }
    return false;
}

void msg_special_address() {
    cout<< "� importante notar que o endere�o IP 0.0.0.0 n�o � um endere�o IP que possa ser atribu�do a um dispositivo em uma rede. Ele � usado apenas como um marcador especial para fins de roteamento e gerenciamento de rede.";
}

string cidr_to_binary(int cidr) {
    bitset<32> binary_mask;
    for (int i = 0; i < cidr; i++) {
        binary_mask.set(31 - i, 1);
    }

    return binary_mask.to_string();
}

string address_to_binary(string str) {
    vector<string> octets = split(str, '.');
    string binary_str = "";

    for (string octet : octets) {
        int value = stoi(octet);
        binary_str += bitset<8>(value).to_string();
    }

    return binary_str;
}

string binary_to_address(string binary) {
    string binary_str = "";
    
    for (int i = 0, j = 1; i < binary.size(); i++, j++) {
        binary_str += binary[i];
        if (j >= 8) {
            j = 0;
            binary_str += " ";
        }
    }
    
    vector<string> binary_octets = split(binary_str, ' ');
    string ip_address = "";

    for (string binary_octet : binary_octets) {
        int value = bitset<8>(binary_octet).to_ulong();
        ip_address += to_string(value) + ".";
    }

    ip_address.pop_back();
    return ip_address;
}

int get_cidr_from_binary_mask(string mask_binary) {

    // Converte o endere�o de IP em um objeto bitset
    bitset<32> ip_bits(mask_binary);

    // Conta quantos bits "1" h� na sequ�ncia do endere�o de IP
    int count_ones = ip_bits.count();

    return count_ones;
}

string calc_network_address(string ip_binary, string subnet_mask_binary) {
    string network_address_binary = "";

    for (int i = 0, j = 1; i < 32; i++, j++) {
        if (subnet_mask_binary[i] == '1') {
            network_address_binary += ip_binary[i];
        } else {
            network_address_binary += '0';
        }
    }

    return network_address_binary; 
}

string calc_broadcast(string ip_address_binary, string mask_binary) {
    string broadcast_bin = "";
    for (int i = 0; i < 32; i++) {
        if (mask_binary[i] == '0') {
            broadcast_bin += '1';
        } else {
            broadcast_bin += ip_address_binary[i];
        }
    }
    
    return binary_to_address(broadcast_bin);
}

void print_report(string ip_address, string ip_class, string network_mask, int cidr, int num_hosts, string broadcast, string network_address) {
    string broadcast_str = "";
    string network_address_str = "";

    vector<string> broadcast_arr = split(broadcast, '.');
    vector<string> network_address_arr = split(network_address, '.');

    cout<< "\nEndere�o IP:\t\t\t\t" << ip_address <<endl;

    if (!ip_class.empty()) cout<< "Classe:\t\t\t\t\t" << ip_class <<endl;
    
    cout<< "M�scara decimal:\t\t\t" << network_mask <<endl;
    cout<< "M�scara CIDR:\t\t\t\t/" << cidr <<endl;
    cout<< "N�mero de hosts:\t\t\t" << num_hosts <<endl;
    cout<< "Endere�o de rede:\t\t\t" << network_address <<endl;
    cout<< "Endere�o de broadcast:\t\t\t" << broadcast <<endl;

    network_address_arr[3] = to_string(stoi(network_address_arr[3]) + 1);
    broadcast_arr[3] = to_string(stoi(broadcast_arr[3]) - 1);

    // vector para string
    for (int i = 0; i < broadcast_arr.size(); i++) {
        if (i > 0) {
            broadcast_str += "."; // adicionar um . entre os octetos
        }
        broadcast_str += broadcast_arr[i]; // concatenar o octeto atual no endere�o
    }

    for (int i = 0; i < network_address_arr.size(); i++) {
        if (i > 0) {
            network_address_str += "."; // adicionar um . entre os octetos
        }
        network_address_str += network_address_arr[i]; // concatenar o octeto atual no endere�o
    }

    cout << "Endere�o IP inicial utiliz�vel:\t\t" << network_address_str <<endl;
    cout << "Endere�o IP final utiliz�vel:\t\t" << broadcast_str <<endl;
}

void calc_ip_com_classe(string ip_address) {

    // Obt�m o primeiro octeto do endere�o usando uma implementa��o minha do split e converte para inteiro
    int first_octet = stoi(split(ip_address, '.')[0]);

    string ip_class, network_mask;
    int cidr;

    // Obtem a classe a partir da faixa do primeiro octeto
    if (first_octet >= 1 && first_octet <= 126) {
        ip_class = "Classe A";
        network_mask = "255.0.0.0";
        cidr = 8;
    } else if (first_octet >= 128 && first_octet <= 191) {
        ip_class = "Classe B";
        network_mask = "255.255.0.0";
        cidr = 16;
    } else if (first_octet >= 192 && first_octet <= 223) {
        ip_class = "Classe C";
        network_mask = "255.255.255.0";
        cidr = 24;
    } else if (first_octet >= 224 && first_octet <= 239) {
        ip_class = "Classe D";
        cout<< "Endere�o IP: " << ip_address <<endl;
        cout<< "Classe: " << ip_class <<endl;
        cout<< "Endere�o reservado para multicast" <<endl;
        return;
    } else if (first_octet >= 240 && first_octet <= 255) {
        ip_class = "Classe E";
        cout<< "Endere�o IP: " << ip_address <<endl;
        cout<< "Classe: " << ip_class <<endl;
        cout<< "Endere�o reservado para uso futuro" <<endl;
        return;
    }

    // Converte para bin�rio
    string ip_address_binary = address_to_binary(ip_address);
    string mask_binary = address_to_binary(network_mask);

    // Calcula o endere�o de rede e converte de bin�rio para decimal
    string network_address = binary_to_address(calc_network_address(ip_address_binary, mask_binary));

    // Calcula o n�mero de hosts
    int num_hosts = pow(2, 32 - cidr) - 2;

    // Calcula o endere�o broadcast
    string broadcast = calc_broadcast(ip_address_binary, mask_binary);
        
    print_report(ip_address, ip_class, network_mask, cidr, num_hosts, broadcast, network_address);
}

void calc_ip_sem_classe_cidr(string ip_address, int cidr) {
    string ip_class;

    // Converte para bin�rio
    string ip_address_binary = address_to_binary(ip_address);
    string mask_binary = cidr_to_binary(cidr);
    string network_mask = binary_to_address(mask_binary);

    // Calcula o endere�o de rede e converte de bin�rio para decimal
    string network_address = binary_to_address(calc_network_address(ip_address_binary, mask_binary));

    // Calcula o n�mero de hosts
    int num_hosts = pow(2, 32 - cidr) - 2;

    // Calcula o endere�o broadcast
    string broadcast = calc_broadcast(ip_address_binary, mask_binary);
        
    print_report(ip_address, "", network_mask, cidr, num_hosts, broadcast, network_address);
}

void calc_ip_sem_classe_decimal(string ip_address, string mask) {
    string ip_class, start_ip, final_ip;

    // Converte para bin�rio
    string ip_address_binary = address_to_binary(ip_address);
    string mask_binary = address_to_binary(mask);
    string network_mask = binary_to_address(mask_binary);

    // Calcula o endere�o de rede e converte de bin�rio para decimal
    string network_address = binary_to_address(calc_network_address(ip_address_binary, mask_binary));

    // Obtem CIDR atrav�s da m�scara em bin�rio
    int cidr = get_cidr_from_binary_mask(mask_binary);

    // Calcula o n�mero de hosts
    int num_hosts = pow(2, 32 - cidr) - 2;

    // Calcula o endere�o broadcast
    string broadcast = calc_broadcast(ip_address_binary, mask_binary);
        
    print_report(ip_address, "", network_mask, cidr, num_hosts, broadcast, network_address);
}

int main() {
    setlocale(LC_ALL, "");
    string continuar = "sim";
    string line;

    while (continuar == "sim") {
        int opcao;

        system("cls");

        cout<< "Calculadora IPv4\nRedes de Computadores II\nAluno: Jo�o Vitor dos Santos Couto\n" <<endl;
        cout<< "Op��es\n1 - Calcular IP com classes\n2 - Calcular IP sem classe e nota��o CIDR\n3 - Calcular IP sem classe e nota��o decimal\n\nDigite o n�mero da op��o desejada:" <<endl;
        cin>> opcao;

        system("cls");

        switch (opcao) {
            case 1:
            {
                string input_ip;
                cout<< "Calcular IP com classe\n" <<endl;
                cout<< "Digite um IP v�lido: (padr�o: n.n.n.n)" <<endl;
                cin>> input_ip;

                if (!is_valid_ip(input_ip)) {
                    system("cls");
                    cout<< "Endere�o IP fornecido � inv�lido, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                if (input_ip == "0.0.0.0") {
                    system("cls");
                    msg_special_address();
                    cout<< "\n\npressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }
                
                calc_ip_com_classe(input_ip);
                break;
            }
            case 2:
            {
                string input_ip;
                string input_cidr;

                cout<< "Calcular IP sem classe com nota��o CIDR\n" <<endl;
                cout<< "Digite um IP v�lido: (padr�o: n.n.n.n)" <<endl;
                cin>> input_ip;

                if (!is_valid_ip(input_ip)) {
                    system("cls");
                    cout<< "Endere�o IP fornecido � inv�lido, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                if (input_ip == "0.0.0.0") {
                    system("cls");
                    msg_special_address();
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                cout<< "\nDigite a m�scara na nota��o CIDR: (padr�o: /n)" <<endl;
                cin>> input_cidr;

                regex cidrRegex("^/[0-9]+$");

                if (!regex_match(input_cidr, cidrRegex)) {
                    system("cls");
                    cout<< "O CIDR fornecido � inv�lido, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                int cidr = stoi(input_cidr.substr(1, input_cidr.length()));

                if (cidr < 0 || cidr > 32) {
                    system("cls");
                    cout<< "O CIDR fornecido � inv�lido, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }
                
                calc_ip_sem_classe_cidr(input_ip, cidr);
                break;
            }
            case 3:
            {
                string input_ip;
                string input_mask;

                cout<< "Calcular IP sem classe com nota��o decimal\n" <<endl;
                cout<< "Digite um IP v�lido: (padr�o: n.n.n.n)" <<endl;
                cin>> input_ip;

                if (!is_valid_ip(input_ip)) {
                    system("cls");
                    cout<< "Endere�o IP fornecido � inv�lido, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                if (input_ip == "0.0.0.0") {
                    system("cls");
                    msg_special_address();
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                cout<< "\nDigite a m�scara na nota��o decimal: (padr�o: n.n.n.n)" <<endl;
                cin>> input_mask;

                if (!is_valid_ip(input_mask)) {
                    system("cls");
                    cout<< "A m�scara fornecida � inv�lida, pressione qualquer tecla para tentar novamente." <<endl;
                    cin.ignore();
                    getline(cin, line);
                    continue;
                }

                calc_ip_sem_classe_decimal(input_ip, input_mask);

                break;
            }
            
            default:
                system("cls");
                cout<< "Op��o inv�lida, pressione qualquer tecla para tentar novamente." <<endl;
                cin.ignore();
                getline(cin, line);
                break;
        }

        cout << "\nDeseja calcular outro ip? ('sim' para calcular, qualquer outra coisa pra sair)" <<endl;
        cin >> continuar;
    }

    return 0;
}
