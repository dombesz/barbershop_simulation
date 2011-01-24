/////////////////////////////////////////////////////////////////////
// barbershopem.h: Event Manager methods definition
///////////////////////////////////////////////////////////////////////

#include "stdlib.h"
#include <string.h>


/////////////////////////////////////////////////////////////////////
// CLASS EventManager
/////////////////////////////////////////////////////////////////////

// CLASS EventManager: Constructor

EventManager::EventManager(Simulation *sim) {

  simul=sim;

  // Resources instantiation
	barber= new Barber("John the barber",1,simul);

}

// CLASS EventManager: Destructor

EventManager::~EventManager() {

  // Resources destruction
 	delete barber;

}

// CLASS EventManager: Events execution

void EventManager::ExecuteEvent(int code, Client *client) {

  switch(code) {

 
  case 0:  barber->Event0(client);break;// Initial event - MANDATORY!!

  case 1:  barber->Event1(client);break;//Client arrives to barbershop
  case 2:  barber->Event2(client);break;//Client being served
  case 3:  barber->Event3(client);break;//Finish serving
  
  default: printf("Error: unknown event #%d at time %f\n",code,simul->Tnow());
  }
}

// CLASS EventManager: Stats init for each resource

void EventManager::Init() {

  // Resources

  barber->ResetStats();
}

// CLASS EventManager: Replications initialization

void EventManager::InitRep() {

  // Scheduler
  simul->Sched()->Purge();
  
  // Resources
  barber->ResetCounters();
  barber->PurgeQueue();
  
}

// CLASS EventManager: Stats computation for each resource

void EventManager::Stats() {

  // Resources

  barber->Stats();
}

// CLASS EventManager: Stats display for each resource

void EventManager::DisplayStats() {

  printf("\n*** SIMULATION STATISTICS ***\n\n");
  printf("\n*** RESOURCES\n");
	barber->DisplayStats();
}

/////////////////////////////////////////////////////////////////////
// CLASS Client
/////////////////////////////////////////////////////////////////////

// CLASS Client: Constructor
Client::Client(){

   }
 
// CLASS Client: setName() -Sets the name for the client

void Client::setName(char n[STRS]){
  
	strcpy(name,n);
   }
 
// CLASS Client: getName() -Returns the name of the client   

char* Client::getName(){
	
	return name;
	}
	
/////////////////////////////////////////////////////////////////////
// CLASS Barber
/////////////////////////////////////////////////////////////////////

// CLASS Barber: Constructor

   Barber::Barber(char n[STRS], int cap, Simulation *sim): Resource(n, cap, sim) {
  
	production=0;
	arrived=0;
	c_stack_size=0;
	stack_size=5;
   }


//Class Barber : Event#0 The initial event

void Barber::Event0(Client *client) {
		c_stack_size=1;
		arrived=1;

		char cname[STRS];
		sprintf(cname, "%d", arrived);
		client->setName(cname);	
		Sim()->Sched()->Schedule(1,Uni(0,10), client);
}
// CLASS Barber : Event #1 Client Arrives

void Barber::Event1(Client *client) {
		printf("Client %s arrived at time %f \n",client->getName(), Sim()->Tnow());
		
		if(c_stack_size<stack_size){	//Checks the number of free chairs
		this->P(2,client,1);
		  c_stack_size++;
		}else{
		printf("Client %s left (No free chairs) %f \n",client->getName(), Sim()->Tnow());	
			}
		
		Client *newclient; //Preparing the next client	recursively
		char cname[STRS];
		arrived++;
		sprintf(cname, "%d", arrived);
		newclient=Sim()->NewClient();
		newclient->setName(cname);	
		Sim()->Sched()->Schedule(1, Sim()->Tnow()+Uni(1,10), newclient);
		
}

// CLASS Barber : Event #2 Client Being Served

void Barber::Event2(Client *client){
	printf("Begin serving client %s on Barber at time %f \n",client->getName(),Sim()->Tnow());
	Sim()->Sched()->Schedule(3, Sim()->Tnow()+Exp(10), client);
}

// Class Barber : Event #3 Barber finishes serving, Client leaves the shop
void Barber::Event3(Client *client){
	printf("End serving client %s on Barber at time %f \n",client->getName(),Sim()->Tnow());
	this->V();					//Releasing barber
	production++;				//Another happy served client
	Sim()->KillClient(client); 	//Client leaves the shop
	c_stack_size--;				//A slot in the queue is freed up
}
