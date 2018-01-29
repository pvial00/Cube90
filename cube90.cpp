#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;

class Cube {
    public:
    int size_factor = 3;
    int alphabet_size = 91;
    vector< vector< vector<int> > > state;
    map<int, char> chars;
    map<char,int> chars_rev;
    int start_char = 32;
    void gendict() {
        int a;
        for (a = 0; a < alphabet_size; a++) {
            chars[a] = char(a + start_char);
            chars_rev[char(a + start_char)] = a;
        }
    }

    vector<int> tonums(string data) {
        vector<int> n;
	int i;
	for (i = 0; i < data.length(); i++) {
	    n.push_back(chars_rev[data[i]]);
	}
	return n;
    }

    void gen_cube (int depth, int width, int length) {
        for (int z=0; z < depth; z++) {
            vector< vector<int> > section;
            for (int y=0; y < width; y++) {
                vector<int> alphabet;
                for (int x=0; x < length; x++) {
                    alphabet.push_back(x);
                }
                for (int mod=0; mod < y; mod++) {
    		    int shift;
                    shift = alphabet.at(0);
		    alphabet.erase(alphabet.begin()+0);
                    alphabet.push_back(shift);
                    shift = alphabet.at(2);
                    alphabet.erase(alphabet.begin()+2);
                    alphabet.insert(alphabet.begin()+13,shift);
                }
                section.push_back(alphabet);
            }
            state.push_back(section);
        }
    }

    void key_cube (string key) {
        int z, x, y, k, kval;
        int shuffle;
        int key_sub;
        int sized_pos;
        for (z=0; z < state.size(); z++) {
            for (int k: key) {
                kval = chars_rev[k];
                for (x=0; x < state[z].size(); x++) {
                    key_sub = state[z][x].at(kval);
		    state[z][x].erase(state[z][x].begin()+kval);
                    state[z][x].push_back(key_sub);
                    for (y=0; y < kval; y++) {
                        if (y % 2 == 0) {
                            shuffle = state[z][x].at(0);
                            state[z][x].erase(state[z][x].begin()+0);
                            state[z][x].push_back(shuffle);
                            shuffle = state[z][x].at(2);
                            state[z][x].erase(state[z][x].begin()+2);
                            state[z][x].insert(state[z][x].begin()+13,shuffle);
                        }
                    }
                }
            }
        }
        vector< vector<int> > section;
        for (int k: key) {
            kval = chars_rev[k];
	    sized_pos = k % size_factor;
	    for (y=0; y < kval; y++) {
		    section = state.at(sized_pos);
		    state.erase(state.begin()+sized_pos);
		    state.push_back(section);
            }
        }
    }

    vector<int> key_scheduler (vector<int> key) {
        int x;
        int sized_pos;
        int sub;
        vector< vector<int> > section;
        vector<int> sub_alpha;
        vector<int> sub_key;
        for (int k: key) {
	    sized_pos = k % size_factor;
            sub = state[sized_pos][sized_pos][k];
	    state[sized_pos][sized_pos].erase(state[sized_pos][sized_pos].begin()+k);
            state[sized_pos][sized_pos].push_back(sub); 
	    sub_key.push_back(sub);
        }
        return sub_key;
    }

    void morph_cube (int counter, vector<int> k) {
        int mod_value;
        int shift;
        int z;
        int y;
        int ke;
        vector< vector<int> >  section_shift;
        mod_value = counter % alphabet_size;
        for (z=0; z < state.size(); z++) {
	    for (int key_element : k) {
                for (y=0; y < state[z].size(); y++) {
		    swap(state[z][y][mod_value], state[z][y][key_element]);
		    ke = key_element;
                }
	    }
            shift = ke % size_factor;
            section_shift = state.at(shift);
	    state.erase(state.begin()+shift);
	    state.push_back(section_shift);
        }
    }

    string encrypt (string data, string key, string nonce = string()) {
        vector<int> sub_key;
        sub_key = tonums(key);
	gendict();
        gen_cube(size_factor, size_factor, alphabet_size);
        key_cube(key);
        key_cube(nonce);
        int ctr = 0;
        int sub;
        int sub_pos;
        int shift;
        int z;
        int y;
        for (unsigned char byte: data) {
            sub = chars_rev[byte];
	    for (z=0; z < state.size(); z++) {
	        for (y=0; y < state[z].size(); y++) {
                    sub_pos = sub;
		    sub = state[z][y].at(sub_pos);
		    shift = state[z][y].at(0);
		    state[z][y].erase(state[z][y].begin()+0);
		    state[z][y].push_back(shift);
	        }
	    }
	    sub_key = key_scheduler(sub_key);
	    morph_cube(ctr, sub_key);
	    data[ctr] = chars[sub];
            ctr++;
        }
        return data;
    }

   string decrypt (string data, string key, string nonce = string()) {
        vector<int> sub_key;
        sub_key = tonums(key);
	gendict();
        gen_cube(size_factor, size_factor, alphabet_size);
        key_cube(key);
        key_cube(nonce);
        int ctr = 0;
        int sub;
        int sub_pos;
        int shift;
        int z;
        int y;
        for (unsigned char byte: data) {
            sub = chars_rev[byte];
	    for (z=state.size(); z--> 0;) {
	        for (y=state[z].size(); y --> 0;) {
		    sub = find(state[z][y].begin(), state[z][y].end(), sub) - state[z][y].begin();
		    shift = state[z][y].at(0);
		    state[z][y].erase(state[z][y].begin()+0);
		    state[z][y].push_back(shift);
	        }
	    }
	    sub_key = key_scheduler(sub_key);
	    morph_cube(ctr, sub_key);
	    data[ctr] = chars[sub];
            ctr++;
        }
        return data;
    }
};

class CubeH {
    public:
    string digest (string data, string key = string(), int length = 32) {
        string d;
        int x;
        string iv;
        if (key.empty() == true) {
            for (x = 0; x < length; x++) {
                key.push_back(char(32));
            }   
        }
        Cube cube;
        d = cube.encrypt(key, data, key);
        return d;
    }
};

class CubeMAC {
    public:
    string mac (string data, string key, int length) {
        string m;
	CubeH hash;
	m = hash.digest(data, key, length);
	return m;
    }
};

class CubeKDF {
    public:
    string genkey (string key, int length, int iterations) {
        string d;
        int x;
        string iv;
        string h;
        CubeH hash;
        h = key;
	key = hash.digest(key, string(), length);
        for (x = 0; x < iterations; x++) {
           h = hash.digest(h, key, length);
        }
        return h;
    }
};

class CubeRandom {
    public:
    string random (int length) {
	int seedlength = 16;
        char aseed[seedlength];
        string iv, bytes, seed;
        int x, s;
        for (x = 0; x < length; x++) {
            iv.push_back(char(32));
        }
        ifstream urandom("/dev/urandom", ios::in|ios::binary);
        urandom.read(aseed, seedlength);
        urandom.close();
        string tmp(aseed);
        for (x = 0; x < seedlength; x++) {
            s = tmp[x] % (91 - 32 + 1) + 32;
            seed.push_back(char(s));
        }
	Cube cube;
        bytes = cube.encrypt(iv, seed);
        return bytes;
    }
};

