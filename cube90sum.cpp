#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include "cube90.cpp"

int hashlen = 32;

using namespace std;

void usage() {
	    cout << "Cube90sum - A slow hash algorithm based on the Cube90 Cipher" << "\n";

	    cout << "---------------------------------------------------------\n";
	    cout << "***  Hash length defaults to 32 characters\n\n";
	    cout << "Usage: cube90sum <input file> <optional hash length in bits>" << "\n";
}

int main(int argc, char** argv) {
    ifstream infile;
    string in, data, digest;
    unsigned char b;
    if (argc < 2) {
        usage();
	exit(EXIT_FAILURE);
    }
    in = argv[1];
    if (argc >= 3) {
        hashlen = atoi(argv[2]);
    }
    int i;
    infile.open(in.c_str(), std::ios::binary);
    infile.seekg(0, ios::end);
    int fsize = infile.tellg();
    infile.close();
    infile.open(in.c_str(), std::ios::binary);
    for (i = 0; i < fsize; i++) {
        b = infile.get();
	data.push_back(b);
    }
    infile.close();
    CubeH hash;
    digest = hash.digest(data, string(), hashlen);
    cout << in << ": ";
    for (unsigned char c: digest) {
        cout << c;
    }
    cout << "\n";
    return 0;
}
