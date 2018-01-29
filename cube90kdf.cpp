#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include "cube90.cpp"

int iterations = 10;
int keylen = 32;

using namespace std;

void usage() {
    cout << "Cube90KDF - A Key Derivation Function based on the Cube90 Cipher\n";
    cout << "---------------------------------------------------------\n";
    cout << "Usage: cube90kdf <password> <optional key length> <optional number of iterations" << "\n";
}

int main(int argc, char** argv) {
    string key, digest;
    unsigned char b;
    if (argc < 2) {
        usage();
	exit(EXIT_FAILURE);
    }
    key = argv[1];
    if (argc >= 3) {
        keylen = atoi(argv[2]);
    }
    if (argc  == 4) {
        iterations = atoi(argv[3]);
    }
    int i;
    CubeKDF kdf;
    key = kdf.genkey(key, keylen, iterations);
    for (unsigned char c: key) {
        cout << c;
    }
    cout << "\n";
    return 0;
}
