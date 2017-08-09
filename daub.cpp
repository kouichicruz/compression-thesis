/** Archivo: daub.cpp
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
#include "daub.hpp"
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

void Daubs::filt(VecDoub_IO &a, const int n, const int isign) {
	//Applies the previously initialized Daubn wavelet filter to data vector a[0..n-1] (for isign D 1)
	//or applies its transpose (for isign D 1). Used hierarchically by routines wt1 and wtn.
	double ai,ai1;
	int i,ii,j,jf,jr,k,n1,ni,nj,nh,nmod;
	if (n < 4) return;
	VecDoub wksp(n);
	nmod = ncof*n; // A positive constant equal to zero mod n.
	n1 = n-1; // Mask of all bits, since n a power of 2.
	nh = n >> 1;
	for (j=0;j<n;j++) wksp[j]=0.0;
	if (isign >= 0) { // Apply filter.
		for (ii=0,i=0;i<n;i+=2,ii++) {
			ni = i+1+nmod+ioff; // Pointer to be incremented and wrapped around.
			nj = i+1+nmod+joff;
			for (k=0;k<ncof;k++) {
				jf = n1 & (ni+k+1); // We use “bitwise and” to wrap around the
				jr = n1 & (nj+k+1); // pointers.
				wksp[ii] += cc[k]*a[jf];
				wksp[ii+nh] += cr[k]*a[jr];
			}
		}
	} else { // Apply transpose filter.
		for (ii=0,i=0;i<n;i+=2,ii++) {
			ai = a[ii];
			ai1 = a[ii+nh];
			ni = i+1+nmod+ioff; // See comments above.
			nj = i+1+nmod+joff;
			for (k=0;k<ncof;k++) {
				jf = n1 & (ni+k+1);
				jr = n1 & (nj+k+1);
				wksp[jf] += cc[k]*ai;
				wksp[jr] += cr[k]*ai1;
			}
		}
	}
	for (j=0;j<n;j++) a[j] = wksp[j]; // Copy the results back from workspace.
}

void Daub4::filt(VecDoub_IO &a, const int n, const int isign) {
/*Applies the Daubechies 4-coefficient wavelet filter to data vector a[0..n-1] (for isign=1)
or applies its transpose (for isign=-1). Used hierarchically by routines wt1 and wtn.*/
	const double C0=0.4829629131445341, C1=0.8365163037378077,
	C2=0.2241438680420134, C3=-0.1294095225512603;
	int nh,i,j;
	if (n < 4) return;
	VecDoub wksp(n);
	nh = n >> 1;
	if (isign >= 0) {// Apply filter.
		for (i=0,j=0;j<n-3;j+=2,i++) {
			wksp[i] = C0*a[j]+C1*a[j+1]+C2*a[j+2]+C3*a[j+3];
			wksp[i+nh] = C3*a[j]-C2*a[j+1]+C1*a[j+2]-C0*a[j+3];
		}
		wksp[i] = C0*a[n-2]+C1*a[n-1]+C2*a[0]+C3*a[1];
		wksp[i+nh] = C3*a[n-2]-C2*a[n-1]+C1*a[0]-C0*a[1];
	} else { // Apply transpose filter.
		wksp[0] = C2*a[nh-1]+C1*a[n-1]+C0*a[0]+C3*a[nh];
		wksp[1] = C3*a[nh-1]-C0*a[n-1]+C1*a[0]-C2*a[nh];
		for (i=0,j=2;i<nh-1;i++) {
			wksp[j++] = C2*a[i]+C1*a[i+nh]+C0*a[i+1]+C3*a[i+nh+1];
			wksp[j++] = C3*a[i]-C0*a[i+nh]+C1*a[i+1]-C2*a[i+nh+1];
		}
	}
	for (i=0;i<n;i++) a[i]=wksp[i];
}


void wt1(VecDoub_IO &a, const int isign, Wavelet &wlet, int lvl)
/*One-dimensional discrete wavelet transform. This routine implements the pyramid algorithm,
replacing a[0..n-1] by its wavelet transform (for isign=1), or performing the inverse operation
(for isign=-1). Note that n MUST be an integer power of 2. The object wlet, of type Wavelet,
is the underlying wavelet filter. Examples of Wavelet types are Daub4, Daubs, and Daub4i.*/
{
	int nn, n=a.size();
	if (n < 4) return;
	if (isign >= 0) { // Wavelet transform.
		wlet.condition(a,n,1);
		for (nn=n;nn>=4;nn>>=1) wlet.filt(a,nn,isign); // Start at largest hierarchy, and work toward smallest.
	} else {
		for (nn=4;nn<=n;nn<<=1) wlet.filt(a,nn,isign); // Start at smallest hierarchy, and work toward largest.
		wlet.condition(a,n,-1);
	}
}

void wtn(VecDoub_IO &a, VecInt_I &nn, const int isign, Wavelet &wlet, int lvl) {
/* Replaces a by its ndim-dimensional discrete wavelet transform, if isign is input as 1. Here
nn[0..ndim-1] is an integer array containing the lengths of each dimension (number of real
values), which must all be powers of 2. a is a real array of length equal to the product of these
lengths, in which the data are stored as in a multidimensional real array. If isign is input as
1, a is replaced by its inverse wavelet transform. The object wlet, of type Wavelet, is the
underlying wavelet filter. Examples of Wavelet types are Daub4, Daubs, and Daub4i. */
	int idim,i1,i2,i3,k,n,nnew,nprev=1,nt,ntot=1,i;
	int ndim=nn.size();
	for (idim=0;idim<ndim;idim++) ntot *= nn[idim];
	if (ntot % 8) {
		cout << "Impossibru" <<endl;
		exit(1);
	}
	for (idim=0;idim<ndim;idim++) { // Main loop over the dimensions.
		n=nn[idim];
		VecDoub wksp(n);
		nnew=n*nprev;
		if (n > 4) {
			for (i2=0;i2<ntot;i2+=nnew) {
				for (i1=0;i1<nprev;i1++) {
					for (i3=i1+i2,k=0;k<n;k++,i3+=nprev) wksp[k]=a[i3]; // Copy the relevant row or column or etc. into workspace.
					if (isign >= 0) { // Do one-dimensional wavelet transform.
						wlet.condition(wksp,n,1);
						for(nt=n, i=0;i<lvl;++i,nt >>= 1) {
							wlet.filt(wksp,nt,isign);
						}
					} else { // Or inverse transform.
						for(nt=(n>>(lvl-1)), i=0;i<lvl;++i,nt <<= 1) wlet.filt(wksp,nt,isign);
						wlet.condition(wksp,n,-1);
					}
					for (i3=i1+i2,k=0;k<n;k++,i3+=nprev) a[i3]=wksp[k]; // Copy back from workspace.
				}
			}
		}
		nprev=nnew;
	}
}
