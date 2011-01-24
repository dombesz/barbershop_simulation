
/////////////////////////////////////////////////////////////////////
// DESP-C++ (Discrete-Event Simulation Package)
// Version 1.1, February 1998
// Jerome Darmont
// LIMOS, Blaise Pascal University (Clermont-Ferrand II), France
/////////////////////////////////////////////////////////////////////
// simulm.h: Simulation engine methods definition
// Invariable
/////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// Includes
/////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <math.h>
#include <string.h>

/////////////////////////////////////////////////////////////////////
// CLASS Simulation
/////////////////////////////////////////////////////////////////////

// CLASS Simulation: Constructor

Simulation::Simulation(float start, float max, long int seed) {

  Reset(start, max, seed);
  clientlist=NULL;
  scheduler=new Scheduler;
  eventmanager=new EventManager(this);
}

// CLASS Simulation: Destructor

Simulation::~Simulation() {

  delete scheduler;
  delete eventmanager;
}

// CLASS Simulation: Simulation execution

void Simulation::Run(int nreplic) {

  int i, nextevent, charcount;
  Client *client;

  // Initialization
  eventmanager->Init();

  printf("\nSimulation started... ");
  charcount=21;

  for (i=1; i<=nreplic; i++) {

    charcount+=(digit(i)+3);
    if (charcount>79) {
      charcount=digit(i)+3;
      printf("\n");
    }
    printf("[%d] ",i);
    // Replication initialization
    tnow=tstart;
	
    eventmanager->InitRep();
	client=NewClient();      // DO NOT FORGET TO DESTROY CLIENTS!
	eventmanager->ExecuteEvent(0,client);
    
	// Engine
    while ((tnow<tmax) && (!scheduler->IsEmpty())) {      
	  nextevent=scheduler->GetEventCode();
      tnow=scheduler->GetEventDate();
      client=scheduler->GetClient();
      scheduler->DestroyEvent();
      eventmanager->ExecuteEvent(nextevent,client);
    }

    // Statistics computation
    eventmanager->Stats();

    // Destruction of clients still in system
    PurgeClientList();
  }

  charcount+=17;
  if (charcount>79) printf("\n");
  printf("End of simulation\n");

  // Results
  eventmanager->DisplayStats();
}

// CLASS Simulation: Returns the Scheduler address

Scheduler *Simulation::Sched() {

  return scheduler;
}

// CLASS Simulation: Returns tnow

float Simulation::Tnow() {

  return tnow;
}

// CLASS Simulation: Returns tmax

float Simulation::Tmax() {

  return tmax;
}

// CLASS Simulation: Reinitialization

void Simulation::Reset(float start, float max, long int seed) {

  tstart=start;
  tmax=max;
  if (seed>0) rseed=seed;
  else rseed=DEFAULT_SEED;

  // Randomize
  lp_tt=rseed;
}

// CLASS Simulation: Creation of a new client in clientlist

Client *Simulation::NewClient() {

  Client *newclient;

  newclient=new Client;
  // Insertion on top
  newclient->next=clientlist;
  newclient->previous=NULL;
  if (clientlist!=NULL) clientlist->previous=newclient;
  clientlist=newclient;
  return newclient;
}

// CLASS Simulation: Deletion of a client in clientlist

void Simulation::KillClient(Client *client) {

  Client *p, *n;

  p=client->previous;
  n=client->next;

  if (p!=NULL) p->next=n;
  else clientlist=n; // on top
  if (n!=NULL) n->previous=p;

  delete client;
}

// CLASS Simulation: Deletion of all clients in clientlist

void Simulation::PurgeClientList() {

  Client *cour, *prec;

  cour=clientlist;
  prec=NULL;
  while (cour!=NULL) {
    prec=cour;
    cour=cour->next;
    delete prec;
  }
  clientlist=NULL;
}

/////////////////////////////////////////////////////////////////////
// CLASS Scheduler
/////////////////////////////////////////////////////////////////////

// CLASS Scheduler: Constructor

Scheduler::Scheduler() {

  top=NULL;
  bottom=NULL;
}

// CLASS Scheduler: Destructor

Scheduler::~Scheduler() {

  Purge();
}

// CLASS Scheduler: Returns scheduler state
// (1 if empty, 0 if not)

int Scheduler::IsEmpty() {

  if (top==NULL) return 1;
  else return 0;
}

// CLASS Scheduler: Insertion into scheduler

void Scheduler::Schedule(int eventcode, float eventdate, Client *client) {

  SchedulerCell *prec, *cour, *nouv;

  nouv=new SchedulerCell(eventcode,eventdate,client);

  prec=NULL;
  cour=bottom;

  while ((cour!=NULL) && (eventdate<cour->Date())) {
    prec=cour;
    cour=cour->Previous();
  }

  nouv->SetPrevious(cour);
  nouv->SetNext(prec);

  if (prec!=NULL) prec->SetPrevious(nouv);
  else bottom=nouv;
  if (cour!=NULL) cour->SetNext(nouv);
  else top=nouv;
}

