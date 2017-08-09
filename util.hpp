/** Archivo: util.hpp
 *  Autor: Cruz, Kouichi Julián Andrés
 */
#ifndef UTIL_H
#define UTIL_H

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


class Util {
	public:
		Util();
		vector<string> huffman(vector<double> input, vector<double> data, vector<unsigned int> proba);
		vd ihuffman(string input, vd data, vector<unsigned int> proba);
		string encode_data(double dt, double thresh);
		string encode_proba(int dt);
		void encode_everything(double dt, double thresh, int proba, string &outd, string &outp);
		vector<vd> dwt(vector<vd> image, int rows, int cols, int lvl);
		vector<vd> idwt(vector<vd> image, int rows, int cols, int lvl);
	private:
		int varocult;
};

#endif
