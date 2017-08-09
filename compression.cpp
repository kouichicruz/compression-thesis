/** Archivo: compression.cpp
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
#include "parser.hpp"
#include "quantum.hpp"
#include "util.hpp"
#include "helper.hpp"
#include "compression.hpp"
#include "/usr/local/include/gdal_priv.h"
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


ll expo(ll a, ll b);

void compress(string path, string pathout, int lvl) {
	Parser parser;
	Quantum quantum;
	Util util;
	
	cout << "Se va a comprimir el path: " << path << endl;
	
	int rows, cols;
	int type, bands;
	parser.parse_image(path, rows, cols, type, bands);
	
	cout << "Imagen parseada. Tiene " << rows << " filas y " << cols << " columnas" << endl;
	
	string outstring = path + " " + NumberToString(rows) + " " + NumberToString(cols) + "\n\n\n\n";
	
	ofstream outbin(pathout);
	if (outbin.is_open()) outbin << path << " " << rows << " " << cols << " " << bands << " " << (int) type << " " << lvl << endl << endl;
	else {
		cout << "Problema al crear archivo" << endl;
		exit(1); // i.e. salta el programa
	}
	
	rep(b, bands) {
		cout << "Banda " << b + 1 << endl;
		int ts;
		int rrows = rows, ccols = cols;
		vector<vd> band = parser.parse_band(path, b, rows, cols, lvl);
		
		//if (rows >= 2 * rrows || cols >= 2 * ccols) {
		if (rows >= 2 * rrows && cols >= 2 * ccols) {
			cout << "Too many levels" << endl;
			exit(1);
		}

		vector<vector<double> > trband = util.dwt(band, rows, cols, lvl);
		
		vector<vd> blank;
		band = blank;
		
		cout << "Trnasformada hecha" << endl;
		
		int quantindex = 16;
		trband = quantum.quantize(trband, quantindex);
		
		cout << "Quantizado" << endl;

		vd choclo;
		vector<double> data;
		vector<unsigned int> proba;
		
		choclo = enchoclar(trband, data, proba, rows, cols);
		
#ifdef DEBUG
		cout << sz(data) << " " << sz(proba) << endl;
		rep(i, sz(data)) cout << data[i] << " ";
		cout << endl;
		rep(i, sz(proba)) cout << proba[i] << " ";
#endif
		cout << "Enchoclado" << endl;
		
		vector<string> outhuf = util.huffman(choclo, data, proba);
		
		cout << "Huffmanizado con " << sz(outhuf) << " chars" << endl;
		
		vd blankvd;
		choclo = blankvd;
		
		write_data(outhuf, data, proba, outbin, quantindex, util);
		
		outbin << endl << endl;
		
		cout << "Banda comprimida" << endl;
	}
	
	outbin.close();
}
