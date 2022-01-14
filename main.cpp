using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>  
#include "SystemBehaviour.h"

#include<direct.h>

bool vDEBUG = false;
double alpha = 0.5;
double beta = 0.3;
double gamma = 0.2;
bool resource_ctrl;
bool qos_ctrl;
unsigned long tstart, tend;
Service* list_of_services;
Master* list_of_master;
Device* list_of_devices;
vector<Scheduler>  scheduler_records;
vector<Queue> info_queue;
int n_services, n_devices, n_master, lambda, tot_sim, seed;
double cutting_value;
string folder_name;

int main() {	
	int max_resched = 99999;
	cutting_value = 0.24; 
	//cutting_value = 0.3;
	cout << "SSIoT Simulator"<<endl;
	
	// seed the RNG	
	std::mt19937 rng(seed); // mt19937: Pseudo-random number generation
		
	Tic();

	resource_ctrl	= false;
	qos_ctrl		= false;

	n_services		= 6;	
	n_devices 		= 100;	
	n_master 		= 5;

	lambda			= 10;
	tot_sim			= 3000;	 // secondi
	seed			= 10;
	

	folder_name = "Sim-n_services_"+ to_string(n_services)+"-n_devices_" + to_string(n_devices) + "-n_master_" + to_string(n_master) +
		"-lambda_" + to_string(lambda) + "-tot_sim_"+ to_string(tot_sim) + "-seed_"+ to_string(seed) +"-resource_ctrl_"
		+ to_string(resource_ctrl) +"-qos_ctrl_"+to_string(qos_ctrl) +"/";

	const char* folder_name_char = folder_name.c_str();
	if (_mkdir(folder_name_char) == -1) {
		cout << "Folder exists." << endl;
	}
	else {
		cout << "Folder created." << endl;
	}

	cout <<"Start..."<< endl;	
	srand(seed);  
	
	list_of_services = ServicesCreation();	
	list_of_master = MasterCreation();	
	list_of_devices = DeviceCreation();	
	GenerateSocialRel(n_devices, list_of_devices);
	scheduler_records = GenerateEventsArray(tot_sim);

	Toc("scheduler");
	//tend = time(0);
	//cout << "Until scheduler lasts: " << difftime(tend, tstart) << " second(s)." << endl;
	std::cout << std::setprecision(3) << std::fixed;

	Calendar event_calendar = Calendar(scheduler_records);

	while (!event_calendar.IsEmpty()) {
		Toc("init get next event");
		Event next_event = event_calendar.GetNextEvent();
		unsigned long start_master_processing = clock();
		Toc("end get next event");

		int event_assigned = false;
		bool empty_list = false;

		if (vDEBUG) {
			cout << "Sched[" << next_event.GetSchedulerID() << "]..." << endl;
		}

		if (next_event.GetEventType() == "scheduler" || next_event.GetEventType() == "re-scheduler") {
			Toc("start ServiceProviderFiltering");
			ServiceProviderFiltering(next_event.GetSchedulerID());
			Toc("end ServiceProviderFiltering - start Orchestrator_MakeDecisions");

			event_assigned = Orchestrator_MakeDecisions(next_event.GetSchedulerID());	
			Toc("end Orchestrator_MakeDecisions");
			unsigned long end_master_processing = clock();
			unsigned long total_master_processing = (end_master_processing - start_master_processing) / 1000;
			if (vDEBUG) {
				if (event_assigned == 1) {
					cout << "Event assignment: TRUE" << endl;
				}
				else {
					cout << "Event assignment: FALSE" << endl;
				}
			}

			if (event_assigned == 1) {
				// schedulo riassegnazione risorse				
				double processing_time = EstimateProcessingTime(next_event.GetSchedulerID()); // cycles/s -> bit/cycles (1/1000) ->  bit 				
				double new_timestamp = next_event.GetTimeStamp() + processing_time + total_master_processing;

				Toc("start add event");
				event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "end_service");
				Toc("end add event");

				if (vDEBUG) {
					cout << "Event: assigned." << endl;
				}
			}
			else if(event_assigned == -1){
				// schedulo di nuovo il servizio				
				double backoff = rand() % 35;
				double new_timestamp = next_event.GetTimeStamp() + backoff + total_master_processing;

				if (scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() < max_resched) {
					scheduler_records[next_event.GetSchedulerID()].SetRescheduleTime(scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() + 1);

					Toc("start add event");
					event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "re-scheduler");
					Toc("end add event");

					if (vDEBUG) {
						cout << "Event: re-scheduler." << endl;
					}
				}				

			}else if(event_assigned == -2){
				// lista vuota???
				// a volte è falso perchè la lista dei provider è vuota? 
				empty_list = true;

				if (vDEBUG) {
					cout << "Event: empty list." << endl;
				}
			}
			
		}
		else if (next_event.GetEventType() == "end_service") {
			EndService(next_event.GetSchedulerID(), next_event.GetTimeStamp());		
			if (vDEBUG) {
				cout << "Event: End_service." << endl;
			}			
		}

		UpdateQueue(next_event, event_assigned, empty_list);
		//system("CLS");
		int perc_tot = (next_event.timestamp * 100) / tot_sim;
		cout << "t: \t" << next_event.timestamp <<"\t\t\t\t" << perc_tot << " % <-- completed";
		cout << "\t \t [ ";

		int perc_tracking = perc_tot / 10;
		if (perc_tracking > 10) {
			perc_tracking = 10;
		}
		for (int m = 0; m < perc_tracking; m++) {
			cout << "|";
		}
		for (int m = 0; m < 10 - perc_tracking; m++) {
			cout << "-";
		}
		cout << " ]" << "\r";
	
		event_calendar.DeleteEvent(next_event.GetEventID());		
		
	}

	Toc("end");

	PrintInfoQueue();
	PrintAvgReputation();
	PrintSchedulerItem();
	PrintUserInfo();
	// -> istante per istante valore di delta (avgRep) ??? HOW???
	cout << "Text file exported." << endl;
  	system("pause");
    return 0;
}
