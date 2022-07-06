#include <vector>
#include <algorithm>
#include <ratio>
#include <chrono>
#include "ClassDefinition.h"
#include <fstream>



/* DEFINE GLOBAL VARIABLE */
//extern double alpha, beta, gamma;
extern Service* list_of_services;
extern Master* list_of_master;
extern Device* list_of_devices;
extern vector<Scheduler>  scheduler_records;
extern int n_services, n_devices, n_master, tot_sim, seed;
extern double lambda;
extern vector<Queue> info_queue;
extern unsigned long tstart;
extern unsigned long tend;
//extern double cutting_value;
extern string folder_name;
extern vector<NodesUnderThreshold> detected_potential_malicious_devices;
extern ResourceMonitor network_monitor;

void Tic() {
	//tstart = time(nullptr);
	if (vDEBUG) {
		tstart = clock();
	}
}

void Toc(string event) {
	if (vDEBUG) {
		tend = clock();
		unsigned long diff = difftime(tend, tstart);
		cout << "Until " << event << ": " << diff << " millisecond(s) or " << diff / 1000 << " second(s)." << endl;
	}

}


Service *ServicesCreation(){

	vector<double> possible_power_cost = {0.1, 0.1, 0.2, 0.2, 0.3, 0.3};
	//vector<double> possible_power_cost = { 0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.3, 0.3 };
	
	//vector<int> possible_cpu_req      = {600, 600, 600, 1000, 1000, 1000, 1400 , 1400 };
	vector<int> possible_cpu_req = { 600, 600, 1000, 1000, 1400 , 1400 };
    
    Service* arrayOfServices = new Service[n_services];    
    
    int length_possible = 6;
	int choosenIndex, randomCpuReq;
	double randomNumber;
	int choosenRND;

	//std::mt19937 rng(seed);
		
	for(int i=0; i<n_services; i++){	

		choosenRND = i;

		choosenIndex = choosenRND % length_possible;
		
		randomNumber = possible_power_cost[choosenIndex];
		randomCpuReq = possible_cpu_req[choosenIndex];
		
		// CAMARDA caso non generalizzato
		//randomNumber = possible_power_cost[5];
		//randomCpuReq = possible_cpu_req[5];

		arrayOfServices[i].SetService(i+1,randomNumber,randomCpuReq);
	}	
	
	

	if (vDEBUG) {
		cout << endl;
		cout << "List of generated services:" << endl;
		for (int i = 0; i < n_services; i++) {
			arrayOfServices[i].PrintService();
		}
	}
	cout << "#########################################" <<endl;
	
	return arrayOfServices;
}

