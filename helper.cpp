/** Archivo: helper.cpp
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
#include "util.hpp"
#include "helper.hpp"
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

vd enchoclar(vector<vd> band, vd &data, vector<unsigned int> &proba, int rows, int cols) {
	vd choclo;
	map<double, int> ass, qty;
	int counter = 0;

	rep(r, rows){
		//cout << "Huffman a fila " << r << " de " << ts << endl;

		rep(c, cols) {
			//cout << "(" << band[r][c] << ", " << trband[r][c] << ") ";
			// anadir a la representacion
			pair<map<double,int>::iterator, bool> ret = ass.insert(mp(band[r][c], counter));
			if (!(ret.second)) { // si el elemento ya estaba
				qty[band[r][c]]++;
				proba[(ret.first)->second]++;
			} else { // si el elemento no estaba
				qty.insert(mp(band[r][c], 1));
				proba.pb(1);
				data.pb(band[r][c]);
				//cout << data[counter] << " ";
				//if (data[counter] < 0) cout << endl << r << " " << c << endl;
				counter++;
			}
			choclo.pb(band[r][c]);
		}
		//cout << endl;
	}
	assert(sz(data) == sz(proba));

	//TODO: Hacer algo mas eficiente que las proximas 8 lineas
	std::vector<pair<double, int> > v;
	rep(i, sz(data)) v.pb(mp(data[i], proba[i]));
	sort(v.begin(), v.end());

	rep(i, sz(data)) {
		data[i] = (v[i]).first;
		proba[i] = (v[i]).second;
	}

	assert(sz(choclo) == rows * cols);

	cout << "Enchoclado" << endl;

	return choclo;
}


void write_data(vector<string> outhuf, vd data, vector<unsigned int> proba, ofstream &outbin, int quantindex, Util util) {
	int icount = 0, index = 0;
	int breakp = -1, breakp2 = -1;
	unsigned char buf = 0;
	ll coun[4] = { }, ct[4] = { };

	vector<string> outdata, outproba;

	int ctc = 0;
	ll sth = 0;
	rep(i, sz(outhuf)) if ((outhuf[i]).compare(" ") != 0) sth += sz(outhuf[i]);
	ct[0] = sth / 8 + (sth % 8 != 0);

	// Añade los datos y las probabilidades al final
	rep(i, sz(data)) {
		if (fabs(data[i]) < 4.0 * quantindex) {
			//string code = util.encode_data(data[i], quantindex);
			string code, probastr;
			util.encode_everything(data[i], quantindex, proba[i], code, probastr);
			ct[1] += sz(code);
			ct[3] += sz(probastr);
			outdata.pb(code);
			outproba.pb(probastr);
		}
	}
	outdata.pb(" ");
	rep(i, sz(data)) {
		if (fabs(data[i]) > 4.0 * quantindex) {
			//string code = util.encode_data(data[i], quantindex);
			string code, probastr;
			util.encode_everything(data[i], quantindex, proba[i], code, probastr);
			ct[2] += sz(code);
			ct[3] += sz(probastr);
			outdata.pb(code);
			outproba.pb(probastr);
		}
	}

	outbin << ct[0] << " " << ct[1] << " " << ct[2] << " " << ct[3] << " " << quantindex << " " << sz(data) << " " << sz(proba) << endl;

	int stco = 0;
	rep(i, sz(outhuf)) {
		if ((outhuf[i]).compare(" ") != 0) rep(c, sz(outhuf[i])) {
			if (outhuf[i][c] == '1') buf |= (1 << (7 - icount));
			icount++;
			if (icount % 8 == 0) {
				outbin.put(buf);
				buf = 0;
				icount = 0;
				coun[stco]++;
			}
		}
	}
	if (icount % 8 != 0) {
		outbin.put(buf);
		coun[stco]++;
	}
	stco++;
	buf = 0;
	icount = 0;

	rep(i, sz(outdata)) {
		if ((outdata[i]).compare(" ") != 0) rep(c, sz(outdata[i])) {
			if (outdata[i][c] == '1') buf |= (1 << (7 - icount));
			icount++;
			if (icount % 8 == 0) {
				outbin.put(buf);
				buf = 0;
				icount = 0;
				coun[stco]++;
			}
		} else {
			if (icount % 8 != 0) {
				outbin.put(buf);
				coun[stco]++;
			}
			stco++;
			buf = 0;
			icount = 0;
		}
	}
	if (icount % 8 != 0) {
		outbin.put(buf);
		coun[stco]++;
	}
	stco++;
	buf = 0;
	icount = 0;

	rep(i, sz(outproba)) {
		if ((outproba[i]).compare(" ") != 0) rep(c, sz(outproba[i])) {
			if (outproba[i][c] == '1') buf |= (1 << (7 - icount));
			icount++;
			if (icount % 8 == 0) {
				outbin.put(buf);
				buf = 0;
				icount = 0;
				coun[stco]++;
			}
		}
	}
	if (icount % 8 != 0) {
		outbin.put(buf);
		coun[stco]++;
	}

	cout << coun[0] << " " << ct[0] << endl;
	cout << coun[1] << " " << ct[1] << endl;
	cout << coun[2] << " " << ct[2] << endl;
	cout << coun[3] << " " << ct[3] << endl;
	assert(coun[0] == ct[0]);
	assert(coun[1] == ct[1] / 8 + (ct[1] % 8 != 0));
	assert(coun[2] == ct[2] / 8 + (ct[2] % 8 != 0));
	assert(coun[3] == ct[3] / 8 + (ct[3] % 8 != 0));
}
