/** Archivo: main.cpp
 *  Autor: Cruz, Kouichi Julián Andrés
 */
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <list>
#include <stack>
#include <bitset>
#include <map>
#include <set>
#include <cmath>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <cstring>
#include <fstream>
#include <string>
#include <cassert>
#include "compression.hpp"
#include "decompression.hpp"
using namespace std;

#define rep(i,a) for(int i=0; i<a;i++)
#define repd(i,a) for(int i=a - 1; i>= 0;i--)
#define forn(i,a,b) for(int i=a;i<b;i++)
#define ford(i,a,b) for(int i=a; i>=b; i--)
#define repl(i,a) for(long long unsigned i=0; i<((long long unsigned) a);i++)
#define repdl(i,a) for(long long unsigned i=((long long unsigned) a) - 1; i >= 0;i--)
#define fornl(i,a,b) for(int i=((long long unsigned) a);i<((long long unsigned) b);i++)
#define fornld(i,a,b) for(int i=((long long unsigned) a);i>= ((long long unsigned) b);i--)
#define mp make_pair
#define ll long long unsigned
#define sz(x) (x).size()
#define pb push_back
#define endl '\n'
#define vi vector<int>
#define ii pair<int, int>
#define vd vector<double>

template <typename T> string NumberToString ( T Number ) {
	ostringstream ss;
	ss << Number;
	return ss.str();
}

int main(int argc, char** argv) {
	//ios_base::sync_with_stdio(false);
	//cin.tie(NULL);

	if (argc != 4 && argc != 5) {
		cout << "Uso:" << endl;
		cout << "./main path_in path_out compress [levels]" << endl;
		cout << "./main path_in path_out decompress" << endl;
		return 1;
	}
	string path = argv[1], pathout = argv[2], mode = argv[3];

	string slvl;
	int lvl = 3;
	if (argc == 5) {
		slvl = argv[4];
		try {
			lvl = stoi(slvl);
			if (lvl < 0) {
				cout << "Level must be non-negative" << endl;
				return 1;
			}
			if (lvl == 0) lvl = 1;
		} catch (int e) {
			cout << "Uso:" << endl;
			cout << "./main path_in path_out compress [levels]" << endl;
			cout << "./main path_in path_out decompress" << endl;
			return 1;
		}
	}

	if (mode.compare("compress") == 0) compress(path, pathout, lvl);
	else if (mode.compare("decompress") == 0) decompress(path, pathout);
	else {
		cout << "Uso:" << endl;
		cout << "./main path_in path_out compress [levels]" << endl;
		cout << "./main path_in path_out decompress" << endl;
		return 1;
	}

	return 0;
}