Device* DeviceCreation(){

	//std::mt19937 rng(seed);
	Device* arrayOfDevice = new Device[n_devices];
	

	//double possible_total_power[] = {0.2,0.4,0.6,0.8};
	double possible_total_power[] = { 0.6,0.8 }; //CAMARDA per isolare solo 1 servizio
	
	
	int length_pos_total_p = sizeof(possible_total_power) / sizeof(double);

	//int possible_class[] = {1,2,2,3};	
	int possible_class[] = { 2,3 }; //CAMARDA per isolare solo 1 servizio
	
	

	//int possible_clock_speed[]={400*1000, 1000 * 1000, 1000 * 1000, 2000 * 1000 }; // megacycles
	//int possible_clock_speed[] = { 40 * 1000, 1000 * 1000, 1000 * 1000, 2000 * 1000 }; // megacycles
	int possible_clock_speed[] = { 1000 * 1000, 2000 * 1000 }; // megacycles
	
	int possible_malicious[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
	int length_pos_malicious = sizeof(possible_malicious) / sizeof(int);

	int possible_man[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int length_pos_man = sizeof(possible_man) / sizeof(int);

	//int possible_owner[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
	vector<int> possible_owner;
	int perc_of_owner = ceil(n_devices * 16 / 100);

	for (int x = 0; x <= perc_of_owner; x++)
		possible_owner.push_back(x+1);

	//CAMARDA
	//for (int x = 0; x < 2; x++)
	//	possible_owner.push_back(x + 1);

	//int length_pos_owner = sizeof(possible_owner) / sizeof(int);
	int length_pos_owner = possible_owner.size();

	int length_pos_loc = n_master;	
	
	vector<int> possible_location(length_pos_loc);
	
	for(int i=0;i<length_pos_loc; i++){
		possible_location[i] = i+1;
	}
				
	int choosenIndex, id_owner, id_man, loc, d_class, clock_s;
	double total_power;
	
	for(int i=0; i<n_devices; i++){

		choosenIndex = rand() % length_pos_total_p;
		
		//CAMARDA
		//choosenIndex = (i % length_pos_total_p);
		total_power = possible_total_power[choosenIndex];
		
		if (total_power > 0.5) 
		{
			int possible_high_class[] = {2,2,2,3,3,3,3,3,3,3};	
			int length_high_class = sizeof(possible_high_class)/ sizeof(int);
			choosenIndex = rand() % length_high_class;
			d_class = possible_high_class[choosenIndex];
						
			if (d_class == 3){
				//clock_s = possible_clock_speed[3];
				clock_s = possible_clock_speed[1];
			}else{
				//clock_s = possible_clock_speed[2];
				clock_s = possible_clock_speed[0];
			}
			
		}else{
			d_class = possible_class[choosenIndex];
			clock_s = possible_clock_speed[choosenIndex];
		}
		
		choosenIndex = rand() % length_pos_owner;
		id_owner = possible_owner[choosenIndex];
		
		choosenIndex = rand() % length_pos_man;
		id_man = possible_man[choosenIndex];
		
		choosenIndex = rand() % length_pos_loc;
		loc = possible_location[choosenIndex];

		//CAMARDA PER PROBABILITA DI PERDITA
		//id_owner = (i % length_pos_owner) + 1;
		//id_man= (i % (length_pos_owner/2)) + 1;
		//loc = (i % (length_pos_owner / 4)) + 1;
		//loc = 1;
		
		//CAMARDA check location

		arrayOfDevice[i].GenerateDevice(i+1,total_power, id_owner, id_man, loc, d_class, clock_s);
	
		// SetServicesList
		int possible_num_service_class[] = {1,1,1,1,1,1,1,1,1,2};	// 90% - 1 -- 10% - 2
		int length_possible_num_service_class = sizeof(possible_num_service_class)/ sizeof(int);	
		choosenIndex = rand() % length_possible_num_service_class;
					
		vector<int> suitable_services; // elenco di id
		
		for(int j=0;j<n_services;j++){
			if(arrayOfDevice[i].GetTotalPower() >= (list_of_services[j].GetPowerCost() * 2))
			{ 
				suitable_services.push_back(list_of_services[j].GetServiceId());											
			}
		}
					
		vector<int> choosen_services;
		int choosenSuitableService;
				
		for(int j=0;j<possible_num_service_class[choosenIndex];j++){			
		
		    if(suitable_services.size() > 0){			
		    choosenSuitableService = rand() % suitable_services.size();
			choosen_services.push_back(suitable_services[choosenSuitableService]);			
			//suitable_services.esase(suitable_services.begin(), suitable_services.end(), suitable_services[choosenSuitableService], suitable_services.end());
			suitable_services.erase(suitable_services.begin() + choosenSuitableService);
		    }
		}
		int choosenMalIndex = rand() % length_pos_malicious;

		if (possible_malicious[choosenMalIndex] == 1) {
			arrayOfDevice[i].SetMalicious(true);
		}	

		arrayOfDevice[i].SetServicesList(choosen_services); // assegno finalmente i servizi definiti

		
		for(unsigned int j=0;j<choosen_services.size();j++){
				for(int k=0;k<n_master;k++){
					vector<int> service_to_find = list_of_master[k].GetAllServices();					
					for(unsigned int s=0;s<service_to_find.size();s++){
						if(service_to_find[s] == choosen_services[j]){	
												
//							Registered_Device new_device_to_add; 
//							new_device_to_add.device_id = arrayOfDevice[i].device_id;					
//							vector<Friend_Record> empty_friend_list;
//							new_device_to_add.friend_info = empty_friend_list;
							
							list_of_master[k].AddDevice(arrayOfDevice[i].GetID());
							arrayOfDevice[i].master_node_id_list.push_back(list_of_master[k].GetID());
						}
					}					
				}	
		}
		// SetMasterNodeList

	}
	
	// SetFriendRecord // va fatta alla fine del processo di creazione
	if (vDEBUG) {
		for (int i = 0; i < n_devices; i++) {
			arrayOfDevice[i].PrintDevice();
		}
	}

	return arrayOfDevice;
}

bool CompareByTrustDesc(const Trust_record& a, const Trust_record& b)
{
	if (qoe_ctrl) {
		if (a.provider_class != b.provider_class) {
			return a.provider_class > b.provider_class;
		}
		else {
			return a.trust_value > b.trust_value;
		}
	}
	else {
		return a.trust_value > b.trust_value;
	}

}

void GenerateSocialRel(int n_devices, Device *defined_devices){
	
	Friend_Record new_social_rel{};
	int check_social = 0;
			
			
	for(int i=0;i<n_devices;i++){
		for(int j=0;j<n_devices;j++){
			check_social = 0;
			
			if(defined_devices[i].GetID() != defined_devices[j].GetID()){
				if(defined_devices[i].GetIDManufacturer() == defined_devices[j].GetIDManufacturer()) {
					new_social_rel.sociality_factor = 0.7; // TODO 0.9
					new_social_rel.type_rel = "POR";
					check_social = 1;
				}
				else if(defined_devices[i].GetIDOwner() == defined_devices[j].GetIDOwner()) {
					new_social_rel.sociality_factor = 0.65; // TODO 0.8
					new_social_rel.type_rel = "OOR";
					check_social = 1;
				}
				else if(defined_devices[i].GetLocation() == defined_devices[j].GetLocation()) {
					new_social_rel.sociality_factor = 0.6;
					new_social_rel.sociality_factor = 0.0; // CAMARDA
					new_social_rel.type_rel = "C-LOR";
					//check_social = 1;
					check_social = -1;			 						
				}
			}
			
			if(check_social>0){
				new_social_rel.friend_device_id = defined_devices[j].GetID();
				defined_devices[i].AddFriendRecord(new_social_rel);
			}
			
		}
	}
}

Master* MasterCreation(){
	//std::mt19937 rng(seed);
	Master* arrayOfMaster = new Master[n_master];
	
	for(int i=0;i<n_master;i++){ 
		arrayOfMaster[i].SetMaster(i+1); // anche location				
	}
	
	int given_master_id = 0;	
	for(int i=0;i<n_services;i++){							
		arrayOfMaster[i % n_master].AddNewService(list_of_services[i].GetServiceId());
		
	}
		
//	for(int i=0; i<n_master; i++){
//		arrayOfMaster[i].PrintMaster();
//	}

	return arrayOfMaster;
}

vector<Scheduler> GenerateEventsArray(int sim_duration, int seed) {
	// seed the RNG	
	std::mt19937 rng(seed); // mt19937: Pseudo-random number generation

	//CAMARDA
	//std::random_device  rand_dev;
	std::mt19937 generator(seed);
	std::uniform_int_distribution<int>  distr(0, n_devices);
		
	//double lambda = static_cast<double>(1)/ averageArrival;

	std::exponential_distribution<double> exp(lambda);

   double sumArrivalTimes = 0;
   double newArrivalTime;

   //vector<float> eventArray;

   vector<Scheduler> scheduler_records;

	//for (int i = 0; i < sim_duration; ++i)
   int i = 0;
   while(sumArrivalTimes<= sim_duration)
	 {
	  
	  newArrivalTime = exp.operator() (rng);// generates the next random number in the distribution 	  
	  sumArrivalTimes = sumArrivalTimes + newArrivalTime;
	  //eventArray.push_back(sumArrivalTimes);
	  Scheduler record_to_push = Scheduler();
	  record_to_push.SetId(i);
	  record_to_push.SetTOA(sumArrivalTimes);
	  // int selected_req_service = rand()% n_services;
	  // 
	  // CAMARDA
	  //int selected_service_requester = rand() % n_devices ;
	  int selected_service_requester = distr(generator) % n_devices;
	 // cout << "Selected service requester per verifica uniformemente distr: " << selected_service_requester << endl;
	  
	  //CAMARDA
	  int selected_req_service = 0;
	  //int selected_service_requester = 0;

	  record_to_push.SetReqServ(list_of_services[selected_req_service].GetServiceId()); // random su tutti i servizi
	  
	  record_to_push.SetSR(list_of_devices[selected_service_requester].GetID()); // random su tutti gli utenti
	  
	  //assign master node having record_to_push.GetReqServ()
	  for (int j = 0; j < n_master; j++) {		  
		  vector<int> master_services = list_of_master[j].GetAllServices();
		  if(std::find(master_services.begin(), master_services.end(), record_to_push.GetReqServ()) != master_services.end()){
			  record_to_push.SetMaster(list_of_master[j].GetID());
		  }
		  
	  }

	  //std::cout << "newArrivalTime:  " << newArrivalTime << "    ,sumArrivalTimes:  " << sumArrivalTimes << std::endl;
	  scheduler_records.push_back(record_to_push);
	  i++;
	  
	 }
   cout << "Eventi totali: " << i << endl;

	return scheduler_records;
}

bool contains(int src, vector<Friend_Record> subjects)
{
	auto iter = std::find_if(subjects.begin(), subjects.end(),
		[&](const Friend_Record& ts) {return ts.friend_device_id == src; });
	return iter != subjects.end();
}

void ServiceProviderFiltering(int id_scheduler_record, double current_timestamp) {
// Filtra in base al servizio scelto -> Master node -> provider registrati -> filtro solo su un servizio
		
		int id_handling_master = scheduler_records[id_scheduler_record].GetMaster();

		Master selected_master;
		Device selected_provider;
		Device selected_service_requester; 
		vector<Trust_record> Trust_list; // {id_provider, valore} 

		int id_service_requester = scheduler_records[id_scheduler_record].GetSR();
		int id_requested_service = scheduler_records[id_scheduler_record].GetReqServ();


		for (int j = 0; j < n_master; j++) {

			if (list_of_master[j].GetID() == id_handling_master) {
				selected_master = list_of_master[j];				
				break;
			}

		}

		vector<int> master_registered_devices_ids = selected_master.GetAllDevices();


		//selected_service_requester = list_of_devices[]
		// riempio service_requester
		for (int j = 0; j < n_devices; j++) {
			if (list_of_devices[j].GetID() == id_service_requester) {
				selected_service_requester = list_of_devices[j];
				break;
			}
		}

		vector<Friend_Record> friend_of_requester = selected_service_requester.GetAllFriends();

		RelationshipInformationNumber rel_info = selected_master.GetRelationshipInformationNumber(selected_service_requester.GetID(), id_requested_service, list_of_devices, n_devices);
		
		// no amici? ->
		Toc("start rel_info.friends");
		if (rel_info.friends != 0) {
			for (unsigned int j = 0; j < master_registered_devices_ids.size(); j++) {
				for (int k = 0; k < n_devices; k++) {
					if (list_of_devices[k].GetID() == master_registered_devices_ids[j]) {
						selected_provider = list_of_devices[k];
						break;
					}
				}

				if (selected_provider.GetID() != -1) {

					vector<int> selected_services = selected_provider.GetServiceIDList();

					for (unsigned int k = 0; k < selected_services.size(); k++) {
 						if (selected_services[k] == id_requested_service) {

							// TODO1: se è vuota??

							for (unsigned int i = 0; i < friend_of_requester.size(); i++) {
								if (friend_of_requester[i].friend_device_id == selected_provider.GetID()) {

									Trust_record trust_value_to_add{};
									trust_value_to_add.id_service_provider = friend_of_requester[i].friend_device_id;
									trust_value_to_add.social_value = friend_of_requester[i].sociality_factor;
									trust_value_to_add.provider_class = selected_provider.GetDeviceClass();
									
									Reputation rep_inspection{};
									rep_inspection = selected_master.GetReputation(selected_service_requester.GetID(), selected_provider.GetID(), id_requested_service);

									double direct_reputation = rep_inspection.reputation_value;	  // ottengo la rep_value diretta

									// rep_value dagli amici 
									double sum_of_friend_rep = 0;
									int number_of_friend_updated = 0;
									for (unsigned int k = 0; k < rel_info.list_of_friend_indexes.size(); k++) {
										Reputation friend_rep_inspection{};
										friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_friend_indexes[k], selected_provider.GetID(), id_requested_service);
										if (friend_rep_inspection.feedback > INIT_NUM_FEEDBACK) {
											sum_of_friend_rep = sum_of_friend_rep + friend_rep_inspection.reputation_value;
											number_of_friend_updated++;
										}

									}

									// rep_value dei non amici
									double sum_of_non_friend_rep = 0;
									int number_of_non_friend_updated = 0;
									for (unsigned int k = 0; k < rel_info.list_of_non_friend_indexes.size(); k++) {
										Reputation friend_rep_inspection{};
										friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_non_friend_indexes[k], selected_provider.GetID(), id_requested_service);
										if (friend_rep_inspection.num_feedback > INIT_NUM_FEEDBACK) {
											sum_of_non_friend_rep = sum_of_non_friend_rep + friend_rep_inspection.reputation_value;
											number_of_non_friend_updated++;
										}
									}

									double rep_results = alpha * direct_reputation;

									if (number_of_friend_updated > 0) {
										rep_results = rep_results + ((beta / double(number_of_friend_updated)) * sum_of_friend_rep);
									}
									if (number_of_non_friend_updated > 0) {
										rep_results = rep_results + ((gamma / double(number_of_non_friend_updated)) * sum_of_non_friend_rep);
									}

									trust_value_to_add.rep_value = rep_results;
									trust_value_to_add.trust_value = trust_value_to_add.social_value * trust_value_to_add.rep_value;

									// SOGLIA
									// double avgRep = selected_master.AverageReputation(selected_provider.GetID(), id_requested_service);
									// TODO: ripristinare taglio soglia
									
									if (trust_value_to_add.trust_value >= cutting_value) {
										Trust_list.push_back(trust_value_to_add);
									}
									else {
										NodesUnderThreshold new_detected_node; 
										new_detected_node.id_provider = trust_value_to_add.id_service_provider;
										new_detected_node.id_requester = id_service_requester;
										new_detected_node.id_service = id_requested_service;
										new_detected_node.timestamp = current_timestamp;
										new_detected_node.trust_value = trust_value_to_add.trust_value;
										new_detected_node.type_rel = "friend";
										detected_potential_malicious_devices.push_back(new_detected_node);
									}
									break;
								}
							}

							scheduler_records[id_scheduler_record].AddServiceProvider(selected_provider.GetID());
							break;
						}
					}


				}

			}
		}
		Toc("end rel_info.friends");

		if(rel_info.friends == 0 || Trust_list.size() == 0){
			// elenco degli amici del requester -> friend_of_requester
			// per ogni amico devo ottenere un vettore di amici di amici		
			Device temp_device_to_analyze; 
			vector<FriendsOfFriend> friends_of_friend_info;
			vector<int> friend_of_friend_over_master_id;
			// per ogni amico
			for (unsigned int i = 0; i < friend_of_requester.size(); i++) {
				for (int j = 0; j < n_devices; j++) {
					if (list_of_devices[j].GetID() == friend_of_requester[i].friend_device_id) {
						temp_device_to_analyze = list_of_devices[j];
						break;
					} // carico oggetto amico 
				}

				vector<Friend_Record> friends_of_friend_record = temp_device_to_analyze.GetAllFriends();
				// carico amici dell'amico
				// e seleziono solo quelli presenti sul master

				for (unsigned int k = 0; k < friends_of_friend_record.size(); k++) {
					if (std::find(master_registered_devices_ids.begin(), master_registered_devices_ids.end(), friends_of_friend_record[k].friend_device_id) != master_registered_devices_ids.end()) {
						FriendsOfFriend new_info; 
						new_info.friend_of_friend_id = friends_of_friend_record[k].friend_device_id;
						new_info.friend_of_requester_id = temp_device_to_analyze.GetID();
						friends_of_friend_info.push_back(new_info);
						if (!(std::find(friend_of_friend_over_master_id.begin(), friend_of_friend_over_master_id.end(), friends_of_friend_record[k].friend_device_id) != friend_of_friend_over_master_id.end())) {
							friend_of_friend_over_master_id.push_back(friends_of_friend_record[k].friend_device_id);
						}
						
					} 
				}
			}
			
			for (unsigned i = 0; i < friend_of_friend_over_master_id.size(); i++) {
				// calcolo numero di amici in comune
				int number_of_fof = 0;
				for (unsigned j = 0; j < friends_of_friend_info.size();j++) {
					if (friends_of_friend_info[j].friend_of_friend_id == friend_of_friend_over_master_id[i]) {
						number_of_fof = number_of_fof + 1;
					}
				}

				Device friend_to_analyze;
				for (int j = 0; j < n_devices; j++) {
					if (list_of_devices[j].GetID() == friend_of_friend_over_master_id[i]) {
						friend_to_analyze = list_of_devices[j];
						break;
					} // carico oggetto amico 
				}

				Trust_record trust_value_to_add{};
				trust_value_to_add.id_service_provider = friend_to_analyze.GetID();

				if (number_of_fof < (n_devices * 5 / 100)) {
					trust_value_to_add.social_value = 0.3;
				}
				else {
					trust_value_to_add.social_value = 0.55; // TODO: da ripristinare
					trust_value_to_add.social_value = 0.59;
				}

				trust_value_to_add.provider_class = friend_to_analyze.GetDeviceClass();

				/*
				*  CAMBIARE SOGGETTI PER OTTENERE LA SECONDA PARTE
				*/
				
				Reputation rep_inspection{};
				rep_inspection = selected_master.GetReputation(selected_service_requester.GetID(), friend_to_analyze.GetID(), id_requested_service);

				

				double direct_reputation = rep_inspection.reputation_value;	  // ottengo la rep_value diretta

				// rep_value: valutazione del provider j dagli amici di chi ha fatto la richiesta
				// sicuramente non ciclerà poichè amici nel master == 0
				double sum_of_friend_rep = 0;
				int number_of_friend_updated = 0;
				for (unsigned int k = 0; k < rel_info.list_of_friend_indexes.size(); k++) {
					Reputation friend_rep_inspection{};
					friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_friend_indexes[k], friend_to_analyze.GetID(), id_requested_service);
					if (friend_rep_inspection.feedback > INIT_NUM_FEEDBACK) {
						sum_of_friend_rep = sum_of_friend_rep + friend_rep_inspection.reputation_value;
						number_of_friend_updated++;
					}

				}

				// rep_value dei non amici
				double sum_of_non_friend_rep = 0;
				int number_of_non_friend_updated = 0;
				for (unsigned int k = 0; k < rel_info.list_of_non_friend_indexes.size(); k++) {
					Reputation friend_rep_inspection{};
					friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_non_friend_indexes[k], friend_to_analyze.GetID(), id_requested_service);
					if (friend_rep_inspection.num_feedback > INIT_NUM_FEEDBACK) {
						sum_of_non_friend_rep = sum_of_non_friend_rep + friend_rep_inspection.reputation_value;
						number_of_non_friend_updated++;
					}
				}

				double rep_results = alpha * direct_reputation;

				if (number_of_friend_updated > 0) {
					rep_results = rep_results + ((beta / double(number_of_friend_updated)) * sum_of_friend_rep);
				}
				if (number_of_non_friend_updated > 0) {
					rep_results = rep_results + ((gamma / double(number_of_non_friend_updated)) * sum_of_non_friend_rep);
				}

				trust_value_to_add.rep_value = rep_results;
				trust_value_to_add.trust_value = trust_value_to_add.social_value * trust_value_to_add.rep_value;

				// SOGLIA
				double avgRep = selected_master.AverageReputation(selected_provider.GetID(), id_requested_service);
				// TODO: ripristinare taglio soglia

				if (trust_value_to_add.trust_value >= cutting_value) {
					Trust_list.push_back(trust_value_to_add);
				}
				else {
					NodesUnderThreshold new_detected_node;
					new_detected_node.id_provider = trust_value_to_add.id_service_provider;
					new_detected_node.id_requester = id_service_requester;
					new_detected_node.id_service = id_requested_service;
					new_detected_node.timestamp = current_timestamp;
					new_detected_node.trust_value = trust_value_to_add.trust_value;
					new_detected_node.type_rel = "fof";
					detected_potential_malicious_devices.push_back(new_detected_node);
				}
				
											
				//double avgRep = selected_master.AverageReputation(friend_to_analyze.GetID(), id_requested_service);
				//trust_value_to_add.rep_value = avgRep;
				//trust_value_to_add.trust_value = trust_value_to_add.social_value * trust_value_to_add.rep_value;
				//Trust_list.push_back(trust_value_to_add);
			}

			if (vDEBUG) {
				cout << "No friends on master list." << endl;
			}
		}
		if (Trust_list.size() == 0) {
			cout << "Empty list: define an approach to handle it!!!" << endl;
		}
		else {
			std::sort(Trust_list.begin(), Trust_list.end(), CompareByTrustDesc);
			//QoE ordino solo dopo il cut
			scheduler_records[id_scheduler_record].SetTrustList(Trust_list);
		}
		Toc("end rel_info.fof-friends");

		
		
		
}

