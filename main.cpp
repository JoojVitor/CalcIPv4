#include <iostream>
#include <string>
#include <sstream>
#include <math.h>

using namespace std;

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

int main() {
    string ip_address, subnet_mask;
    cout << "Enter IP address (e.g. 192.168.1.1): ";
    cin >> ip_address;
    cout << "Enter subnet mask (e.g. 255.255.255.0): ";
    cin >> subnet_mask;

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
    int num_subnets = pow(2, num_bits);

    // Print number of subnets
    cout << "Number of subnets: " << num_subnets << endl;

    return 0;
}
