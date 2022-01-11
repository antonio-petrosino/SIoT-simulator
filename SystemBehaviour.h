#include <vector>
#include <algorithm>
#include "ClassDefinition.h"

#define INIT_FEED = 0.5;

/* DEFINE GLOBAL VARIABLE */
extern double alpha, beta, gamma;
extern Service* list_of_services;
extern Master* list_of_master;
extern Device* list_of_devices;
extern vector<Scheduler>  scheduler_records;
extern int n_services, n_devices, n_master, lambda, tot_sim, seed;


Service *ServicesCreation(){

    const float possible_power_cost[] = {0.1,0.1,0.1,0.2,0.2,0.2,0.3,0.3};
    const int possible_cpu_req[]      = {6, 6, 6, 10, 10, 10, 14, 14};
    
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
	
	Device* arrayOfDevice = new Device[n_devices];
	
	const float possible_total_power[] = {0.2,0.4,0.6,0.8};
	const int possible_class[] = {1,2,2,3};	
	const int possible_clock_speed[]={400, 1000, 1000, 2000};
	
	int length_pos_total_p = sizeof(possible_total_power)/ sizeof(float);
	
	const int possible_owner[] = {1,2,3,4,5,6,7,8,9,10};
	int length_pos_owner = sizeof(possible_owner)/ sizeof(int);
	
	const int length_pos_loc = n_master;
	vector<int> possible_location(length_pos_loc);
	
	for(int i=0;i<length_pos_loc; i++){
		possible_location[i] = i+1;
	}
				
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
	
		// SetServicesList
		const int possible_num_service_class[] = {1,1,1,1,1,1,1,1,1,2};	// 90% - 1 -- 10% - 2
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
				
		arrayOfDevice[i].SetServicesList(choosen_services); // assegno finalmente i servizi definiti

		
		for(int j=0;j<choosen_services.size();j++){		
				for(int k=0;k<n_master;k++){
					vector<int> service_to_find = list_of_master[k].GetAllServices();					
					for(int s=0;s<service_to_find.size();s++){						
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
	if (qos_ctrl) {
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
					new_social_rel.sociality_factor = 0.9;
					new_social_rel.type_rel = "POR";
					check_social = 1;
				}
				else if(defined_devices[i].GetIDOwner() == defined_devices[j].GetIDOwner()) {
					new_social_rel.sociality_factor = 0.8;
					new_social_rel.type_rel = "OOR";
					check_social = 1;
				}
				else if(defined_devices[i].GetLocation() == defined_devices[j].GetLocation()) {
					new_social_rel.sociality_factor = 0.6;
					new_social_rel.type_rel = "C-LOR";
					check_social = 1;			 						
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


vector<Scheduler> GenerateEventsArray(int sim_duration) {
	// seed the RNG	
	std::mt19937 rng(seed); // mt19937: Pseudo-random number generation
		
	//double lambda = static_cast<double>(1)/ averageArrival;

	std::exponential_distribution<double> exp(lambda);

   float sumArrivalTimes = 0;
   float newArrivalTime;

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
	  int selected_req_service = rand()% n_services;
	  record_to_push.SetReqServ(list_of_services[selected_req_service].GetServiceId()); // random su tutti i servizi
	  int selected_service_requester = rand() % n_devices;
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

void ServiceProviderFiltering(int id_scheduler_record) {
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

		// riempio service_requester
		for (int j = 0; j < n_devices; j++) {
			if (list_of_devices[j].GetID() == id_service_requester) {
				selected_service_requester = list_of_devices[j];
				break;
			}
		}

		vector<Friend_Record> friend_of_requester = selected_service_requester.GetAllFriends();

		RelationshipInformationNumber rel_info = selected_master.GetRelationshipInformationNumber(selected_service_requester.GetID(), id_requested_service, list_of_devices, n_devices);
				
		for (int j = 0; j < master_registered_devices_ids.size(); j++) {
			for (int k = 0; k < n_devices; k++) {				
				if (list_of_devices[k].GetID() == master_registered_devices_ids[j]) {
					selected_provider = list_of_devices[k];
					break;
				}
			}
			
			if (selected_provider.GetID() != -1) {

				vector<int> selected_services = selected_provider.GetServiceIDList();

				for (int k = 0; k < selected_services.size(); k++) {
					if (selected_services[k] == id_requested_service) {						
						
						// aggiungerò solo se la soglia va bene 
						// TODO: manca calcolo punteggio amici di amici					

						for (int i = 0; i < friend_of_requester.size(); i++) {
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
								for (int k = 0; k < rel_info.list_of_friend_indexes.size(); k++) {
									Reputation friend_rep_inspection{};
									friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_friend_indexes[k], selected_provider.GetID(), id_requested_service);
									if (friend_rep_inspection.feedback > 100) {
										sum_of_friend_rep = sum_of_friend_rep + friend_rep_inspection.reputation_value;
										number_of_friend_updated++;
									}
									
								}

								// rep_value dei non amici
								double sum_of_non_friend_rep = 0;
								int number_of_non_friend_updated = 0;
								for (int k = 0; k < rel_info.list_of_non_friend_indexes.size(); k++) {
									Reputation friend_rep_inspection{};
									friend_rep_inspection = selected_master.GetReputation(rel_info.list_of_non_friend_indexes[k], selected_provider.GetID(), id_requested_service);
									if(friend_rep_inspection.num_feedback > 100) {
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
								trust_value_to_add.trust_value = trust_value_to_add.social_value*trust_value_to_add.rep_value;
								Trust_list.push_back(trust_value_to_add);
								break;
							}
						}

						scheduler_records[id_scheduler_record].AddServiceProvider(selected_provider.GetID());
						break;
					}
				}

				
			}
			
		}
		//TODO: eventuale taglio sotto  una certa soglia (per reputation)
		std::sort(Trust_list.begin(), Trust_list.end(), CompareByTrustDesc);

		//QuS ordino solo dopo il cut
		scheduler_records[id_scheduler_record].SetTrustList(Trust_list);
		//return scheduler_record;
		
}

void AssignFeedback(Master* list_of_master, int n_master, int id_master, int id_service_provider, int id_service_requester, int id_requested_service, bool mal_behaviour){
	
	const int possible_mal_feedback[]	= { 0,0,0,0,0,1,1,1,1,1 }; // mal 50%
	const int possible_feedback[]		= { 1,1,1,1,1,1,1,1,1,0 }; // ben 90%
	int len_possible_feedback			= sizeof(possible_feedback) / sizeof(int);
	int choosenFeedback					= rand() % len_possible_feedback;
	int selected_master_id;
	int new_feed;

	if (mal_behaviour) {
		new_feed = possible_mal_feedback[choosenFeedback];
	}
	else {
		new_feed = possible_feedback[choosenFeedback];
	}


	for (int i = 0; i < n_master; i++) {
		if (list_of_master[i].GetID() == id_master) {
			selected_master_id = i;
		}

	}

	list_of_master[selected_master_id].SetReputation(id_service_provider, id_service_requester, id_requested_service, new_feed);

}

bool Orchestrator_MakeDecisions(int id_sched_event) {	
	vector<Trust_record> providers_ranking = scheduler_records[id_sched_event].GetTrustList();
	Master selected_master;
	Device selected_provider;
	Service selected_service;
	int service_id;
	int master_id = scheduler_records[id_sched_event].GetMaster();
	

	if (resource_ctrl) {
		// lofaremo
		// secondo in classifica ecc..ecc..
		// definire la soglia minima
	}
	else {
		
		for (int i = 0; i < n_master;i++) {
			if (list_of_master[i].GetID() == master_id) {
				selected_master = list_of_master[i];
				break;
			}
		}
		
		selected_provider = selected_master.GetDeviceByID(providers_ranking[0].id_service_provider,list_of_devices, n_devices);
		service_id = scheduler_records[id_sched_event].GetReqServ();
		
		for (int i = 0; i < n_services; i++) {
			if (list_of_services[i].GetServiceId() == service_id) {
				selected_service = list_of_services[i];
				break;
			}
		}

		bool allocation_success = selected_provider.AllocateDeviceResources(selected_service.GetPowerCost());
		if (allocation_success) {
			scheduler_records[id_sched_event].SetChoosenSP(selected_provider.GetID());
		}
			
		return allocation_success;
		
	}

	// ritornare falso se non l'ho assegnato
	// ritornare vero se ho un provider valido
	return true;
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
			if (list_of_devices[i].GetID() == service_id) {
				selected_provider = list_of_devices[i];
				break;
			}
		}


		double cyclespersec = selected_provider.GetClockSpeed();
		int bitperservice = selected_service.GetCpuReq();


		double time = (1000 * double(bitperservice)) / (cyclespersec);

		return time;
}