void AssignFeedback(int id_master, int id_service_provider, int id_service_requester, int id_requested_service, bool mal_behaviour, double timestamp) {

	int possible_mal_feedback[] = { 0,0,0,0,0,1,1,1,1,1 }; // mal 50%
	int possible_feedback[] = { 1,1,1,1,1,1,1,1,1,0 }; // ben 90%
	int len_possible_feedback = sizeof(possible_feedback) / sizeof(int);
	int choosenFeedback = rand() % len_possible_feedback;
	int selected_master_id;
	int new_feed;

	if (mal_behaviour) {
		new_feed = possible_mal_feedback[choosenFeedback];
	}
	else {
		new_feed = possible_feedback[choosenFeedback];
	}

	//CAMARDA se non deve avere i feed negativi per le perdite
	new_feed = 1;

	for (int i = 0; i < n_master; i++) {
		if (list_of_master[i].GetID() == id_master) {
			selected_master_id = i;
		}

	}

	list_of_master[selected_master_id].SetReputation(id_service_provider, id_service_requester, id_requested_service, new_feed);

	DeltaTrace new_delta;
	new_delta.avgValue = list_of_master[selected_master_id].AverageReputation(id_service_provider, id_requested_service);
	new_delta.timestamp = timestamp;
	new_delta.service_id = id_requested_service;

	for (int i = 0; i < n_devices; i++) {
		if (list_of_devices[i].GetID() == id_service_provider) {
			list_of_devices[i].PushBackDeltaValue(new_delta);
			break;
		}
	}

	

}

