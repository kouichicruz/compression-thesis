/** Archivo: decompression.cpp
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

void decompress(string path, string pathout) {
	Parser parser;
	Util util;

	cout << "DEBUG: se va a descomprimir el path: " << path << endl;

	ifstream inbin(path);

	vector<vector<vd> > image;
	int rows, cols, bands, type, lvl, nb = 0;
	string line, outpath, srows, scols, sbands, stype, slvl;

	if (inbin.is_open()) {
		cout << "Hey kids wanna buy some magic?" << endl;
		getline(inbin, line);

		istringstream iss(line);
		iss >> outpath >> srows >> scols >> sbands >> stype >> slvl;

		rows = atoi(srows.c_str());
		cols = atoi(scols.c_str());
		bands = atoi(sbands.c_str());
		type = atoi(stype.c_str());
		lvl = atoi(slvl.c_str());
		vd choclo;
		string tmp, tmp2;
		int stat = 0;

		rep(b, bands) {
			getline(inbin, line); // una linea en blanco

			//cout << line << endl;

			getline(inbin, line); // la primera linea tiene cuanto de cada cosa

			//cout << line << endl;

			vector<vd> band;
			vector<ll> coun;
			ll ct0, ct1, ct2, ct3, quantindex, szdata, szproba;

			parser.get_metadata(line, ct0, ct1, ct2, ct3, quantindex, szdata, szproba);

			ll ct0b = ct0; // por hacer C&P
			ll ct1b = ct1 / 8 + (ct1 % 8 != 0);
			ll ct2b = ct2 / 8 + (ct2 % 8 != 0);
			ll ct3b = ct3 / 8 + (ct3 % 8 != 0);

			string strim = "";
			vd dt;
			vector<unsigned int> probas;
			char c;

			repl(i, ct0b) { // lee el choclo
				if (inbin.get(c)) strim += c;
				else {
					cout << "Se pudrio todo" << endl;
					exit(1);
				}
			}

			ll ind = 0;
			unsigned int co = 4;
			bitset<32> buf;
			repl(i, ct1b) { // lee los datos que no son detallados
				if (inbin.get(c)) {
					/*repd(j, 8) buf[j] = c & (1 << j);
					bitset<4> dt1, dt2;
					ford(j, 7, 4) dt1[j - 4] = buf[j];
					repd(j, 4) dt2[j] = buf[j];
					union {
						int s;
						unsigned long l;
					} lunion;
					lunion.l = 4 * dt1.to_ulong();
					dt.pb();
					dt.pb(4 * dt2.to_ulong());
					ind += 2;*/
					co--;
					repd(j, 8) buf[j + 8 * co] = c & (1 << j);
					if (co == 0) {
						int ret = 0;
						//forn(i, 16, 32) tmp[i] = buf[i - 16];
						rep(i,32) tmp[i] = buf[i];
						union {
								int s;
								unsigned long l;
							} lunion;
						lunion.l = buf.to_ulong();
						dt.pb(lunion.s);
#ifdef DEBUG
						cout << "lunion.f = " << dec << lunion.s << " bits = " << hex <<lunion.l << endl;
#endif
						buf.reset();
						co = 4;
					}
				} else {
					cout << "Se pudrio todo" << endl;
					exit(1);
				}
			}

			co = 4;
			repl(i, ct2b) { // lee los datos que si son detallados
				if (inbin.get(c)) {
					co--;
					repd(j, 8) buf[j + 8 * co] = c & (1 << j);
					if (co == 0) {
						float ret = 0.0;
						bitset<32> tmp;
						//forn(i, 16, 32) tmp[i] = buf[i - 16];
						rep(i,32) tmp[i] = buf[i];
						union {
						        float f[ sizeof(unsigned long)/sizeof(float) ];
						        unsigned long l;
						    } funion;
						funion.l = tmp.to_ulong();
						dt.pb(funion.f[0]);
#ifdef DEBUG
						cout << "funion.f = " << funion.f[0] << " bits = " << hex <<funion.l << endl;
#endif
						buf.reset();
						co = 4;
					}
				} else {
					cout << "Se pudrio todo" << endl;
					exit(1);
				}
			}

			co = 4;
			buf.reset();
			repl(i, ct3b) { // lee las probas
				if (inbin.get(c)) {
					co--;
					repd(j, 8) buf[j + 8 * co] = c & (1 << j);
					if (co == 0) {
						probas.pb(buf.to_ulong());
						buf.reset();
						co = 4;
					}
				} else {
					cout << "Se pudrio todo" << endl;
					exit(1);
				}
			}

			//forn(i, sz(probas), szproba) probas.pb(1);

			if (sz(dt) == sz(probas) + 1) dt.erase(dt.begin() + ind - 1);

#ifdef DEBUG
			rep(i, sz(dt)) cout << dt[i] << " ";
			cout << endl;
			rep(i, sz(probas)) cout << dec << probas[i] << " ";
			cout << endl;

			cout << dec << sz(dt) << " " << sz(probas) << endl;
#endif
			assert(sz(probas) == szproba);
			assert(sz(dt) == szdata);

			cout << "Buffereado" << endl;
			choclo = util.ihuffman(strim, dt, probas);
			cout << sz(choclo) << " " << rows << " " << cols << endl;
			//assert(sz(choclo) == rows * cols);
			cout << "Dehuffmanizado" << endl;

			int constr = 1<<lvl; // 2^lvl

			int rrows = rows, ccols = cols;
			while (rrows % constr) rrows++;
			while (ccols % constr) ccols++;
#ifdef DEBUG
			cout << rrows << " " << ccols << endl;
#endif

			rep(i, rrows) {
				vd row;
				rep(j, ccols) {
#ifdef DEBUG
					cout << i << " " << j << " ";
#endif
					double tmp = choclo[i * ccols + j];
#ifdef DEBUG
					cout << tmp << endl;
#endif
					row.pb(tmp);
				}
				band.pb(row);
			}
#ifdef DEBUG
			cout << endl;
			rep(i, ccols) cout << band[1][i] << " ";
			cout << endl;
#endif
			cout << "Desenchoclado" << endl;

			vector<vector<double> > trband = util.idwt(band, rrows, ccols, lvl);
			cout << "Detransformado" << endl;

#ifdef DEBUG
			cout << endl;
			rep(i, cols) cout << trband[1][i] << " ";
			cout << endl;
#endif

			image.pb(trband);
			cout << "Iamgenizado" << endl;
			cout << "DEBUG: Banda " << ++nb << endl;
			getline(inbin, line); // una linea en blanco
		}

		inbin.close();
	} else {
		cout << "Problema al leer archivo" << endl;
		exit(1);
	}

	assert(sz(image) == bands);

	parser.create_image("descompr " + pathout, image, rows, cols, type);
}
