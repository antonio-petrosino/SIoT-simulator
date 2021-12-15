using namespace std;
#include <iostream>
#include <random>
#include <array>
#include <vector>
#include "SystemBehaviour.h"

int main() {	  
	cout << "Progetto SSIoT"<<endl;
	
	int n_services, n_devices, n_master, lambda, tot_sim, seed;
    bool resource_ctrl, qos_ctrl;
    float alfa, beta, gamma;
    
	n_services		= 1;	
	n_devices 		= 150;	
	n_master 		= 1;
	lambda			= 5;
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

//	for(int i=0; i<n_master; i++){
//		list_of_master[i].PrintMaster();
//	}	
  	system("pause");

	// possibilità di calcolare quanto tempo effettivo impiega il master a calcolare tutto (poi vediamo)
	// si può fare

	// facciamo un oggetto "calendario" che itera 

    return 0;
}


// return_vettore_di_servizi ...