// CLASS Scheduler: Returns 1st event code

int Scheduler::GetEventCode() {

  if (top!=NULL) return top->Code();
  else return -1;
}

// CLASS Scheduler: Returns last event code

float Scheduler::GetEventDate() {

  if (top!=NULL) return top->Date();
  else return -1;
}

// CLASS Scheduler: Returns pointer toward client

Client *Scheduler::GetClient() {

  if (top!=NULL) return top->Cli();
  else return NULL;
}

// CLASS Scheduler: Deletes 1st event

void Scheduler::DestroyEvent() {

  SchedulerCell *sauve;

  sauve=top;
  if (top->Next()!=NULL) {
    top=top->Next();
    top->SetPrevious(NULL);
  } else {
    top=NULL;
    bottom=NULL;
  }
  delete sauve;
}

// CLASS Scheduler: Empties the scheduler

void Scheduler::Purge() {

  SchedulerCell *cour, *save;

  cour=bottom;
  while (cour!=NULL) {
    save=cour;
    cour=cour->Next();
    delete save;
  }

  top=NULL;
  bottom=NULL;
}

/////////////////////////////////////////////////////////////////////
// CLASS SchedulerCell
/////////////////////////////////////////////////////////////////////

// CLASS SchedulerCell: Constructor

SchedulerCell::SchedulerCell(int code, float date, Client *cli) {

  eventcode=code;
  eventdate=date;
  client=cli;
  next=NULL;
  previous=NULL;
}

// CLASS SchedulerCell: Returns event code

int SchedulerCell::Code() {

  return eventcode;
}

// CLASS SchedulerCell: Returns event date

float SchedulerCell::Date() {

  return eventdate;
}

// CLASS SchedulerCell: Returns pointer toward client

Client *SchedulerCell::Cli() {

  return client;
}

// CLASS SchedulerCell: Returns next cell

SchedulerCell *SchedulerCell::Next() {

  return next;
}

// CLASS SchedulerCell: Returns previous cell

SchedulerCell *SchedulerCell::Previous() {

  return previous;
}

// CLASS SchedulerCell: Reintitialize pointer toward next cell

void SchedulerCell::SetNext(SchedulerCell *newnext) {

  next=newnext;
}

// CLASS SchedulerCell: Reintitialize pointer toward previous cell

void SchedulerCell::SetPrevious(SchedulerCell *newprev) {

  previous=newprev;
}

/////////////////////////////////////////////////////////////////////
// CLASS Resource
/////////////////////////////////////////////////////////////////////

// CLASS Resource: Constructor

Resource::Resource(char n[STRS], int cap, Simulation *sim) {

  strcpy(name,n);
  capacity=cap;
  simul=sim;
  top=NULL;
  bottom=NULL;
}

// CLASS Resource: Destructor

Resource::~Resource() {

  PurgeQueue();
}

// CLASS Resource: Empties queue

void Resource::PurgeQueue() {

  QueueCell *cour, *save;

  cour=bottom;
  while (cour!=NULL) {
    save=cour;
    cour=cour->Next();
    delete save;
  }

  top=NULL;
  bottom=NULL;
}

// CLASS Resource: Resource reservation (P)

void Resource::P(int event, Client *client, int prior) {

  ccapacity--;
  if (ccapacity>=0) {                    // Immediate action
    response-=simul->Tnow();
    simul->Sched()->Schedule(event,simul->Tnow(),client);
  } else {                               // Event is put in queue
    wait-=simul->Tnow();
    EnQueue(event,client,prior);
  }
}

// CLASS Resource: Resource liberation (V)

void Resource::V() {

  Client *nextclient;

  ccapacity++;
  if (ccapacity>capacity) {
    printf("Error: capacity overflow for resource %s at time %f\n",name,simul->Tnow());
    ccapacity=capacity;
  }
  response+=simul->Tnow();
  nbserv++;
  if (!QueueEmpty()) {                  // Queue not empty
    nextclient=GetClient();
    wait+=simul->Tnow();
    response-=simul->Tnow();
    simul->Sched()->Schedule(GetEventCode(),simul->Tnow(),nextclient);
    DestroyTop();
  }
}

// CLASS Resource: Returns simulation object address

Simulation *Resource::Sim() {

  return simul;
}

// CLASS Resource: Statistical counters initialization

void Resource::ResetCounters() {

  ccapacity=capacity;
  response=0;
  wait=0;
  nbserv=0;
}

// CLASS Resource: Global stats initialization 

void Resource::ResetStats() {

  int i;

  for (i=0; i<5; i++) {
    stats[i]=0;
    stats2[i]=0;
  }
  n=0;

}

// CLASS Resource: Statistics computation

