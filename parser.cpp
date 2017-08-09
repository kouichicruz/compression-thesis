/** Archivo: parser.cpp
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
#include "parser.hpp"
#include "/usr/local/include/gdal_priv.h"
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


ll expo(ll a, ll b) {
	if (b == 0) return 1;
	if (b % 2) return a * expo(a, b - 1);
	ll c = expo(a, b / 2);
	return c * c;
}

Parser::Parser() {
	GDALAllRegister();
}

void Parser::parse_image(string file, int &rows, int &cols, int &type, int &bands) {
	GDALDataset* dataset = (GDALDataset *) GDALOpen(file.c_str(), GA_ReadOnly);
	assert(dataset != NULL);
	//GDALRasterBand* band;
	int xSize, ySize;
	vector<vector<vd> > ret;
	
	bands = dataset->GetRasterCount();
	GDALRasterBand* band = dataset->GetRasterBand(1);
	assert(band != NULL);
	type = band->GetRasterDataType();
	cols = dataset->GetRasterXSize();
	rows = dataset->GetRasterYSize();
}

vector<vd> Parser::parse_band(string file, int bandn, int &rows, int &cols, int lvl) {
	GDALDataset* dataset = (GDALDataset *) GDALOpen(file.c_str(), GA_ReadOnly);
	vector<vd> oband;
	GDALRasterBand* band = dataset->GetRasterBand(bandn + 1);
	assert(band != NULL);
	
	int xSize = band->GetXSize();
	int ySize = band->GetYSize();
	rows = ySize;
	cols = xSize;
	rep(i, ySize) {
		double* row = (double *) CPLMalloc(sizeof(double)*xSize);
		vd vrow;
		
		band->RasterIO(GF_Read, 0, i, xSize, 1, row, xSize, 1, GDT_Float64, 0, 0); // se los lee en double de 64 por las dudas
		
		rep(j, xSize) {
			vrow.pb(row[j]);
		}
		oband.pb(vrow);
		
		free(row);
	}

	extend_band(oband, rows, cols, lvl);

	return oband;
}

void Parser::create_image(string file, vector<vector<vd> > image, int rows, int cols, int type) {
	GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	assert(poDriver != NULL);
	
	char** papszMetadata = poDriver->GetMetadata();
	if(CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE)) printf( "Driver supports Create() method.\n");
	if(CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE )) printf( "Driver supports CreateCopy() method.\n");
	
	GDALDataType tarp = static_cast<GDALDataType>(type);
	
	GDALDataset *poDstDS = poDriver->Create(file.c_str(), cols, rows, sz(image), tarp, NULL);
	assert(poDstDS != NULL);
	
	cout << type << endl;
	
	GDALRasterBand* poBand;
	rep(b, sz(image)) {
		poBand = poDstDS->GetRasterBand(b + 1);
		vector<vd> band = image[b];
		rep(i, rows) { // iterar para cada fila
			if (type == 1) {
				unsigned char row[cols];
				rep(j, cols) row[j] = band[i][j];
			
				poBand->RasterIO(GF_Write, 0, i, cols, 1, row, cols, 1, tarp, 0, 0);
			} else if (type == 2) {
				uint16_t row[cols];
				rep(j, cols) row[j] = band[i][j];
			
				poBand->RasterIO(GF_Write, 0, i, cols, 1, row, cols, 1, tarp, 0, 0);
			} else if (type == 4) {
				uint32_t row[cols];
				rep(j, cols) row[j] = band[i][j];
			
				poBand->RasterIO(GF_Write, 0, i, cols, 1, row, cols, 1, tarp, 0, 0);
			} else if (type == 6) {
				double row[cols];
				rep(j, cols) row[j] = band[i][j];
			
				poBand->RasterIO(GF_Write, 0, i, cols, 1, row, cols, 1, tarp, 0, 0);
			}
		}
	}
	
	GDALClose(poDstDS);
}

void Parser::extend_band(vector<vd> &band, int &rows, int &cols, int lvl) {
	vd empt;
	rep(i, cols) empt.pb(0.0);

	cout << "Antes la imagen tiene " << rows << "filas y " << cols << endl;

	int constr = 1<<lvl; // 2^lvl

	while (rows % constr) {
		vd empt2;
		rep(i, cols) empt2.pb(0.0);
		band.pb(empt2);
		rows++;
	}

	while (cols % constr) {
		rep(i, rows) (band[i]).pb(0.0);
		cols++;
	}

	cout << "La imagen ahora tiene " << rows << "filas y " << cols << endl;
}

void Parser::get_metadata(string line, ll &ct0, ll &ct1, ll &ct2, ll &ct3, ll &quantindex, ll &szdata, ll &szproba) {
	string sco0, sco1, sco2, sco3, squant, sdata, sproba;
	istringstream isi(line);
	isi >> sco0 >> sco1 >> sco2 >> sco3 >> squant >> sdata >> sproba;
	cout << sco0 << " " << sco1 << " " << sco2 << " " << sco3 << " " << squant << " " << sdata << " " << sproba << endl;


	ct0 = stoull(sco0);
	ct1 = stoull(sco1);
	ct2 = stoull(sco2);
	ct3 = stoull(sco3);
	quantindex = stoi(squant);
	szdata = stoull(sdata);
	szproba = stoull(sproba);
	cout << ct0 << " " << ct1 << " " << ct2 << " " << ct3 << " " << quantindex << " " << szdata << " " << szproba << endl;

}
