using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include <ctime>

#include "SystemBehaviour.h"

#define INIT_FEED = 0.5;

bool vDEBUG = false;
time_t tstart, tend;




int main() {	  
	cout << "Progetto SSIoT"<<endl;
	
	int n_services, n_devices, n_master, lambda, tot_sim, seed;
	bool resource_ctrl, qos_ctrl;
    float alfa, beta, gamma;

	tstart = time(0);

	n_services		= 6;	
	n_devices 		= 500;	
	n_master 		= 5;

	lambda			= 2;
	tot_sim			= 1000;	 // secondi
	seed			= 19;

	alfa 			= 0.5;
	beta 			= 0.3;
	gamma 			= 0.2;	

	resource_ctrl 	= true;
	qos_ctrl 		= true;
		
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


	for (int i = 0; i < scheduler_records.size(); i++) {
		scheduler_records[i] = ServiceProviderFiltering(scheduler_records[i], list_of_services, n_services, list_of_devices, n_devices, list_of_master, n_master, seed);		
	}
	//	for(int i=0; i<n_master; i++){
	//		list_of_master[i].PrintMaster();
	//	}	
	// 
	tend = time(0);
	cout << "Until provider threshold: " << difftime(tend, tstart) << " second(s)." << endl;

  	system("pause");

	// possibilità di calcolare quanto tempo effettivo impiega il master a calcolare tutto (poi vediamo)
	// si può fare

	// facciamo un oggetto "calendario" che itera 

    return 0;
}


// return_vettore_di_servizi ...
