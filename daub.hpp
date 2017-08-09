/** Archivo: daub.hpp
 *  Autor: Cruz, Kouichi Julián Andrés
 */
#ifndef DAUB_H
#define DAUB_H

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
#define vd vector<double>

typedef vector<double> VecDoub, VecDoub_O, VecDoub_IO;
typedef vector<int> VecInt, VecInt_O, VecInt_IO;
typedef const vector<int> VecInt_I;

struct Wavelet {
	virtual void filt(VecDoub_IO &a, const int n, const int isign) = 0;
	virtual void condition(VecDoub_IO &a, const int n, const int isign) {}
};

struct Daubs : Wavelet {
//Structure for initializing and using the DAUBn wavelet filter for any n whose coefficients are
//provided (here n D 4; 12; 20).
	int ncof,ioff,joff;
	VecDoub cc,cr;
	double c4[4]=
	{0.4829629131445341,0.8365163037378079,
	0.2241438680420134,-0.1294095225512604};
	double c12[12]=
	{0.111540743350, 0.494623890398, 0.751133908021,
	0.315250351709,-0.226264693965,-0.129766867567,
	0.097501605587, 0.027522865530,-0.031582039318,
	0.000553842201, 0.004777257511,-0.001077301085};
	double c20[20]=
	{0.026670057901, 0.188176800078, 0.527201188932,
	0.688459039454, 0.281172343661,-0.249846424327,
	-0.195946274377, 0.127369340336, 0.093057364604,
	-0.071394147166,-0.029457536822, 0.033212674059,
	0.003606553567,-0.010733175483, 0.001395351747,
	0.001992405295,-0.000685856695,-0.000116466855,
	0.000093588670,-0.000013264203};
	Daubs(int n) : ncof(n), cc(n), cr(n) {
		int i;
		ioff = joff = -(n >> 1);
		// ioff = -2; joff = -n + 2; // Alternative centering. (Used by Daub4, above.)
		if (n == 4) for (i=0; i<n; i++) cc[i] = c4[i];
		else if (n == 12) for (i=0; i<n; i++) cc[i] = c12[i];
		else if (n == 20) for (i=0; i<n; i++) cc[i] = c20[i];
		else throw("n not yet implemented in Daubs");
			double sig = -1.0;
			for (i=0; i<n; i++) {
			cr[n-1-i]=sig*cc[i];
			sig = -sig;
		}
	}
	void filt(VecDoub_IO &a, const int n, const int isign); // See below.
};

struct Daub4 : Wavelet {
	void filt(VecDoub_IO &a, const int n, const int isign);
};

void wt1(VecDoub_IO &a, const int isign, Wavelet &wlet, int lvl);
void wtn(VecDoub_IO &a, VecInt_I &nn, const int isign, Wavelet &wlet, int lvl);

#endif
