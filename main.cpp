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

bool vDEBUG = true;
double alpha = 0.5;
double beta = 0.3;
double gamma = 0.2;
bool resource_ctrl;
bool qoe_ctrl;
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
	
	
	Tic();

	vector<bool>	parameter_to_test_resource_ctrl		= { true }; //{true, false};
	vector<bool>	parameter_to_test_qoe_ctrl			= { true };
	
	vector<int>		parameter_to_test_n_services		= { 6 };
	vector<int>		parameter_to_test_n_devices			= { 100 };
	vector<int>		parameter_to_test_n_master			= { 5 };

	vector<int>		parameter_to_test_lambda			= { 10 };
	vector<int>		parameter_to_test_seed				= { 1 };

	vector<int>		parameter_to_test_tot_sim			= { 100, 3000 };

	for (int nts = 0; nts < parameter_to_test_tot_sim.size(); nts++) {
		for (int rc = 0; rc < parameter_to_test_resource_ctrl.size(); rc++) {
			for (int qc = 0; qc < parameter_to_test_qoe_ctrl.size(); qc++) {

				for (int ns = 0; ns < parameter_to_test_n_services.size(); ns++) {
					for (int nd = 0; nd < parameter_to_test_n_devices.size(); nd++) {
						for (int nm = 0; nm < parameter_to_test_n_master.size(); nm++) {

							for (int nl = 0; nl < parameter_to_test_lambda.size(); nl++) {
								for (int nseed = 0; nseed < parameter_to_test_seed.size(); nseed++) {

									resource_ctrl = parameter_to_test_resource_ctrl[rc];
									qoe_ctrl = parameter_to_test_qoe_ctrl[qc];

									n_services = parameter_to_test_n_services[ns];
									n_devices = parameter_to_test_n_devices[nd];
									n_master = parameter_to_test_n_master[nm];

									lambda = parameter_to_test_lambda[nl];
									tot_sim = parameter_to_test_tot_sim[nts];	 // secondi
									seed = parameter_to_test_seed[nseed];

									// seed the RNG	
									std::mt19937 rng(seed); // mt19937: Pseudo-random number generation


									folder_name = "Sim-n_services_" + to_string(n_services) + "-n_devices_" + to_string(n_devices) + "-n_master_" + to_string(n_master) +
										"-lambda_" + to_string(lambda) + "-tot_sim_" + to_string(tot_sim) + "-seed_" + to_string(seed) + "-resource_ctrl_"
										+ to_string(resource_ctrl) + "-qoe_ctrl_" + to_string(qoe_ctrl) + "/";

									const char* folder_name_char = folder_name.c_str();
									if (_mkdir(folder_name_char) == -1) {
										cout << "Folder exists." << endl;
									}
									else {
										cout << "Folder created." << endl;
									}

									cout << "Start..." << endl;
									srand(seed);

									list_of_services = ServicesCreation();
									list_of_master = MasterCreation();
									list_of_devices = DeviceCreation();
									GenerateSocialRel(n_devices, list_of_devices);
									scheduler_records = GenerateEventsArray(tot_sim);

									Toc("scheduler");
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
												// de-allocate resources scheduling
												double processing_time = EstimateProcessingTime(next_event.GetSchedulerID()); // cycles/s -> bit/cycles (1/1000) ->  bit 				
												double new_timestamp = next_event.GetTimeStamp() + processing_time + total_master_processing;

												Toc("start add event");
												event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "end_service");
												Toc("end add event");

												if (vDEBUG) {
													cout << "Event: assigned." << endl;
												}
											}
											else if (event_assigned == -1) {
												// new service scheduling			
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

											}
											else if (event_assigned == -2) {
												// unwanted scenario												
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
										system("CLS");
										cout << "SSIoT Simulator. \nScenario:" << folder_name << endl;
										int perc_tot = (next_event.timestamp * 100) / tot_sim;
										cout << "t: \t" << next_event.timestamp << "\t\t\t\t" << perc_tot << " % ";
										if (info_queue.size() > 0) {
											cout << "\t queue: \t" << info_queue.back().total_service_queued << "\t";
										}
										else {
											cout << "\t queue: \t" << "0" << "\t";
										}
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

										cout << " ]";
										tend = clock();
										unsigned long diff = difftime(tend, tstart) / 1000;
										cout << "\nElapsed time: " << diff << " sec" << endl;
										if (perc_tot > 0) {
											cout << "\nEstimated time: " << (diff * 100 / perc_tot) << " sec" << endl;
										}
										//cout << "\r";

										event_calendar.DeleteEvent(next_event.GetEventID());									

									}


									cout << "\n";
									
									bool tempDebug = vDEBUG;
									vDEBUG = true;
									Toc("end");
									vDEBUG = tempDebug;
									PrintInfoQueue();
									PrintAvgReputation();
									PrintSchedulerItem();
									PrintUserInfo();
									PrintEstimateDeltaStateEachDevices();

									Toc("end");									
									cout << "\nText files exported. Folder:" << folder_name << endl;									
								}

							}
						}
					}
				}
			}
		}
	}
	
  	system("pause");
    return 0;
}
