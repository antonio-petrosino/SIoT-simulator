using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <ctime>
#include "SystemBehaviour.h"


bool vDEBUG = true;
double alpha = 0.5;
double beta = 0.3;
double gamma = 0.2;
bool resource_ctrl = false;
bool qos_ctrl = false;
time_t tstart, tend;
Service* list_of_services;
Master* list_of_master;
Device* list_of_devices;
vector<Scheduler>  scheduler_records;
vector<Queue> info_queue;
int n_services, n_devices, n_master, lambda, tot_sim, seed;


int main() {	
	int max_resched = 100;
	cout << "Progetto SSIoT"<<endl;
	// seed the RNG	
	std::mt19937 rng(seed); // mt19937: Pseudo-random number generation

	tstart = time(0);

	n_services		= 6;	
	n_devices 		= 400;	
	n_master 		= 5;

	lambda			= 5;
	tot_sim			= 1000;	 // secondi
	seed			= 2;
		
	cout <<"Start..."<< endl;	
	srand(seed);  
	
	list_of_services = ServicesCreation();	
	list_of_master = MasterCreation();	
	list_of_devices = DeviceCreation();	
	GenerateSocialRel(n_devices, list_of_devices);
	scheduler_records = GenerateEventsArray(tot_sim);

	tend = time(0);
	cout << "Until scheduler lasts: " << difftime(tend, tstart) << " second(s)." << endl;

	Calendar event_calendar = Calendar(scheduler_records);

	while (!event_calendar.IsEmpty()) {
		
		Event next_event = event_calendar.GetNextEvent();
		int event_assigned = false;
		bool empty_list = false;

		if (vDEBUG) {
			cout << "Sched[" << next_event.GetSchedulerID() << "]..." << endl;
		}

		if (next_event.GetEventType() == "scheduler" || next_event.GetEventType() == "re-scheduler") {
			ServiceProviderFiltering(next_event.GetSchedulerID());
			event_assigned = Orchestrator_MakeDecisions(next_event.GetSchedulerID());	

			if (vDEBUG) {
				if (event_assigned) {
					cout << "Event assignment: TRUE" << endl;
				}
				else {
					cout << "Event assignment: FALSE" << endl;
				}
			}

			if (event_assigned == 1) {
				// schedulo riassegnazione risorse				
				double processing_time = EstimateProcessingTime(next_event.GetSchedulerID()); // cycles/s -> bit/cycles (1/1000) ->  bit 				
				double new_timestamp = next_event.GetTimeStamp() + processing_time;
				event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "end_service");

				if (vDEBUG) {
					cout << "Event: assigned." << endl;
				}
			}
			else if(event_assigned == -1){
				// schedulo di nuovo il servizio				
				double backoff = rand() % 35;
				double new_timestamp = next_event.GetTimeStamp() + backoff;

				if (scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() < max_resched) {
					scheduler_records[next_event.GetSchedulerID()].SetRescheduleTime(scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() + 1);
					event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "re-scheduler");

					if (vDEBUG) {
						cout << "Event: re-scheduler." << endl;
					}
				}				

			}else if(event_assigned == -2){
				// lista vuota???
				// a volte � falso perch� la lista dei provider � vuota? POSSIBILE?
				empty_list = true;

				if (vDEBUG) {
					cout << "Event: empty list." << endl;
				}
			}

			//UpdateQueue(next_event, event_assigned, empty_list);
			//event_calendar.DeleteEvent(next_event.GetEventID());
			
		}
		else if (next_event.GetEventType() == "end_service") {
			EndService(next_event.GetSchedulerID(), next_event.GetTimeStamp());		
			if (vDEBUG) {
				cout << "Event: End_service." << endl;
			}
			//cout << "..." << endl;
		}
		UpdateQueue(next_event, event_assigned, empty_list);
		event_calendar.DeleteEvent(next_event.GetEventID());		
		cout << "..." << endl;
	}

	tend = time(0);
	cout << "Until provider threshold: " << difftime(tend, tstart) << " second(s)." << endl;
  	system("pause");

	// possibilit� di calcolare quanto tempo effettivo impiega il master a calcolare tutto (poi vediamo)
	// si pu� fare

    return 0;
}
