/** Archivo: util.cpp
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
#include <cassert>
#include <cstdint>
#include "util.hpp"
#include "huffman.hpp"
#include "daub.hpp"
using namespace std;

#define rep(i,a) for(int i=0; i<a;i++)
#define repd(i,a) for(int i=a - 1; i>= 0;i--)
#define forn(i,a,b) for(int i=a;i<b;i++)
#define ford(i,a,b) for(int i=a; i>=b; i--)
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

ll ex(ll a, ll b) {
	if (b == 0) return 1;
	if (b % 2) return a * ex(a, b - 1);
	ll c = ex(a, b / 2);
	return c * c;
}

string DecToBin(int number)
{
    if ( number == 0 ) return "0";
    if ( number == 1 ) return "1";

    if ( number % 2 == 0 )
        return DecToBin(number / 2) + "0";
    else
        return DecToBin(number / 2) + "1";
}

Util::Util() {
}

/* Huffman */
vector<string> Util::huffman(vector<double> input, vector<double> data, vector<unsigned int> proba) {
	// PRE: data y proba tienen el mismo sitio
	assert(sz(data) == sz(proba));

	Huffman huf(data, proba);

	vector<string> output;
	rep(i, sz(input)) output.pb(huf.write_char(input[i]));

	return output;
}

vd Util::ihuffman(string input, vector<double> data, vector<unsigned int> proba) {
	Huffman huf(data, proba);

	vd output = huf.read_bits(input);
	
	return output;
}

// Encoding


string Util::encode_data(double dt, double thresh) {
	string ret;
	if (fabs(dt) < thresh) {
		bitset<32> bits((int) dt);
		ret = bits.to_string();
	} else if (fabs(dt) < 4.0 * thresh) {
		int dint = (int) trunc(dt);
		bitset<32> bits(dint);
		ret = bits.to_string();
	} else {
		float tmp = (float) dt;
		/* Idea: un float es:
		 * 1 bit de signo
		 * 8 bits de exponente
		 * 23 de mantisa
		 * Para representar un valor (1,m)*2^ex
		 * La idea es truncar la mantisa, para que nos quede un numero de 16 bits en total (es decir, se trunca la mantisa a 7 bits)
		 * Esto permite representar numeros grandes sin aumentar la cantidad de bits, a costa de presición */
		union FloatInt
		{
		     float f;
		     unsigned int i;
		};
		FloatInt conv;
		conv.f = tmp;
		bitset<32> second((conv.i));
		ret = second.to_string();
	}
	return ret;
}

string Util::encode_proba(int dt) {
	string ret = DecToBin(dt);
	while (sz(ret) < 32) ret = '0' + ret;
	return ret;
}

void Util::encode_everything(double dt, double thresh, int proba, string &outd, string &outp) {
	outd = encode_data(dt, thresh);
	outp = encode_proba(proba);
}

/* DWT */

vector<vd> Util::dwt(vector<vd> image, int rows, int cols, int lvl) {
	vd img;
	
	rep(r, rows) rep(c, cols) img.pb(image[r][c]);
	
	vi s;
	s.pb(rows);
	s.pb(cols);
	
	//Daubs wlet(4);
	Daub4 wlet;
	
	wtn(img, s, 1, wlet, lvl);
	
	vector<vd> trans;
	rep(i, rows) {
		vd row;
		rep(j, cols) row.pb(img[i * cols + j]);
		trans.pb(row);
	}
	
	return trans;
}

vector<vd> Util::idwt(vector<vd> image, int rows, int cols, int lvl) {
	vd img;
	
	rep(r, rows) rep(c, cols) img.pb(image[r][c]);
	
	vi s;
	s.pb(rows);
	s.pb(cols);
	
	//Daubs wlet(4);
	Daub4 wlet;
	
	wtn(img, s, -1, wlet, lvl);
	
	vector<vd> trans;
	rep(i, rows) {
		vd row;
		rep(j, cols) row.pb(img[i * cols + j]);
		trans.pb(row);
	}
	
	return trans;
}
