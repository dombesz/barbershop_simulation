
/////////////////////////////////////////////////////////////////////
// DESP-C++ (Discrete-Event Simulation Package)
// Version 1.1, February 1998
// Jerome Darmont
// LIMOS, Blaise Pascal University (Clermont-Ferrand II), France
/////////////////////////////////////////////////////////////////////
// simutil.h: Utilitary functions
// Invariable
/////////////////////////////////////////////////////////////////////
// Random variables computation (float/int)
//
// - Poisson law:       float Poisson(float lambda);
//                      int IPoisson(float lambda);
// - Exponential law:   float Exp(float avg);
//                      int IExp(int avg);
// - Uniform law:       float Uni(float min, float max);
//                      int IUni(int min, int max);
/////////////////////////////////////////////////////////////////////
// Student t-distribution function: float t(int n);
/////////////////////////////////////////////////////////////////////
// Computation - number of digits in an integer: int digit(int n);
/////////////////////////////////////////////////////////////////////

#include "lewis.h"

/////////////////////////////////////////////////////////////////////
// Poisson law
/////////////////////////////////////////////////////////////////////

float Poisson(float lambda) {

  float res=-log(1-randu(lp_tt))/lambda;
  return res;
}

int IPoisson(float lambda) {

  int res=(int)(-log(1-randu(lp_tt))/lambda);
  return res;
}

/////////////////////////////////////////////////////////////////////
// Exponential law
/////////////////////////////////////////////////////////////////////

float Exp(float avg) {

  float res=-log(1-randu(lp_tt))*avg;
  return res;
}

int IExp(int avg) {

  int res=(int)(-log(1-randu(lp_tt))*avg);
  return res;
}

/////////////////////////////////////////////////////////////////////
// Uniform law
/////////////////////////////////////////////////////////////////////

float Uni(float min, float max) {

  float res=min+(max-min)*randu(lp_tt);
  return res;
}

int IUni(int min, int max) {

  int res=(int)(min+(max-min+1)*randu(lp_tt));
  return res;
}

/////////////////////////////////////////////////////////////////////
// Student t-distribution function
/////////////////////////////////////////////////////////////////////
// Returns the right critical value for the Student t-distribution,
// depending on n
/////////////////////////////////////////////////////////////////////

// Critical values for the Student t-distribution (constants)
// Gamma=0.975 (95% confidence intervals)
// Mark   0: n=1
// ...
// Mark  99: n=100
// Mark 100: n=102
// ...
// Mark 124: n=150
// Mark 125: n=200
// ...
// Mark 133: n=1000
// Mark 134: n=infinite

const float TDIST[135]={
  // 1..100	
 12.7062, 4.3027, 3.1824, 2.7764, 2.5706,
  2.4469, 2.3646, 2.3060, 2.2622, 2.2281,
  2.2010, 2.1788, 2.1604, 2.1448, 2.1315,
  2.1199, 2.1098, 2.1009, 2.0930, 2.0860,
  2.0796, 2.0739, 2.0687, 2.0639, 2.0595,
  2.0555, 2.0518, 2.0484, 2.0452, 2.0423,
  2.0395, 2.0369, 2.0345, 2.0322, 2.0301,
  2.0281, 2.0262, 2.0244, 2.0227, 2.0211,
  2.0195, 2.0181, 2.0167, 2.0154, 2.0141,
  2.0129, 2.0117, 2.0106, 2.0096, 2.0086,
  2.0076, 2.0066, 2.0057, 2.0049, 2.0040,
  2.0032, 2.0025, 2.0017, 2.0010, 2.0003,
  1.9996, 1.9990, 1.9983, 1.9977, 1.9971,
  1.9966, 1.9960, 1.9955, 1.9949, 1.9944,
  1.9939, 1.9935, 1.9930, 1.9925, 1.9921,
  1.9917, 1.9913, 1.9908, 1.9905, 1.9901,
  1.9897, 1.9893, 1.9890, 1.9886, 1.9883,
  1.9879, 1.9876, 1.9873, 1.9870, 1.9867,
  1.9864, 1.9861, 1.9758, 1.9855, 1.9853,
  1.9850, 1.9847, 1.9845, 1.9842, 1.9840,
  // 102..150
  1.9835, 1.9830, 1.9826, 1.9822, 1.9818,
  1.9814, 1.9810, 1.9806, 1.9803, 1.9799,
  1.9796, 1.9793, 1.9790, 1.9787, 1.9784, 
  1.9781, 1.9778, 1.9776, 1.9773, 1.9771,
  1.9768, 1.9766, 1.9763, 1.9761, 1.9759,
  // 200-1000 + infinite
  1.9719, 1.9679, 1.9659, 1.9647, 1.9639,
  1.9634, 1.9629, 1.9626, 1.9623, 1.9600
};

float t(int n) {

  if (n<=0) return -1; // Error
  else if (n<=100) return TDIST[n-1]; // 1..100
  else if (n<=150) return TDIST[((n-100)/2)+99]; // 102..150
  else if (n<=1000) return TDIST[((n-100)/100)+124]; // 200..1000
  else return TDIST[134]; // infinite
}

/////////////////////////////////////////////////////////////////////
// Computation of the number of digits in an integer
/////////////////////////////////////////////////////////////////////

int digit(int n) {

  int res=1;

  while (n>=10) {
    n=n/10;
    res++;
  }

  return res;
}