bool AllocateDeviceResources(double service_power, int device_index) {
	// return true only if the device can handle the service
	if (service_power > 0) {
		if (list_of_devices[device_index].GetRemainingPower() - service_power > 0) {
			list_of_devices[device_index].SetRemainingPower(list_of_devices[device_index].GetRemainingPower() - service_power);

			//double a = this->GetTotalPower() - this->GetRemainingPower();
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}

}

void ReleaseDeviceResources(double service_power, int device_index) {
	if (list_of_devices[device_index].GetRemainingPower() != list_of_devices[device_index].GetTotalPower()) {
		list_of_devices[device_index].SetRemainingPower(list_of_devices[device_index].GetRemainingPower() + service_power);
	}
	else {
		cout << "ERRORE!" << endl;
		//system("pause");
	}

}

int Orchestrator_MakeDecisions(int id_sched_event, double ts) {	
	vector<Trust_record> providers_ranking = scheduler_records[id_sched_event].GetTrustList();

	Master selected_master;
	//Device selected_provider;
	Service selected_service;
	bool allocation_success = false;

	int service_id = scheduler_records[id_sched_event].GetReqServ();
	for (int i = 0; i < n_services; i++) {
		if (list_of_services[i].GetServiceId() == service_id) {
			selected_service = list_of_services[i];
			break;
		}
	}

	int master_id = scheduler_records[id_sched_event].GetMaster();
	for (int i = 0; i < n_master; i++) {
		if (list_of_master[i].GetID() == master_id) {
			selected_master = list_of_master[i];
			break;
		}
	}

	if (providers_ranking.size() < 1) {
		return -2;
	}

	if (resource_ctrl) {

		for (unsigned int i = 0; i < providers_ranking.size(); i++) {
			//selected_provider = selected_master.GetDeviceByID(providers_ranking[i].id_service_provider, list_of_devices, n_devices);
			//allocation_success = list_of_devices[].



			allocation_success = AllocateDeviceResources(selected_service.GetPowerCost(), selected_master.GetDeviceIndexByID(providers_ranking[i].id_service_provider, list_of_devices, n_devices));

			if (allocation_success) {
				scheduler_records[id_sched_event].SetChoosenSP(providers_ranking[i].id_service_provider);

				network_monitor.AddTraceRecord(ts, selected_service.GetPowerCost(), "Allocate");

				return 1;
			}
		}	
	}
	else {				
		//selected_provider = 			
		allocation_success = AllocateDeviceResources(selected_service.GetPowerCost(), selected_master.GetDeviceIndexByID(providers_ranking[0].id_service_provider, list_of_devices, n_devices));

		if (allocation_success) {
			scheduler_records[id_sched_event].SetChoosenSP(providers_ranking[0].id_service_provider);
			network_monitor.AddTraceRecord(ts, selected_service.GetPowerCost(), "Allocate");
			return 1;
		}
		else {
			return -1;
		}		
	}

	return -1;
}

double EstimateProcessingTime(int id_sched_event) {
	Master selected_master;
	Device selected_provider;
	Service selected_service;
	int provider_id;
	int service_id;
	int master_id = scheduler_records[id_sched_event].GetMaster();


		for (int i = 0; i < n_master; i++) {
			if (list_of_master[i].GetID() == master_id) {
				selected_master = list_of_master[i];
				break;
			}
		}

		provider_id = scheduler_records[id_sched_event].GetChoosenSP();
		service_id = scheduler_records[id_sched_event].GetReqServ();

		for (int i = 0; i < n_services; i++) {
			if (list_of_services[i].GetServiceId() == service_id) {
				selected_service = list_of_services[i];
				break;
			}
		}

		for (int i = 0; i < n_devices; i++) {
			if (list_of_devices[i].GetID() == provider_id) {
				selected_provider = list_of_devices[i];
				break;
			}
		}


		double cyclespersec = selected_provider.GetClockSpeed();
		int bitperservice = selected_service.GetCpuReq();


		double time = (1000 * double(bitperservice)) / ((cyclespersec));

		return time;
}

void EndService(int id_sched_event, double end_ts) {
	// riassegnazione risorse 
	Master selected_master;
	Device selected_provider;
	Service selected_service;
	//int provider_id;	

	int master_id = scheduler_records[id_sched_event].GetMaster();
	int requester_id = scheduler_records[id_sched_event].GetSR();
	int service_id = scheduler_records[id_sched_event].GetReqServ();

	for (int i = 0; i < n_services; i++) {
		if (list_of_services[i].GetServiceId() == service_id) {
			selected_service = list_of_services[i];
			break;
		}
	}
		
	for (int i = 0; i < n_master; i++) {
		if (list_of_master[i].GetID() == master_id) {
			selected_master = list_of_master[i];
			break;
		}
	}
	scheduler_records[id_sched_event].SetEndTimestamp(end_ts);
	ReleaseDeviceResources(selected_service.GetPowerCost(), selected_master.GetDeviceIndexByID(scheduler_records[id_sched_event].GetChoosenSP(), list_of_devices, n_devices));


	network_monitor.AddTraceRecord(end_ts, selected_service.GetPowerCost(), "Deallocate");
	// TODO: assegno ai nodi malevoli, feed negativi
	// rilascio feedback
	// selected_provider.GetID()
	bool mal_node = list_of_devices[selected_master.GetDeviceIndexByID(scheduler_records[id_sched_event].GetChoosenSP(), list_of_devices, n_devices)].GetMalicious();
	AssignFeedback(master_id, scheduler_records[id_sched_event].GetChoosenSP(), requester_id, service_id, mal_node, end_ts);
}

void UpdateQueue(Event next_event, int event_assigned, bool empty_list) {
	int prev_total;
	int empty_prev_total;
	int prev_accomplished;
	int prev_perdita;

	if (info_queue.size() > 0) {
		Queue prev_queue_variation = info_queue[info_queue.size() - 1];		
		prev_total = prev_queue_variation.total_service_queued;
		empty_prev_total = prev_queue_variation.total_empty_list;
		prev_accomplished = prev_queue_variation.total_accomplished;
		prev_perdita = prev_queue_variation.totale_perdita; // CAMARDA

		if (next_event.GetTimeStamp() < prev_queue_variation.timestamp) {
			cout << "WARNING: info_queue logical error." << endl;
			//system("pause");
		}
	}
	else {
		prev_total = 0;
		empty_prev_total = 0;
		prev_accomplished = 0;
		prev_perdita = 0; // CAMARDA
	}

	Queue queue_variation = {};

	if (next_event.GetEventType() == "re-scheduler" && event_assigned == 1) {						
		queue_variation.total_service_queued = prev_total - 1; // coda --, timestamp
		queue_variation.timestamp = next_event.GetTimeStamp();
		queue_variation.total_empty_list = empty_prev_total;
		queue_variation.total_accomplished = prev_accomplished;
		queue_variation.totale_perdita = prev_perdita;
		info_queue.push_back(queue_variation);
	}
	else if (next_event.GetEventType() == "scheduler" && event_assigned == -1)
	{	
		queue_variation.total_service_queued = prev_total + 1; // coda ++, timestamp
		queue_variation.timestamp = next_event.GetTimeStamp();
		queue_variation.total_empty_list = empty_prev_total;
		queue_variation.total_accomplished = prev_accomplished;
		queue_variation.totale_perdita = prev_perdita + 1; // CAMARDA
		info_queue.push_back(queue_variation);
	}
	else if (next_event.GetEventType() == "end_service") {
		queue_variation.total_service_queued = prev_total;
		queue_variation.timestamp = next_event.GetTimeStamp();
		queue_variation.total_empty_list = empty_prev_total;
		queue_variation.total_accomplished = prev_accomplished + 1;
		queue_variation.totale_perdita = prev_perdita; // CAMARDA
		info_queue.push_back(queue_variation);
	}
	else if (empty_list) {
		queue_variation.total_service_queued = prev_total;
		queue_variation.timestamp = next_event.GetTimeStamp();
		queue_variation.total_empty_list = empty_prev_total + 1;
		queue_variation.total_accomplished = prev_accomplished;
		queue_variation.totale_perdita = prev_perdita; // CAMARDA
		info_queue.push_back(queue_variation);
	}
	else {
		queue_variation.total_service_queued = prev_total;
		queue_variation.timestamp = next_event.GetTimeStamp();
		queue_variation.total_empty_list = empty_prev_total;
		queue_variation.total_accomplished = prev_accomplished;
		queue_variation.totale_perdita = prev_perdita; // CAMARDA
		info_queue.push_back(queue_variation);
	}

}



void PrintInfoQueue() {
	ofstream myfile(".\\"+ folder_name + "InfoQueue.txt");
	if (myfile.is_open())
	{
		myfile << "total_service_queued" << "\t" << "total_empty_list" << "\t" << "total_accomplished" << "\t" << "totale_perdita\t" << "timestamp" << "\n";

		for (unsigned int i = 0; i < info_queue.size(); i++) {
			myfile << info_queue[i].total_service_queued << "\t" << info_queue[i].total_empty_list << "\t" << info_queue[i].total_accomplished << "\t" << info_queue[i].totale_perdita << "\t" << info_queue[i].timestamp << "\n";
		}

		//CAMARDA
		int indice_print_prob_perdita = info_queue.size()-10;
		double prob_perdita = (double)info_queue[indice_print_prob_perdita].totale_perdita / ((double)info_queue[indice_print_prob_perdita].total_accomplished + (double)info_queue[indice_print_prob_perdita].totale_perdita);
		cout << "Prob_perdita: " <<  prob_perdita << endl;
		//indice_print_prob_perdita = info_queue.size() - 10;
		double traffico_perso = (double)info_queue[indice_print_prob_perdita].totale_perdita / (double)info_queue[indice_print_prob_perdita].timestamp;
		cout << "Traffico perso: " << traffico_perso <<endl;
		myfile.close();
	}
	else cout << "Unable to open file";
}

void PrintSchedulerItem(){
	ofstream myfile(".\\" + folder_name + "SchedInfo.txt");
	if (myfile.is_open())
	{
		myfile << "id_action\t" << "time_of_arrival\t" << "service_requester\t" << "requested_service\t";
		myfile << "handling_master_node\t" << "choosen_service_provider\t" << "end_timestamp\t" << "elaboration_time\t" << "number_of_reschedule\t" << "\n";

		for (unsigned int i = 0; i < scheduler_records.size(); i++) {
			myfile << "%%%%%%%%%%%%%%%% SCHEDULER ITEM(" << i <<")%%%%%%%%%%%%%%%%%%%%%%%%%%%%" << "\n";
			myfile << scheduler_records[i].GetID() << "\t";
			myfile << scheduler_records[i].GetTOA() << "\t";
			myfile << scheduler_records[i].GetSR() << "\t";
			myfile << scheduler_records[i].GetReqServ() << "\t";
			myfile << scheduler_records[i].GetMaster() << "\t";
			myfile << scheduler_records[i].GetChoosenSP() << "\t";
			myfile << scheduler_records[i].GetEndTimestamp() << "\t";
			myfile << scheduler_records[i].GetMasterTime() << "\t";
			myfile << scheduler_records[i].GetRescheduleTime() << "\t\n";
			myfile << "############## Trust list ###############" << "\n";
			myfile << "id_service_provider\t" << "provider_class\t" << "social_value\t" << "rep_value\t" << "trust_value\t" << "\n";

			vector<Trust_record> trust_list = scheduler_records[i].GetTrustList();

			for (unsigned int j = 0; j < trust_list.size(); j++) {
				myfile << trust_list[j].id_service_provider << "\t";
				myfile << trust_list[j].provider_class << "\t";
				myfile << trust_list[j].social_value << "\t";
				myfile << trust_list[j].rep_value << "\t";
				myfile << trust_list[j].trust_value << "\t";
				myfile << "\n";
			}
			myfile << "#########################################" << "\n";
		}

		myfile.close();

	}
	else cout << "Unable to open file";
}

void PrintAvgReputation() {

	//for ogni master
	//	for ogni nodo
	//		for ogni servizio 
	ofstream myfile(".\\" + folder_name + "AvgRepInfo.txt");
	if (myfile.is_open())
	{
		myfile << "NodeID\t" << "ServiceID\t" << "AvgRep\t" << "\n";
		Master selected_master;
		for (int i = 0; i < n_master; i++) {
			selected_master = list_of_master[i];

			vector<int> devices_on_master_i = selected_master.GetAllDevices();

			for (unsigned int j = 0; j < devices_on_master_i.size(); j++) {
				Device device_to_analyze;
				for (int k = 0; k < n_devices; k++) {
					if (list_of_devices[k].GetID() == devices_on_master_i[j]) {
						device_to_analyze = list_of_devices[k];
						vector<int> list_of_services_of_device_k = device_to_analyze.GetServiceIDList();
						for (unsigned int z = 0; z < list_of_services_of_device_k.size(); z++) {
							double avgToWriteOnFile = selected_master.AverageReputation(device_to_analyze.GetID(), list_of_services_of_device_k[z]);
							myfile << device_to_analyze.GetID() << "\t" << list_of_services_of_device_k[z] << "\t" << avgToWriteOnFile << "\n";
						}
						break;
					}

				}
			}
		}
		myfile.close();

	}
	else cout << "Unable to open file";


}

void PrintUserInfo() {
	ofstream myfile(".\\" + folder_name + "1.UserInfo.txt");
	if (myfile.is_open())
	{
		myfile << "device_id\t";
		myfile << "id_owner\t";
		myfile << "id_manufacturer\t";
		myfile << "location\t";
		myfile << "device_class\t";
		myfile << "clock_speed\t";
		myfile << "total_power\t";
		myfile << "remaining_power\t";
		myfile << "malicious_node\t";
		myfile << "services_id_list\t\n";

		for (int i = 0; i < n_devices; i++) {
			
			myfile << list_of_devices[i].GetID() << "\t";
			myfile << list_of_devices[i].GetIDOwner() << "\t";
			myfile << list_of_devices[i].GetIDManufacturer() << "\t";
			myfile << list_of_devices[i].GetLocation() << "\t";
			myfile << list_of_devices[i].GetDeviceClass() << "\t";
			myfile << list_of_devices[i].GetClockSpeed() << "\t";
			myfile << list_of_devices[i].GetTotalPower() << "\t";
			myfile << list_of_devices[i].GetRemainingPower() << "\t";
			myfile << list_of_devices[i].GetMalicious() << "\t";
			vector<int> service_of_user = list_of_devices[i].GetServiceIDList();
			myfile << "[ ";
			for (int j = 0; j < service_of_user.size(); j++) {
				if (service_of_user.size() != (j + 1)) {
					myfile << service_of_user[j] << ", ";
				}
				else {
					myfile << service_of_user[j];
				}
			}
			myfile << " ]" << "\t";
			myfile << "" << "\t";
			myfile << "\n";
			
		}

		myfile.close();

	}
	else cout << "Unable to open file";
}

void PrintDetectedMalicious() {
	ofstream myfile(".\\" + folder_name + "DetectedMaliciousNodes.txt");
	if (myfile.is_open())
	{
		myfile << "id_requester\t";
		myfile << "id_provider\t";
		myfile << "id_service\t";
		myfile << "trust_value\t";
		myfile << "type_rel\t";
		myfile << "timestamp\t\n";

		for (int i = 0; i < detected_potential_malicious_devices.size(); i++) {

			myfile << detected_potential_malicious_devices[i].id_requester << "\t";
			myfile << detected_potential_malicious_devices[i].id_provider << "\t";
			myfile << detected_potential_malicious_devices[i].id_service << "\t";
			myfile << detected_potential_malicious_devices[i].trust_value << "\t";
			myfile << detected_potential_malicious_devices[i].type_rel << "\t";
			myfile << detected_potential_malicious_devices[i].timestamp << "\t";
			myfile << "\n";

		}

		myfile.close();

	}
	else cout << "Unable to open file";
}

void EstimateDeltaStateEachDevices(double timestamp) {
	Master selected_master;
	for (int i = 0; i < n_master; i++) {
		selected_master = list_of_master[i];
		vector < int > devices_on_master = selected_master.GetAllDevices();
		vector < int > services_on_master = selected_master.GetAllServices();
		for (int z = 0; z < services_on_master.size(); z++) {
			for (int j = 0; j < devices_on_master.size(); j++) {
				int k_index = 0;
				//for (int k = 0; k < n_devices; k++) {
				//if (devices_on_master[j] == list_of_devices[k].GetID()) {							
				k_index = devices_on_master[j] - 1;
				//	break;
				//}

				DeltaTrace new_delta;
				new_delta.avgValue = selected_master.AverageReputation(devices_on_master[j], services_on_master[z]);
				new_delta.timestamp = timestamp;
				new_delta.service_id = services_on_master[z];

				list_of_devices[k_index].PushBackDeltaValue(new_delta);

				//myfile << list_of_devices[k_index].GetID() <<"\t" << services_on_master[z] << "\t" << avgRep;
				//myfile << "\n";
				//}
			}

		}
	}
}

void PrintEstimateDeltaStateEachDevices() {
	ofstream myfile(".\\" + folder_name + "DeltaState.txt");

	if (myfile.is_open()) {
		Master selected_master;
		for (int i = 0; i < n_master; i++) {
			selected_master = list_of_master[i];
			vector < int > devices_on_master = selected_master.GetAllDevices();
			vector < int > services_on_master = selected_master.GetAllServices();
			for (int z = 0; z < services_on_master.size(); z++) {
				for (int j = 0; j < devices_on_master.size(); j++) {
					int k_index = 0;
					//for (int k = 0; k < n_devices; k++) {
					//if (devices_on_master[j] == list_of_devices[k].GetID()) {							
					k_index = devices_on_master[j] - 1;
					//	break;
					//}
					vector < DeltaTrace > list_of_delta = list_of_devices[k_index].GetDeltaValue();
					myfile << "USER INFO \t ID\t" << devices_on_master[j] << "\t SERVICE ID\t" << services_on_master[z] << "\n";
					myfile << "DELTA INFO\n";
					myfile << "AVGVALUE\tTIMESTAMP\n";

					for (int k = 0; k < list_of_delta.size(); k++) {
						if (list_of_delta[k].service_id == services_on_master[z]) {
							myfile << list_of_delta[k].avgValue << "\t";
							myfile << list_of_delta[k].timestamp << "\n";
						}
					}
					myfile << "END DELTA INFO\n";
					myfile << "END USER INFO\n";

					//myfile << list_of_devices[k_index].GetID() <<"\t" << services_on_master[z] << "\t" << avgRep;
					//myfile << "\n";
					//}
				}

			}
		}
		myfile.close();

	}
	else {
		cout << "Unable to open file";
	}
};

int GetNumberOfSim(vector<int> parameter_to_test_tot_sim, vector<bool> parameter_to_test_resource_ctrl, vector<bool> parameter_to_test_qoe_ctrl, vector<int> parameter_to_test_n_services, vector<int> parameter_to_test_n_devices, vector<int> parameter_to_test_n_master, vector<int> parameter_to_test_lambda, vector<int> parameter_to_test_seed) {
	int totsim = 0;
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
									totsim++;
								}
							}
						}
					}
				}
			}
		}
	}

	return totsim;
}

