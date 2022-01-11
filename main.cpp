using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <ctime>
#include "SystemBehaviour.h"


bool vDEBUG = false;
double alpha = 0.5;
double beta = 0.3;
double gamma = 0.2;
bool resource_ctrl = false;
bool qos_ctrl = false;
time_t tstart, tend;


int main() {	  
	cout << "Progetto SSIoT"<<endl;

	int n_services, n_devices, n_master, lambda, tot_sim, seed;
	

    //float alfa, beta, gamma;

	tstart = time(0);

	n_services		= 6;	
	n_devices 		= 150;	
	n_master 		= 5;

	lambda			= 2;
	tot_sim			= 1000;	 // secondi
	seed			= 1;
		
	cout <<"Start..."<< endl;
	
	srand(seed);  
	
	Service *list_of_services;	
	list_of_services = ServicesCreation(n_services);
	
	Master *list_of_master;
	list_of_master = MasterCreation(n_master, n_services, list_of_services);
	
	Device *list_of_devices;
	list_of_devices = DeviceCreation(n_devices, n_services, n_master, list_of_services, list_of_master);
	
	GenerateSocialRel(n_devices, list_of_devices);	

	vector<Scheduler>  scheduler_records = GenerateEventsArray(seed, lambda, tot_sim, list_of_services, n_services, list_of_devices, n_devices, list_of_master, n_master);

	tend = time(0);
	cout << "Until scheduler lasts: " << difftime(tend, tstart) << " second(s)." << endl;

	Calendar event_calendar = Calendar(scheduler_records);

	while (!event_calendar.IsEmpty()) {
		
		Event next_event = event_calendar.GetNextEvent();
		bool event_assigned;
		cout << "Sched[" << next_event.GetSchedulerID() << "]..." << endl;

		if (next_event.GetEventType() == "scheduler") {
			scheduler_records[next_event.GetSchedulerID()] = ServiceProviderFiltering(scheduler_records[next_event.GetSchedulerID()], list_of_services, n_services, list_of_devices, n_devices, list_of_master, n_master, seed);

			event_assigned = Orchestrator_MakeDecisions(scheduler_records[next_event.GetSchedulerID()], list_of_master, n_master, list_of_devices, n_devices, list_of_services, n_services);

			if (event_assigned) {
				// schedulo riassegnazione risorse
				// tempo di calcolo???? 
				double processing_time = 100;
				double new_timestamp = next_event.GetTimeStamp() + processing_time;
				event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "end_service");
			}
			else {
				// schedulo di nuovo il servizio
				// backoff???
				double backoff = 200;
				double new_timestamp = next_event.GetTimeStamp() + backoff;
				event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "scheduler");
			}

			event_calendar.DeleteEvent();
			cout << "..." << endl;
		}
		else if (next_event.GetEventType() == "end_service") {
			cout << "..." << endl;
		}
	}

	tend = time(0);
	cout << "Until provider threshold: " << difftime(tend, tstart) << " second(s)." << endl;

  	system("pause");

	// possibilità di calcolare quanto tempo effettivo impiega il master a calcolare tutto (poi vediamo)
	// si può fare

    return 0;
}
