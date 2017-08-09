/** Archivo: parser.hpp
 *  Autor: Cruz, Kouichi Julián Andrés
 */
#ifndef PARSER_H
#define PARSER_H

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

class Parser {
	public:
		Parser();
		void parse_image(string file, int &rows, int &cols, int &type, int &bands);
		vector<vd> parse_band(string file, int bandn, int &rows, int &cols, int lvl);
		//vector<vector<vd> > parse_image(string file, int &rows, int &cols, int &type);
		void create_image(string file, vector<vector<vd> > image, int rows, int cols, int type);
		void extend_band(vector<vd> &band, int &rows, int &cols, int lvl);
		void get_metadata(string line, ll &ct0, ll &ct1, ll &ct2, ll &ct3, ll &quantindex, ll &szdata, ll &szproba);
};

#endif
