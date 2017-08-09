/** Archivo: huffman.cpp
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
#include "huffman.hpp"
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

Huffman::Huffman(vector<double> data, vector<unsigned int> proba) {
	int n = sz(data);
	//cout << "Se comprime lgo de " << n << endl;
	graph.resize(2 * n - 1); // PROPIEDAD: arbol de n hojas tiene 2n-1 aristas
	
	priority_queue<pair<double, int>, vector<pair<double, int> >, greater<pair<double, int> > > pq;
	rep(i, n) pq.push(pair<double, int>(proba[i], i));
	int ava = n;
	
	while(sz(pq) > 1) {
		pair<double, int> mini = pq.top(); pq.pop();
		pair<double, int> mini2 = pq.top(); pq.pop();
		
		int a = mini.second, a2 = mini2.second;
		double b = mini.first, b2 = mini2.first;
		
		//cout << "Nodos " << a << "," << a2 << "; probas " << b << " " << b2 << "; conectan con " << ava << endl;
		
		(graph[ava]).pb(pair<int, double>(a, b + b2));
		
		(graph[ava]).pb(pair<int, double>(a2, b + b2));
		
		pq.push(pair<double, int>(b + b2, ava));
		ava++;
	}

	queue<int> q;
	q.push(2 * n - 2);
	vector<string> repres;
	rep(i, 2 * n - 1) repres.pb("");
	while(!q.empty()) {
		int t = q.front();
		q.pop();
		if (sz(graph[t]) == 2) {
			repres[(graph[t][0]).first] = repres[t] + '0';
			q.push((graph[t][0]).first);
			repres[(graph[t][1]).first] = repres[t] + '1';
			q.push((graph[t][1]).first);
		}
	}
	
	rep(i, n) dt.insert(mp(data[i], repres[i]));
	rep(i, n) revers.insert(mp(repres[i], data[i]));
}

string Huffman::write_char(double dat) {
	return dt[dat];
}

vd Huffman::read_bits(string input) {
	vd output;
	string conv = "";
	rep(i, sz(input)) {
		bitset<8> bin_x(input[i]);
		conv += bin_x.to_string();
	}
	
	string buf = "";
	rep(i, sz(conv)) {
		buf += conv[i];
		if (revers.find(buf) != revers.end()) {
			//cout << revers[buf] << " " << buf << endl;
			output.pb(revers[buf]);
			buf = "";
		}
	}

	return output;
}
