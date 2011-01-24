
/////////////////////////////////////////////////////////////////////
// DESP-C++ (Discrete-Event Simulation Package)
// Version 1.1, February 1998
// Jerome Darmont
// LIMOS, Blaise Pascal University (Clermont-Ferrand II), France
/////////////////////////////////////////////////////////////////////
// simulc.h: Simulation engine classes definition
// Invariable
/////////////////////////////////////////////////////////////////////

class Simulation;
class Scheduler;
class SchedulerCell;
class Resource;
class QueueCell;

class EventManager; // Defined in the eventc.hh variable module
class Client;       // Defined in the eventc.hh variable module

/////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////

#define STRS 25               // Resources' names size
#define DEFAULT_SEED 127      // Default random seed

/////////////////////////////////////////////////////////////////////
// CLASS Simulation
/////////////////////////////////////////////////////////////////////
// Simulation control
/////////////////////////////////////////////////////////////////////

class Simulation {

  public:

    // Methods

    Simulation(float start, float max, long int seed); // Constructor
    ~Simulation();                      // Destructor
    void Run(int nreplic);              // Simulation execution
    Scheduler *Sched();                 // Returns scheduler address
    float Tnow();                       // Returns tnow
    float Tmax();                       // Returns tmax
    void Reset(float start, float max, long int seed); // Reinitialization
    Client *NewClient();                // Creates a client in clientlist
    void KillClient(Client *client);    // Deletes a client in clientlist
    void PurgeClientList();             // Deletes all clients

  private:

    // Private attributes

    float tstart;                       // Simulation starting time
    float tmax;                         // Simulation ending time
    float tnow;                         // Current date
    long int rseed;                     // Random generator seed
    Client *clientlist;                 // Clients list
    Scheduler *scheduler;               // Pointer toward scheduler
    EventManager *eventmanager;         // Pointer toward event manager

};

/////////////////////////////////////////////////////////////////////
// CLASS Scheduler
/////////////////////////////////////////////////////////////////////
// Scheduler (sorted event list)
/////////////////////////////////////////////////////////////////////

class Scheduler {

  public:

    // Methods

    Scheduler();                        // Constructor
    ~Scheduler();                       // Destructor
    int IsEmpty();                      // Returns scheduler state
    void Schedule(int eventcode, float eventdate, Client *client); // Insert
    int GetEventCode();                 // Returns next event code
    float GetEventDate();               // Returns next event date
    Client *GetClient();                // Returns client to "serve"
    void DestroyEvent();                // Deletes next event
    void Purge();                       // Deteles all events

  private:

    // Private attributes

    SchedulerCell *top;                 // Pointer toward 1st (next) event
    SchedulerCell *bottom;              // Pointer toward last event

};

/////////////////////////////////////////////////////////////////////
// CLASS SchedulerCell
/////////////////////////////////////////////////////////////////////
// Scheduler cell
/////////////////////////////////////////////////////////////////////

class SchedulerCell {

  public:

    // Methods

    SchedulerCell(int code, float date, Client *cli); // Constructor
    int Code();                         // Returns event code
    float Date();                       // Returns event date
    Client *Cli();                      // Returns client served
    SchedulerCell *Next();              // Returns next cell
    SchedulerCell *Previous();          // Returns previous cell
    void SetNext(SchedulerCell *newnext); // New next cell
    void SetPrevious(SchedulerCell *newprev); // New previous cell

  private:

    // Private attributes

    int eventcode;                      // Event code
    float eventdate;                    // Event date
    Client *client;                     // Client served
    SchedulerCell *next;                // Next cell
    SchedulerCell *previous;            // Previous cell

};

/////////////////////////////////////////////////////////////////////
// CLASS Resource
/////////////////////////////////////////////////////////////////////
// Passive resource (active resources are subclasses)
/////////////////////////////////////////////////////////////////////

class Resource {

  public:

    // Public methods

    Resource(char n[STRS], int cap, Simulation *sim); // Constructor
    ~Resource();                        // Destructor
    void PurgeQueue();                  // Empties queue
    void P(int event, Client *client, int prior); // Reserves resource
    void V();                           // Frees ressource
    Simulation *Sim();                  // Returns simulation object address
    void ResetCounters();               // Counters reinitialization
    void ResetStats();                  // Global stats reinitialization
    void Stats();                       // Stats computation
    void DisplayStats();                // Stats display
    float Mean(short i);                // Returns stats (mean value)
    float Dev(short i);                 // Returns stats (std dev)

  private:

    // Internal methods

    void EnQueue(int eventcode, Client *client, int priority); // Insert
    int GetEventCode();                 // Returns 1st event in queue
    Client *GetClient();                // Returns 1st client in queue
    void DestroyTop();                  // Deletes 1st element in queue
    int QueueEmpty();                   // Queue status

    // Private attributes

    char name[STRS];                    // Resource name
    QueueCell *top;                     // Queue top
    QueueCell *bottom;                  // Queue bottom
    int capacity;                       // Resource capacity
    int ccapacity;                      // Current capacity
    Simulation *simul;                  // Pointer toward simulation object
    float response;                     // Response time (1 replication)
    float wait;                         // Waiting time (1 replication)
    int nbserv;                         // Number of clients served
    float stats[5],stats2[5];           // Stats (accumulated)
    int n;                              // Stats (number of experiences)
    float mean[5], dev[5], cint[5];     // Mean values - Standard deviations - Confidence intervals
                                        // 0 : Response time
                                        // 1 : Waiting time
                                        // 2 : Number of clients served
                                        // 3 : Current number of clients
                                        // 4 : Number of waiting clients

};

/////////////////////////////////////////////////////////////////////
// CLASS QueueCell
/////////////////////////////////////////////////////////////////////
// Resource queue cell
/////////////////////////////////////////////////////////////////////

class QueueCell {

  public:

    // Methods

    QueueCell(int code, Client *cli, int prior); // Constructor
    int Code();                         // Returns event code
    Client *Cli();                      // Returns client
    int Priority();                     // Returns priority
    QueueCell *Next();                  // Returns next cell
    QueueCell *Previous();              // Returns previous cell
    void SetNext(QueueCell *newnext);   // New next cell
    void SetPrevious(QueueCell *newprev); // New previous cell

  private:

    // Private attributes

    int eventcode;                      // Event code
    Client *client;                     // Client
    int priority;                       // Priority
    QueueCell *next;                    // Next cell
    QueueCell *previous;                // Previous cell

};
