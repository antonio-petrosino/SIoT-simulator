#include "ClassDefinition.h"

Service* ServicesCreation(int n_services){

    const float possible_power_cost[] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
    const int possible_cpu_req[]      = {6  ,6  ,  6, 10, 10, 10, 14, 14};
    
    Service* arrayOfServices = new Service[n_services];    
    
    int length_possible = sizeof(possible_power_cost)/ sizeof(float);    

	int choosenIndex, randomCpuReq, temp_power_cost, temp_cpu_req;
	float randomNumber;
	
		
	for(int i=0; i<n_services; i++){	
		choosenIndex = rand() % length_possible;
		
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


Device* DeviceCreation(int n_devices){
	
	Device* arrayOfDevice = new Device[n_devices];
	
	const float possible_total_power[] = {0.2,0.4,0.6,0.8};
	const int possible_class[] = {1,2,2,3};	
	const int possible_clock_speed[]={400, 1000, 1000, 2000};
	
	int length_pos_total_p = sizeof(possible_total_power)/ sizeof(float);
	
	const int possible_owner[] = {1,2,3,4,5,6,7,8,9,10};
	int length_pos_owner = sizeof(possible_owner)/ sizeof(int);
	
	const int possible_location[] = {1,2,3,4,5};
	int length_pos_loc = sizeof(possible_location)/ sizeof(int);
	
	int choosenIndex, id_owner, id_man, loc, d_class, clock_s;
	float total_power;
	
	for(int i=0; i<n_devices; i++){

		choosenIndex = rand() % length_pos_total_p;
		total_power = possible_total_power[choosenIndex];
		
		if (total_power > 0.5) 
		{
			int possible_high_class[] = {2,2,2,3,3,3,3,3,3,3};	
			int length_high_class = sizeof(possible_high_class)/ sizeof(int);
			choosenIndex = rand() % length_high_class;
			d_class = possible_high_class[choosenIndex];
						
			if (d_class == 3){
				clock_s = possible_clock_speed[3];
			}else{
				clock_s = possible_clock_speed[2];
			}
			
		}else{
			d_class = possible_class[choosenIndex];
			clock_s = possible_clock_speed[choosenIndex];
		}
		
		choosenIndex = rand() % length_pos_owner;
		id_owner = possible_owner[choosenIndex];
		
		choosenIndex = rand() % length_pos_owner;
		id_man = possible_owner[choosenIndex];
		
		choosenIndex = rand() % length_pos_loc;
		loc = possible_location[choosenIndex];
		
		arrayOfDevice[i].GenerateDevice(i+1,total_power, id_owner, id_man, loc, d_class, clock_s);
	
		// SetFriendRecord
		// SetServicesList
		// SetMasterNodeList
	}
	
	for(int i=0; i<n_devices; i++){
		arrayOfDevice[i].PrintDevice();
	}

    
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
