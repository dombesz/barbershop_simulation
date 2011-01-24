/////////////////////////////////////////////////////////////////////
// barbershopec.h: Event Manager classes definition
// Variable with simulated systems
/////////////////////////////////////////////////////////////////////

// Resources declaration

class Barber;

/////////////////////////////////////////////////////////////////////
// CLASS EventManager
/////////////////////////////////////////////////////////////////////
// Simulation events management
/////////////////////////////////////////////////////////////////////

// The Event Manager must "know" all resources.

class EventManager {

  public:

    // Methods

    EventManager(Simulation *sim);      // Constructor
    ~EventManager();                    // Destructor
    void ExecuteEvent(int code, Client *client); // Event execution
    void Init();                        // Initialization
    void InitRep();                     // Replication initialization 
    void Stats();                       // Stats computation (end of replication)
    void DisplayStats();                // Statistics display

  private:

    // Attributes

    Simulation *simul;                  // Pointer toward simulation object

    // Resources
	
	Barber *barber;


};

/////////////////////////////////////////////////////////////////////
// CLASS Client
/////////////////////////////////////////////////////////////////////
// Custom simulation entity
/////////////////////////////////////////////////////////////////////

class Client {

  public:
           // Add here (in public) additional attributes
		   Client();
		   void setName(char n[STRS]);
		   char* getName();
           Client *next;
           Client *previous;
		   char name[STRS];
};

/////////////////////////////////////////////////////////////////////
// CLASS Barber
/////////////////////////////////////////////////////////////////////


class Barber: public Resource {

  public:

   // Constructor

    Barber(char n[STRS], int cap, Simulation *sim);

   // Events
	void Event0(Client *client); //The initial event
    void Event1(Client *client); //Client arrives
    void Event2(Client *client); //Client being served
	void Event3(Client *client); //Finish serving/leave shop
  private:
	int stack_size;		//Nr of chairs
	int c_stack_size;	//Nr of free chairs
	int arrived;		//Nr of arrived clients
	int production;		//counter
};

