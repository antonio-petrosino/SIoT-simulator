#include "ClassDefinition.h"

Service* ServicesCreation(int n_services, int seed){
/**
	n_services = numero totale di servizi
*/	
	Service* arrayOfServices = new Service[n_services];
    srand(seed);            
    float possible_power_cost[] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
    int possible_cpu_req[] 		= {6,6,6,10,10,10,14,14};
    int length = sizeof(possible_power_cost)/ sizeof(float);
    

	int choosenIndex = 0;
	float randomNumber = 0;
	int randomCpuReq = 0;
	int temp_power_cost = 0;
	int temp_cpu_req = 0;
		
	for(int i=0; i<n_services; i++){	
		choosenIndex = rand() % length;
		
		randomNumber = possible_power_cost[choosenIndex];
		randomCpuReq = possible_cpu_req[choosenIndex];
		
		arrayOfServices[i].SetService(i+1,randomNumber,randomCpuReq);
	}	
	
	cout << endl;
	cout << "List of generated services:" <<endl;
	for(int i=0; i<n_services; i++){
		arrayOfServices[i].PrintService();
	}
	
	cout << "#########################################" <<endl;
	
	return arrayOfServices;
}


/*

Master* ServicesCreation(int n_services, int seed){

	n_services = numero totale di servizi
	
	Master* arrayOfServices = new Service[n_services];
    srand(seed);            
    float possible_power_cost[] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
    int possible_cpu_req[] 		= {6,6,6,10,10,10,14,14};
    int length = sizeof(possible_power_cost)/ sizeof(float);
    

	int choosenIndex = 0;
	float randomNumber = 0;
	int randomCpuReq = 0;
	int temp_power_cost = 0;
	int temp_cpu_req = 0;
		
	for(int i=0; i<n_services; i++){	
		choosenIndex = rand() % length;
		
		randomNumber = possible_power_cost[choosenIndex];
		randomCpuReq = possible_cpu_req[choosenIndex];
		
		arrayOfServices[i].SetService(i+1,randomNumber,randomCpuReq);
	}	
	
	cout << endl;
	cout << "List of generated services:" <<endl;
	for(int i=0; i<n_services; i++){
		arrayOfServices[i].PrintService();
	}
	
	cout << "#########################################" <<endl;
	
	return arrayOfServices;
}
*/
