
/////////////////////////////////////////////////////////////////////
// DESP-C++ (Discrete-Event Simulation Package)
// Version 1.1, February 1998
// Jerome Darmont
// LIMOS, Blaise Pascal University (Clermont-Ferrand II), France
/////////////////////////////////////////////////////////////////////
// lewis.h: Lewis-Payne Random Generator
// Invariable
/////////////////////////////////////////////////////////////////////
// Call: variable=randu(lp_tt). lp_tt is the random generator seed.
// Its value must be initialized (e.g., to rand()) before first use
// of function randu(). randu() returns a long double.
/////////////////////////////////////////////////////////////////////

// Includes

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Global variables used by the generator (prefixed by "lp_")

const long int lp_m[99]= {0,
                    692513452, 1183199546, -408704453,-1948498849,
                   -741268548,-2056134124,  344472089, 1971513546,
                  -1186404342,-1288692714, 1300795239,-1058175985,
                   1279878134,-1398448201, 2093706256, 1318340072,
                   2069868445,  559626742,  653860201,  734005281,
                   1296960365, -475499606, 1237127688, 1691062534,
                    584524938,  598826735,  -15324583, 2011606675,
                    780278841,-1972910449, -226245823,  668140974,
                   1131266630,  249592506,  -86628624, -275463608,
                   -295592482,  166446056,-1112309364, 1293403676,
                   -424242630, 1590026273, 1360713837, -751543611,
                  -1645096292, -555286679,  713491269, 2123841324,
                   1229902981,  523752922, 1407975601, 1594927896,
                    356479372,-1011550732,-1215576184,-1625113692,
                   1881095498, -379674164, -537984567, -227157412,
                   -872755286,  -18090145, 1295468477,-1182986326,
                  -1874750317, -708581624, -655308140, 1503686128,
                    691242801, 1150891949,-1013129773,-1536584373,
                  -1059302984, 1085932207,-1683257391,-1341523941,
                  -1361343899, -610447094, -961687630,  434014805,
                   1370456709, 1804175579,   64864485, 1579425988,
                   1405582889,-2132743785,-1150733498,  643025249,
                   1288826228, 1727979288,-1597603817,  168759895,
                   -383327804, 1882949212, 1969381119, -706548769,
                   -885815584,-1787141026};

const long int lp_im2p31=2147483647;

long double lp_diviseur;
long int lp_mm[99], lp_igerm, lp_ibat[129];
int  lp_jrand, lp_krand;
long int lp_tt;

// randu() function

long double randu(long int& iu) {

  int  ii ;
  long int indbat,u ;
  long double temp  ;

  if (iu>0) {
    // init
    lp_diviseur=0.25/(1024.0*1024.0*1024.0);
    for (ii=0; ii<=98; ii++) lp_mm[ii]=lp_m[ii];
    lp_jrand=0;
    lp_igerm=iu;
    iu=-1;
    for (ii=1; ii<=128; ii++) {
      // fill table ibat with 128 random values from shift register...
      ++lp_jrand;
      if (lp_jrand>98) lp_jrand=1;
      lp_krand=lp_jrand+27;
      if (lp_krand>98) lp_krand=lp_krand-98;
      lp_mm[lp_jrand]^=lp_mm[lp_krand];
      lp_ibat[ii]=lp_mm[lp_jrand];
    }
  }

  // circulating (mod p) head of shift register

  if (++lp_jrand>98) lp_jrand=1;

  // circulating (mod p) x**q

  lp_krand=lp_jrand+27;
  if (lp_krand>98) lp_krand-=98;

  // prepare x**(p-1) for next shift, result stored in x**0

  lp_mm[lp_jrand]^=lp_mm[lp_krand];
  lp_igerm*=65539;
  lp_igerm&=lp_im2p31;
  indbat=1+(lp_igerm/16777216);
  u=lp_ibat[indbat];
  lp_ibat[indbat]=lp_mm[lp_jrand];
  temp=lp_diviseur*u;
  if (temp<=0) temp=temp+1.0 ;
  else if (temp>1) temp=1.0;

  return 1.0-temp; // 1 - randu
}