void PrintResourceMonitor() {

	ofstream myfile(".\\" + folder_name + "ResourceMonitor.txt");


	if (myfile.is_open()) {
		myfile << "totalAvailableResources\ttotalNetworkResources\ttimestamp\tsingle_variation\n";

		vector<ResourceMonitorTrace> data_to_print = network_monitor.GetResourceMonitor();

		for (int i = 0; i<data_to_print.size(); i++) {
			myfile << data_to_print[i].totalAvailableResources << "\t";
			myfile << data_to_print[i].totalNetworkResources << "\t";
			myfile << data_to_print[i].timestamp << "\t";
			myfile << data_to_print[i].single_variation << "\t\n";
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}
};

void PrintProcessingTime(double overall_processing_time, vector<double> processing_time_vector) {

	ofstream myfile(".\\" + folder_name + "ProcessingTime.txt");

	double mean_processing_time = overall_processing_time / processing_time_vector.size();
	cout << "\nText files exported. Folder:" << folder_name << endl;

	if (myfile.is_open()) {
		myfile << "Average delay:\t" << mean_processing_time << "\tTotal spent time for the processing: \t" << overall_processing_time;
		myfile << "\n------------------single processing time ---------------------\n";

		for (int i = 0; i < processing_time_vector.size(); i++) {
			myfile << processing_time_vector[i] << "\t";
		}
		myfile << "\n END";
	}
	else {
		cout << "Unable to open file";
	}
}