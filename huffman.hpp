/** Archivo: huffman.hpp
 *  Autor: Cruz, Kouichi Julian Andres
 */
#ifndef HUFFMAN_H
#define HUFFMAN_H

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

class Huffman {
	vector<string> repres;
	vector<vector<pair<int, double> > > graph;
	vector<double> data;
	map<double, string> dt;
	map<string, double> revers;
	public:
		Huffman(vector<double> data, vector<unsigned int> proba);
		string write_char(double dat);
		vd read_bits(string line);
	private:
		void representation();
};

#endif
