using namespace std;
#include <iostream>
#include <random>
#include "SystemBehaviour.h"

int main() {	  
	cout << "Progetto SSIoT";
	
	int n_services, n_devices, n_master, lambda, tot_sim, seed;
    bool resource_ctrl, qos_ctrl;
    float alfa, beta, gamma;
    
	n_services		= 6;	
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
	//Service* list_of_services;
	list_of_services = ServicesCreation(n_services);
	
	Device *list_of_devices;
	list_of_devices = DeviceCreation(n_devices, n_services, list_of_services);

    return 0;
}


// return_vettore_di_servizi ...