void Resource::Stats() {

  int nbwait, nbbs, i;
  float s[5];

  if (ccapacity<0) {
    nbwait=-ccapacity;
    nbbs=capacity;
  } else {
    nbwait=0;
    nbbs=capacity-ccapacity;
  }

  // Response time (for the current replication)
  if (nbserv!=0) s[0]=(response+nbbs*Sim()->Tnow())/nbserv;
  else s[0]=0;
  // Waiting time (for the current replication)
  if ((nbserv+nbbs)!=0) s[1]=(wait+nbwait*Sim()->Tnow())/(nbserv+nbbs);
  else s[1]=0;
  // Served (for the current replication)
  s[2]=nbserv;
  // Being served (for the replication)
  s[3]=nbbs;
  // Waiting (for the replication)
  s[4]=nbwait;

  // Additions
  for (i=0; i<5; i++) {
    stats[i]+=s[i];
    stats2[i]+=s[i]*s[i];
  }
  n++;
}

// CLASS Resource: Statistics display

void Resource::DisplayStats() {

  int i;

  // Computation

  for (i=0; i<5; i++) {
    if (n!=0) mean[i]=stats[i]/n;
    else mean[i]=0;
    if (n!=0) dev[i]=(n*stats2[i]-stats[i]*stats[i])/(n*n);
    else dev[i]=0;
    if (dev[i]>0) dev[i]=sqrt(dev[i]);
    else dev[i]=0;
    if (n>1) cint[i]=t(n-1)*dev[i]/sqrt(n);
    else cint[i]=0;
  }

  // Display

  printf("\nStatistics for resource: %s (0.95 confidence interval)\n\n",name);
  printf("\t* Mean response time              : %10.2f\t+/- %10.2f\n",mean[0],cint[0]);
  printf("\t* Mean waiting time               : %10.2f\t+/- %10.2f\n",mean[1],cint[1]);
  printf("\t* Mean # of clients served        : %10.2f\t+/- %10.2f\n",mean[2],cint[2]);
  printf("\t* Mean # of clients being served  : %10.2f\t+/- %10.2f\n",mean[3],cint[3]);
  printf("\t* Mean # of clients still waiting : %10.2f\t+/- %10.2f\n",mean[4],cint[4]);
}

// CLASS Resource: Returns mean value

float Resource::Mean(short i) {

  if ((i>=0) && (i<=4)) return mean[i];
  else return -1;
}

// CLASS Resource: Returns standard deviation

float Resource::Dev(short i) {

  if ((i>=0) && (i<=4)) return dev[i];
  else return -1;
}

// CLASS Resource: Insertion into queue

void Resource::EnQueue(int eventcode, Client *client, int priority) {

  QueueCell *prec, *cour, *nouv;

  nouv=new QueueCell(eventcode,client,priority);

  prec=NULL;
  cour=bottom;

  while ((cour!=NULL) && (priority>cour->Priority())) {
    prec=cour;
    cour=cour->Previous();
  }

  nouv->SetPrevious(cour);
  nouv->SetNext(prec);

  if (prec!=NULL) prec->SetPrevious(nouv);
  else bottom=nouv;
  if (cour!=NULL) cour->SetNext(nouv);
  else top=nouv;
}

// CLASS Resource: Returns the code of the 1st event in queue 

int Resource::GetEventCode() {

  if (top!=NULL) return top->Code();
  else return -1;
}

// CLASS Resource: Returns pointer toward 1st client in queue

Client *Resource::GetClient() {

  if (top!=NULL) return top->Cli();
  else return NULL;
}

// CLASS Resource: Deletes queue top

void Resource::DestroyTop() {

  QueueCell *save;

  if (top!=NULL) {
    save=top;
    top=top->Next();
    if  (top!=NULL) top->SetPrevious(NULL);
    else bottom=NULL;
    delete save;
  }
}

// CLASS Resource: Returns queue status
// (1: empty, 0: not empty)

int Resource::QueueEmpty() {

  if (top==NULL) return 1;
  else return 0;
}

/////////////////////////////////////////////////////////////////////
// CLASS QueueCell
/////////////////////////////////////////////////////////////////////

// CLASS QueueCell: Constructor

QueueCell::QueueCell(int code, Client *cli, int prior) {

  eventcode=code;
  client=cli;
  priority=prior;
  next=NULL;
  previous=NULL;
}

// CLASS QueueCell: Returns event code

int QueueCell::Code() {

  return eventcode;
}

// CLASS QueueCell: Returns pointer toward client

Client *QueueCell::Cli() {

  return client;
}

// CLASS QueueCell: Returns event priority

int QueueCell::Priority() {

  return priority;
}

// CLASS QueueCell: Returns pointer toward next cell

QueueCell *QueueCell::Next() {

  return next;
}

// CLASS QueueCell: Returns pointer toward previous cell

QueueCell *QueueCell::Previous() {

  return previous;
}

// CLASS QueueCell: Reinitializes pointer toward next cell

void QueueCell::SetNext(QueueCell *newnext) {

  next=newnext;
}

// CLASS QueueCell: Reinitializes pointer toward previous cell

void QueueCell::SetPrevious(QueueCell *newprev) {

  previous=newprev;
}
