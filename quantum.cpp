/** Archivo: quantum.cpp
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
#include "quantum.hpp"
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

Quantum::Quantum() {

}

vector<vd> Quantum::quantize(vector<vd> image, int thresh) {
	rep(i, sz(image)) rep(j, sz(image[i])) {
		if (fabs(image[i][j]) < 4.0 * thresh){
			image[i][j] = (int) trunc(image[i][j]);
			if (fabs(image[i][j]) < thresh) image[i][j] = 0;
		}
	}
	return image;
}
