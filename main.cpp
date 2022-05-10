using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <ctime>
#include <iomanip>
#include <string>  
#include <math.h>
#include "SystemBehaviour.h"
#include <direct.h>

bool vDEBUG = false;
bool resource_ctrl;
bool qoe_ctrl;
unsigned long tstart, tend;


//LAUNCHER: C:\Users\anton\source\repos\TestLibrerie
// 0 0 6 100 5 4 1 3000
// resource_ctrl, qoe_ctrl, n_services, n_devices,	n_master, lambda, seed,	tot_sim 
// 1 1 5 200 5 4 1 1000

// 1 1 5 150 5 1000 1 1000
// 1 1 1 100 1 20 1 200

Service* list_of_services;
Master* list_of_master;
Device* list_of_devices;
vector<Scheduler>  scheduler_records;
vector<Queue> info_queue;
vector<NodesUnderThreshold> detected_potential_malicious_devices;
int n_services, n_devices, n_master, lambda, tot_sim, seed;

string folder_name;
ResourceMonitor network_monitor;


int main(int argc, char* argv[]) {
	unsigned long iteration_tstart;
	int max_resched = 999;
	//cutting_value = 0.265; 	
	//cutting_value = 0.27;
	cout << "SSIoT Simulator"<<endl;	

	// parse argv
	if (argc < 8) {
		cout << "ERROR: Not enough argument !" << endl;
		cout << "Args <- " << argv << " -> End args" << endl;
		return -1;
	}

	if (stoi(argv[1]) == 0) {
		resource_ctrl = false;
	}
	else {
		resource_ctrl = true;
	}
	//resource_ctrl = (bool) argv[1];
	if (stoi(argv[2]) == 0) {
		qoe_ctrl = false;
	}
	else {
		qoe_ctrl = true;
	}
	
	n_services = stoi(argv[3]);
	n_devices = stoi(argv[4]);
	n_master = stoi(argv[5]);
	lambda = stoi(argv[6]);
	seed = stoi(argv[7]);
	tot_sim = stoi(argv[8]);

	unsigned long cmd_print_interval = clock();	

	Tic();
	vector<double>  processing_time_vector = {};
	double overall_processing_time = 0;
	int iteration_number = 0;
	int tot_number_of_simulations = 1;

	int numbero_di_perdite = 0;

	/*
	vector<bool>	parameter_to_test_resource_ctrl = { true, false }; // OK
	vector<bool>	parameter_to_test_qoe_ctrl = { false, true }; // OK

	vector<int>		parameter_to_test_n_services = { 6 }; // OK
	vector<int>		parameter_to_test_n_devices = { 200 }; // OK
	vector<int>		parameter_to_test_n_master = { 5 }; // OK

	vector<int>		parameter_to_test_lambda = { 15 }; // OK
	vector<int>		parameter_to_test_seed = { 1 };

	vector<int>		parameter_to_test_tot_sim = { 3000 }; // OK

	int tot_number_of_simulations = GetNumberOfSim(parameter_to_test_tot_sim, parameter_to_test_resource_ctrl, parameter_to_test_qoe_ctrl, parameter_to_test_n_services, parameter_to_test_n_devices, parameter_to_test_n_master, parameter_to_test_lambda, parameter_to_test_seed);
	int iteration_number = 0;

	for (int nts = 0; nts < parameter_to_test_tot_sim.size(); nts++) {
		for (int rc = 0; rc < parameter_to_test_resource_ctrl.size(); rc++) {			
			for (int qc = 0; qc < parameter_to_test_qoe_ctrl.size(); qc++) {
				// if resource control is off -> qoe cannot be enabled
				if (parameter_to_test_resource_ctrl[rc] == false && parameter_to_test_qoe_ctrl[qc] == true) {
					continue;
				}

				for (int ns = 0; ns < parameter_to_test_n_services.size(); ns++) {
					for (int nd = 0; nd < parameter_to_test_n_devices.size(); nd++) {
						for (int nm = 0; nm < parameter_to_test_n_master.size(); nm++) {

							for (int nl = 0; nl < parameter_to_test_lambda.size(); nl++) {
								for (int nseed = 0; nseed < parameter_to_test_seed.size(); nseed++) {
*/
									/*
									resource_ctrl = parameter_to_test_resource_ctrl[rc];
									qoe_ctrl = parameter_to_test_qoe_ctrl[qc];

									n_services = parameter_to_test_n_services[ns];
									n_devices = parameter_to_test_n_devices[nd];
									n_master = parameter_to_test_n_master[nm];

									lambda = parameter_to_test_lambda[nl];
									tot_sim = parameter_to_test_tot_sim[nts];	 // secondi
									seed = parameter_to_test_seed[nseed];
									*/
									iteration_number++;
									iteration_tstart = clock();
									processing_time_vector = {};
									overall_processing_time = 0;

									srand(seed);
									// seed the RNG	
									// std::mt19937 rng(seed); // mt19937: Pseudo-random number generation
									// TODO: rivedere seed

									folder_name = "Sim-n_services_" + to_string(n_services) + "-n_devices_" + to_string(n_devices) + "-n_master_" + to_string(n_master) +
										"-lambda_" + to_string(lambda) + "-tot_sim_" + to_string(tot_sim) + "-seed_" + to_string(seed) + "-resource_ctrl_"
										+ to_string(resource_ctrl) + "-qoe_ctrl_" + to_string(qoe_ctrl) + "/";

									const char* folder_name_char = folder_name.c_str();
									if (_mkdir(folder_name_char) == -1) {
										cout << "Folder exists." << endl;
										//return -1;
									}
									else {
										cout << "Folder created." << endl;
									}

									cout << "Start..." << endl;
									srand(seed);

									list_of_services = ServicesCreation();
									list_of_master = MasterCreation();
									list_of_devices = DeviceCreation();

									info_queue = {};
									detected_potential_malicious_devices = {};
									network_monitor = ResourceMonitor(n_devices, list_of_devices);


									GenerateSocialRel(n_devices, list_of_devices);
									scheduler_records = GenerateEventsArray(tot_sim, seed);

									Toc("scheduler");
									std::cout << std::setprecision(3) << std::fixed;

									Calendar event_calendar = Calendar(scheduler_records);


									// CAMARDA CHECK
									for (int counter = 0; counter < n_devices; counter++) {
										vector<Friend_Record> friend_count_check = list_of_devices[counter].GetAllFriends();

										std::cout << "Devices number: " << counter + 1;
										std::cout << " friends list length" << friend_count_check.size() << endl;
										int classe_uno = 0, classe_due = 0, classe_tre = 0, classe_unk = 0;
										
										for (int f_counter = 0; f_counter < friend_count_check.size(); f_counter++) {
											//cout << "------------------------\t Device ID \t";
											//cout << friend_count_check[f_counter].friend_device_id;

											Device device_to_return = Device();
											for (int v = 0; v < n_devices; v++) {
												if (list_of_devices[v].GetID() == friend_count_check[f_counter].friend_device_id) {
													//device_to_return =  list_of_devices[v];
													device_to_return = list_of_devices[v];
													break;
												}
											}
											//cout << " Device Class\t";
											//cout << device_to_return.GetDeviceClass() << endl; 
											if (device_to_return.GetDeviceClass() == 1)
											{
												classe_uno++;
											}
											else if (device_to_return.GetDeviceClass() == 2) {
												classe_due++;
											}
											else if (device_to_return.GetDeviceClass() == 3) {
												classe_tre++;
											}
											else {
												classe_unk++;
											}											
										}
										cout << "-------- classe uno " << classe_uno << "\tclasse due " << classe_due << "\tclasse tre " << classe_tre << "\tclasse unk " << classe_unk << endl;
	
									}

									system("pause");
									
									while (!event_calendar.IsEmpty()) {
										unsigned long start_master_processing = clock();
										Toc("init get next event");
										Event next_event = event_calendar.GetNextEvent();										
										Toc("end get next event");

										int event_assigned = false;
										bool empty_list = false;

										if (vDEBUG) {
											cout << "Sched[" << next_event.GetSchedulerID() << "]..." << endl;
										}

										if (next_event.GetEventType() == "scheduler" || next_event.GetEventType() == "re-scheduler") {
											//start_master_processing = clock();
											Toc("start ServiceProviderFiltering");
											ServiceProviderFiltering(next_event.GetSchedulerID(), next_event.GetTimeStamp());
											Toc("end ServiceProviderFiltering - start Orchestrator_MakeDecisions");

											event_assigned = Orchestrator_MakeDecisions(next_event.GetSchedulerID(), next_event.GetTimeStamp());
											Toc("end Orchestrator_MakeDecisions");
											unsigned long end_master_processing = clock();
											double total_master_processing = double(end_master_processing - start_master_processing) / 1000;

											scheduler_records[next_event.GetSchedulerID()].SetMasterTime(scheduler_records[next_event.GetSchedulerID()].GetMasterTime()+total_master_processing);

											//if (total_master_processing > 0){
												processing_time_vector.push_back(total_master_processing);
												overall_processing_time = overall_processing_time + total_master_processing;
											//}

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
												numbero_di_perdite++;
												if (scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() < max_resched) {
													scheduler_records[next_event.GetSchedulerID()].SetRescheduleTime(scheduler_records[next_event.GetSchedulerID()].GetRescheduleTime() + 1);

													//CAMARDA COMMENTATO PER PROBABILITA DI PERDITA
													//Toc("start add event");
													//event_calendar.AddEvent(next_event.GetSchedulerID(), new_timestamp, "re-scheduler");
													//Toc("end add event");

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

										if (clock() - cmd_print_interval > refresh_rate) {
											system("CLS");
											cout << "SSIoT Simulator" << "   Current iteration: "<< iteration_number << " / " << tot_number_of_simulations << endl;
											cout << "\nScenario:\n" << folder_name << endl;
											int perc_tot = (next_event.timestamp * 100) / tot_sim;
											cout << "t: \t" << next_event.timestamp << "\t\t\t\t" << perc_tot << " % ";
											if (info_queue.size() > 0) {
												cout << "\t perdita: \t" << numbero_di_perdite << "\t";
											}
											else {
												cout << "\t perdita: \t" << numbero_di_perdite << "\t";
											}
											cout << "\n[ ";

											int perc_tracking = perc_tot;
											if (perc_tracking > 100) {
												perc_tracking = 100;
											}
											for (int m = 0; m < perc_tracking; m++) {
												cout << "|";
											}
											for (int m = 0; m < 100 - perc_tracking; m++) {
												cout << "-";
											}

											cout << " ]";
											tend = clock();
											unsigned long diff = difftime(tend, iteration_tstart) / 1000;
											cout << "\n\nElapsed time: " << diff << " [sec]";
											if (perc_tot > 0) {
												cout << " =======>     Estimated time: " << (diff * 100 / perc_tot) << " [sec]";
											}
											else {
												cout << " =======>     Estimated time: TBD [sec]";
											}
											//cout << "\r";

											cout << "\nConsole refresh rate: 1 print each " << refresh_rate << " ms" << endl;
											cmd_print_interval = clock();
										}									

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
									PrintDetectedMalicious();
									PrintResourceMonitor();
									PrintProcessingTime(overall_processing_time, processing_time_vector);

									cout << "\nText files exported. Folder:" << folder_name << endl;									
									vDEBUG = true;
									Toc("text exported");
									vDEBUG = tempDebug;
/*
								}

							}
						}
					}
				}
			}
		}
	}
	*/
  	//system("pause");
    return 0;
}
