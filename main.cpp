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
    
	n_services		= 10;	
	n_devices 		= 200;	
	n_master 		= 5;
	lambda			= 2;
	tot_sim			= 600;	
	seed			= 10;
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
	
	int a;
	

	for(int i=0; i<n_master; i++){
		list_of_master[i].PrintMaster();
	}
	
    return 0;
}


// return_vettore_di_servizi ...
