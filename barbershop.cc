#include <stdio.h>
#include "simutil.h"
#include "simulc.h"
#include "barbershopec.h"
#include "simulm.h"
#include "barbershopem.h"

int main() {

  Simulation *sim;
  int nreplic, tsim;

  printf("\nNumber of replications: ");
  scanf("%d",&nreplic);
  
  printf("\nSimulation time:        ");
  scanf("%d",&tsim);
  
  sim=new Simulation(0,tsim,-1);

  printf("\nBEGIN Barbershop Simulation\n\n");
  sim->Run(nreplic);
  printf("\nEND Barbershop Simulation\n\n");
}
