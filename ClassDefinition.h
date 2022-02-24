extern bool vDEBUG;
extern bool qoe_ctrl;
extern bool resource_ctrl;
#include <algorithm>

//#define INIT_FEED = 0.5;
const int INIT_NUM_FEEDBACK = 30;
const int INIT_STARTING_POINT = 27;
double refresh_rate = 1000; // ms

// Struct SECTION
struct Friend_Record {
	int friend_device_id; // id_dispositivo_amico
	string type_rel;
	double sociality_factor;
};

struct Reputation {
	int id_service_provider;	 
	int id_service_requester;
	int id_requested_service;
	int feedback; //TODO: vettore con timestamp per importanza nel tempo?	
	int num_feedback;
	double reputation_value;
};

struct Queue {
	int total_service_queued;
	int total_empty_list;
	int total_accomplished;
	double timestamp;
};

struct Trust_record {
	int id_service_provider;	
	int provider_class;
	double social_value;
	double rep_value;
	double trust_value;
};

struct RelationshipInformationNumber{
	int friends;
	int non_friends;
	vector<int> list_of_friend_indexes;
	vector<int> list_of_non_friend_indexes;
};

struct FriendsOfFriend {
	int friend_of_requester_id;
	int friend_of_friend_id;
};

struct DeltaTrace {
	double avgValue;
	double timestamp;
	int service_id;
};

struct NodesUnderThreshold {
	int id_requester;
	int id_provider; 
	int id_service;
	double trust_value;
	double timestamp;
	string type_rel;
};

struct ResourceMonitorTrace {
	double totalAvailableResources;
	double totalNetworkResources;
	double timestamp;
	double single_variation;
};

/*
Device GetDeviceByID(int id_device, Device* list_of_devices, int n_devices) {
	for (int j = 0; j < n_devices; j++) {
		if (list_of_devices[j].GetID() == id_device) {
			return list_of_devices[j];

		}
	}
};
*/

// Class SECTION
class Service
{
	int service_id;
	double power_cost;
	int cpu_req; //tolto power_cost -> [cycles/sec]
	
	public:
		Service(){
			this->service_id = 0;
			this->power_cost = 0;
			this->cpu_req 	= 0;
		};
		
		void SetService(int s_id, double power_c, int cpu_r){
			this->service_id 	= s_id;
			this->power_cost 	= power_c;
			this->cpu_req 		= cpu_r; 
		};
		
		int GetServiceId(){
			return this->service_id;
		}
		
		double GetPowerCost(){
			return this->power_cost;
		}
		
		int GetCpuReq(){
			return this->cpu_req;
		}
		
		void PrintService(){			
			cout << "ID Servizio:" << this->service_id;
			cout << " - Power cost: " << this->power_cost;
			cout << " - CPU Requirement: " << this->cpu_req << endl;
		}
	
};

class Device{	
	int device_id, id_owner, id_manufacturer, location, device_class, clock_speed;
	vector<Friend_Record> friend_list{};
	//Friend_Record* friend_list; 
	double total_power, remaining_power;
	vector<int> services_id_list;  // vettore di id -> elenco di servizi che può fare
	bool malicious_node;
	vector<DeltaTrace> delta_value = {};
	
	public: 
		vector<int> master_node_id_list;
				
		Device(){
			this->device_id 		= -1;			
			this->total_power 		= 0.0; 
			this->remaining_power	= 0.0;
			this->id_owner			= 0;
			this->id_manufacturer	= 0;
			this->location			= 0;
			this->device_class		= 0;
			this->clock_speed 		= 0; // gigacycles/s
			this->malicious_node	= false;
			
			vector<Friend_Record> empty_friend_list;
			this->friend_list = empty_friend_list;
			
			vector<int> empty_services_id_list;
			this->services_id_list = empty_services_id_list;	
			
			vector<int> empty_master_node_list;
			this->master_node_id_list = empty_master_node_list;
		};
		
		void GenerateDevice(int id_d, double tot_p, int id_o, int id_man, int loc, int dev_c, int clock_s){
			this->device_id 		= id_d;
			this->total_power 		= tot_p; 
			this->remaining_power	= tot_p;
			this->id_owner			= id_o;
			this->id_manufacturer	= id_man;
			this->location			= loc;
			this->device_class		= dev_c;
			this->clock_speed 		= clock_s;		

		};
		
		int GetID(){
			return this->device_id;
		}

		void SetID(int id_new) {
			this->device_id = id_new;
		}

		// new get
		int GetIDOwner() {
			return this->id_owner;
		}

		void SetIDOwner(int id_new) {
			this->id_owner = id_new;
		}

		int GetIDManufacturer() {
			return this->id_manufacturer;
		}
		void SetIDManufacturer(int id_new) {
			this->id_manufacturer = id_new;
		}

		int GetLocation() {
			return this->location;
		}

		void SetLocation(int new_location) {
			this->location = new_location;
		}


		int GetDeviceClass() {
			return this->device_class;
		}
		void SetDeviceClass(int new_class) {
			this->device_class = new_class;
		}

		int GetClockSpeed() {
			return this->clock_speed;
		}
		void SetClockSpeed(int new_clock) {
			this->clock_speed = new_clock;
		}

		double GetTotalPower() {
			return this->total_power;
		}

		void SetTotalPower(double new_tot_power) {
			this->total_power = new_tot_power;
		}

		double GetRemainingPower() {
			return this->remaining_power;
		}

		void SetRemainingPower(double new_rem_power) {
			this->remaining_power = new_rem_power;
		}
			
		void RemovePower(double power_to_remove) {
			this->remaining_power = this->remaining_power - power_to_remove;
		}

		vector<int> GetServiceIDList() {
			return this->services_id_list;
		}
			
		vector<int> GetMasterNodeIDList(){
			return this->master_node_id_list;
		}
			
		bool GetMalicious() {
			return this->malicious_node;
		}

		void SetMalicious(bool mal_node) {
			this->malicious_node = mal_node;
		}
	
//		SetFriendRecord(vector<Friend_Record> new_friend_records){//			
//			int length = new_friend_records.size();
//			vector<Friend_Record> empty_friend_list;
//			this->friend_list = empty_friend_list;			
//			for(int i=0;i<length;i++){
//				this->friend_list.push_back(new_friend_records[i]);
//			}		
//		};
		
		void AddFriendRecord(Friend_Record new_friend_records){
			this->friend_list.push_back(new_friend_records);
		}

		vector<Friend_Record> GetAllFriends() {
			return this->friend_list;
		}
		
		void SetServicesList(vector<int> new_service_list){		
			
			int length = new_service_list.size();
			vector<int> empty_services_id_list;
			this->services_id_list = empty_services_id_list;			
			for(int i=0;i<length;i++){
				this->services_id_list.push_back(new_service_list[i]);
			}
					};
		
		void SetMasterNodeList(vector<int> new_master_list){			
			int length = new_master_list.size();
			vector<int> empty_master_node_list;
			this->master_node_id_list = empty_master_node_list;
			
			for(int i=0;i<length;i++){		
				this->master_node_id_list.push_back(new_master_list[i]);
			}
		};

		void PrintDevice(){	
			cout << endl;
			cout << "## ID Device:" 		<< this->device_id;
			cout << " - ID owner: " 		<< this->id_owner;
			cout << " - ID manufacturer: " 	<< this->id_manufacturer;
			cout << " - Location: "			<< this->location;
			cout << " - Device class: "		<< this->device_class;
			cout << " - Clock speed: "		<< this->clock_speed;
			cout << " - Total power: "		<< this->total_power;
			cout << " - Remain. power: "	<< this->remaining_power;			
			
//			// print friend
//			cout << " - Friends list:";
//			for (Friend_Record i:this->friend_list){
//				cout <<" [" << i.type_rel<<":"<<i.friend_device_id<<":"<<i.sociality_factor<< "] -";
//			}
			
			// print Services
			cout << " - Services list:";
			for (int i:this->services_id_list){
				cout <<" [" << i<< "] -";
			}
			
			// print MasterNode				
			cout << " - Master node list:";
			for (int i:this->master_node_id_list){
				cout <<" [" << i<< "] -";
			}
			
			cout << " ##"<<endl;
		};

		void PushBackDeltaValue(DeltaTrace new_delta) {
			this->delta_value.push_back(new_delta);
		}
	
		vector <DeltaTrace> GetDeltaValue() {
			return this->delta_value;
		}
};

//struct Registered_Device{	
//	int device_id;
//	vector<Friend_Record> friend_info;
//};		
//	

class Master
{
	int master_id;	
	int location;
	vector<int> assigned_services;
	//vector<Registered_Device> registered_devices;
	vector<int> registered_devices;
	vector<Reputation> list_of_reputation;
	
	//https://en.cppreference.com/w/cpp/container/map
	
	
	public:
		
		Master(){
			vector<int> empty_list;
			vector<int> empty_dev_list;
			
			this->master_id = 0;
			this->location = 0;						
			this->assigned_services = empty_list;				
			this->registered_devices = empty_dev_list;			
		};
		
		void SetMaster(int m_id){
			this->master_id = m_id;
			this->location = m_id; // location == id SEMPRE			
		};
		
//		SetAssignedServices(vector<int> new_assigned_services){			
//			int length = new_assigned_services.size();
//			vector<int> empty_services_list;
//			this->assigned_services = empty_services_list;			
//			for(int i=0;i<length;i++){
//				this->assigned_services.push_back(new_assigned_services[i]);
//			}		
//		};

		void AddNewService(int id_new_service){
			this->assigned_services.push_back(id_new_service);
		}
		
		int GetID(){
			return this->master_id;
		}
		
		vector<int> GetAllServices(){
			return this->assigned_services;
		}
		
		void AddDevice(int new_devices){	
			this->registered_devices.push_back(new_devices);		
		}
		
		void RemoveDevice(int devices_to_delete){
				
			vector<int> temp_vect;	
			int index_to_delete = -1;
			
			for(unsigned int i=0;i<this->registered_devices.size();i++){
				if(this->registered_devices[i] == devices_to_delete){
					index_to_delete = i;
					break;
				}
			}
			this->registered_devices.erase(this->registered_devices.begin()+index_to_delete);		
		}

		vector<int> GetAllDevices() {
			return this->registered_devices;
		}
						
		/*Reputation GetReputation(int id_service_provider) {

			
			for (int i = 0;i<list_of_reputation.size();i++) {
				if (list_of_reputation[i].id_service_requester == id_service_provider) {
					return this->list_of_reputation[i];					
				}
			}
			

		}*/

		void SetReputation(int id_service_provider, int id_service_requester, int id_requested_service, int new_feed) {

			/*for (int i = 0; i < list_of_reputation.size(); i++) {
				if (list_of_reputation[i].id_service_requester == id_service_provider) {
					this->list_of_reputation[i] = reputation_swap;
				}
			}
			*/
			bool rep_found = false;

			for (unsigned int i = 0; i < this->list_of_reputation.size(); i++) {
				if(this->list_of_reputation[i].id_requested_service == id_requested_service){
					if (this->list_of_reputation[i].id_service_requester == id_service_requester) {
						if (this->list_of_reputation[i].id_service_provider == id_service_provider) {
							rep_found = true;
							this->list_of_reputation[i].num_feedback = this->list_of_reputation[i].num_feedback + 1;
							this->list_of_reputation[i].feedback = this->list_of_reputation[i].feedback + new_feed;
							this->list_of_reputation[i].reputation_value = double(this->list_of_reputation[i].feedback) / double(this->list_of_reputation[i].num_feedback);
						}
					}
				}
			}

			if (!rep_found) {
				Reputation new_rep_to_add{};
				new_rep_to_add.id_requested_service = id_requested_service;
				new_rep_to_add.id_service_requester = id_service_requester;
				new_rep_to_add.id_service_provider = id_service_provider;
				new_rep_to_add.feedback = INIT_STARTING_POINT + new_feed;
				new_rep_to_add.num_feedback = INIT_NUM_FEEDBACK + 1;
				new_rep_to_add.reputation_value = double(new_rep_to_add.feedback) / double(new_rep_to_add.num_feedback);
				this->list_of_reputation.push_back(new_rep_to_add);
			}
		}

		Reputation GetReputation(int id_service_requester, int id_service_provider, int id_requested_service) {
			Reputation rep_output{};
			bool rep_found = false;

			rep_output.reputation_value = -1;
			if (this->list_of_reputation.size() > 0) {
				for (unsigned int i=0; i < list_of_reputation.size(); i++) {
					if (this->list_of_reputation[i].id_service_requester == id_service_requester) {
						if (this->list_of_reputation[i].id_service_provider == id_service_provider) {
							if (this->list_of_reputation[i].id_requested_service = id_requested_service) {
								rep_output = this->list_of_reputation[i];
								rep_found = true;
							}

						}
					}
				}

			}
			if (!rep_found)
			{
				// inizializzare a zero la reputazione e poi restituirla
				rep_output.id_service_provider = id_service_provider;
				rep_output.id_service_requester = id_service_requester;
				rep_output.id_requested_service = id_requested_service; 
				rep_output.feedback = INIT_STARTING_POINT;
				rep_output.num_feedback = INIT_NUM_FEEDBACK;
				rep_output.reputation_value = double(rep_output.feedback)/double(rep_output.num_feedback);
				this->list_of_reputation.push_back(rep_output);				
			}

			return rep_output;
		}


		double AverageReputation(int id_service_provider, int id_requested_service) {
			double avgRep = 0.0;
			double totRep = 0;

			if (this->list_of_reputation.size() > 0) {
				for (unsigned int i = 0; i < this->list_of_reputation.size(); i++) {
					if (this->list_of_reputation[i].id_service_provider == id_service_provider) {
						if (this->list_of_reputation[i].id_requested_service == id_requested_service) {
							if (this->list_of_reputation[i].num_feedback > INIT_NUM_FEEDBACK) {
								totRep = totRep + 1;
								avgRep = avgRep + this->list_of_reputation[i].reputation_value;
							}						
						}
						
							
					}
					
				}
			}

			if (totRep > 0) {
				return avgRep / totRep;

			}
			else {
				return 0.9;
			}
		}
				
		void PrintMaster(){	
			cout << endl;
			cout << "ID Master node:" << this->master_id;
			cout << " - location: " << this->location << endl;
			//cout << " - CPU Requirement: " << this->cpu_req << endl;
			
			// print Registered devices				
			cout << " - Registered devices:";
			for (int i:this->registered_devices){
				cout <<" [" << i<< "] -";
			}
			cout << endl;
		}

		
		int GetDeviceIndexByID(int id_device, Device* list_of_devices, int n_devices) {
			Device device_to_return = Device();
			for (int v = 0; v < n_devices; v++) {
				if (list_of_devices[v].GetID() == id_device) {
					//device_to_return =  list_of_devices[v];
					return v;
				}
			}			
		};



		RelationshipInformationNumber GetRelationshipInformationNumber(int id_requester, int id_requested_service, Device* list_of_devices, int n_devices) {
			// Query master node to count how many friends a given device has
			// filtrato per servizio??? No, da fare
			RelationshipInformationNumber infos{};			
			infos.friends= 0;
			infos.non_friends = 0;
			

			//infos.list_of_friend_indexes			
			vector<Friend_Record> friends = list_of_devices[GetDeviceIndexByID(id_requester, list_of_devices, n_devices)].GetAllFriends();
						
			for (unsigned int i = 0; i < this->registered_devices.size(); i++) {

				bool isFriend = false;
				
				for (unsigned int j = 0; j < friends.size(); j++) {
					if (friends[j].friend_device_id == this->registered_devices[i]) {
						//Device friend_to_analyze = ;
						vector<int> services_list = list_of_devices[GetDeviceIndexByID(friends[j].friend_device_id, list_of_devices, n_devices)].GetServiceIDList();
						if (std::find(services_list.begin(), services_list.end(), id_requested_service) != services_list.end()) {
							isFriend = true;
							infos.list_of_friend_indexes.push_back(friends[j].friend_device_id);
							break;
						}
					}
				}				

				if (isFriend) {
					infos.friends++;

				}
				else {

					infos.non_friends++;
				}				
			}				

			for (unsigned int i = 0; i < this->registered_devices.size(); i++) {
				// se non è contenuto in vettore amico, aggiungo al vettore non amici				
				if (!(std::find(infos.list_of_friend_indexes.begin(), infos.list_of_friend_indexes.end(), this->registered_devices[i]) != infos.list_of_friend_indexes.end())) {
					infos.list_of_non_friend_indexes.push_back(this->registered_devices[i]);
				}

			}

			return infos;
		};
	
};

class Scheduler
{
	int id_action;
	double time_of_arrival;
	int service_requester;
	int requested_service;
	vector<int> service_providers_array;
	int handling_master_node;	
	int choosen_service_provider;
	double end_timestamp;
	int number_of_reschedule;
	double master_elaboration_time;
	//Master obj_master_node;
	vector<Trust_record> Trust_list;

public:
	Scheduler() {
		id_action = -1;
		time_of_arrival = -1;
		service_requester = -1;
		requested_service = -1;
		handling_master_node = -1;
		choosen_service_provider = -1;
		number_of_reschedule = 0;
		master_elaboration_time = 0;
		end_timestamp = -0.01;
	};

	void SetId(int new_id)
	{
		this->id_action = new_id;

	};

	int GetID()
	{
		return this->id_action;
	};

	void SetRescheduleTime(int r_time) {
		this->number_of_reschedule = r_time;
	};

	int GetRescheduleTime() {
		return this->number_of_reschedule;
	}

	void SetTOA(double new_toa)
	{
		this->time_of_arrival = new_toa;

	};

	double GetTOA()
	{
		return this->time_of_arrival;
	};

	void SetSR(int new_sr)
	{
		this->service_requester = new_sr;

	};

	int GetSR()
	{
		return this->service_requester;
	};

	void SetReqServ(int new_req_serv)
	{
		this->requested_service = new_req_serv;

	};

	int GetReqServ()
	{
		return this->requested_service;
	};

	void SetMaster(int new_master)
	{
		this->handling_master_node = new_master;

	};

	int GetMaster()
	{
		return this->handling_master_node;
	};

	void AddServiceProvider(int new_service_provider) {
		this->service_providers_array.push_back(new_service_provider);
	}

	void RemoveServiceProvider(int sp_to_delete) {

		vector<int> temp_vect;
		int index_to_delete = -1;

		for (unsigned int i = 0; i < this->service_providers_array.size(); i++) {
			if (this->service_providers_array[i] == sp_to_delete) {
				index_to_delete = i;
				break;
			}
		}
		this->service_providers_array.erase(this->service_providers_array.begin() + index_to_delete);
	}

	void SetTrustList(vector<Trust_record> new_list) {
		this->Trust_list = new_list;
	}


	vector<Trust_record> GetTrustList() {
		return this->Trust_list;
	}

	/*
		Master GetMasterObj() {
			return this->obj_master_node;
		}

		void SetMasterObj(Master obj_mn) {
			this->obj_master_node = obj_mn;
		}
	*/

	int GetChoosenSP()
	{
		return this->choosen_service_provider;
	};

	void SetChoosenSP(int new_csp)
	{
		this->choosen_service_provider = new_csp;

	};


	void SetEndTimestamp(double new_ts)
	{
		this->end_timestamp = new_ts;

	};

	double GetEndTimestamp()
	{
		return this->end_timestamp;
	};

	double GetMasterTime() {
		return this->master_elaboration_time;
	};

	void SetMasterTime(double time) {
		this->master_elaboration_time = time;
	}

};

class Event {
private:
	int event_id;
	int scheduler_id;	
	//Scheduler scheduler_item;
	bool deleted;
	string event_type;

public:
	double timestamp;

	Event() {
		this->event_id = -1;
		this->scheduler_id = -1;
		this->timestamp = 0.0;
		//this->scheduler_item = {};
		this->deleted = false;
	}

	void SetEventType(string e_t) {
		this->event_type = e_t;
	}

	string GetEventType() {
		return this->event_type;
	}

	void SetEventID(int e_id) {
		this->event_id = e_id;
	}

	int GetEventID() {
		return this->event_id;
	}

	void SetSchedulerID(int id) {
		this->scheduler_id = id;
	}

	int GetSchedulerID() {
		return this->scheduler_id;
	}

	void SetTimeStamp(double ts) {
		this->timestamp = ts;
	}

	double GetTimeStamp() {
		return this->timestamp;
	}
	
	/*
	void SetSchedulerItem(Scheduler sch_itm) {
		this->scheduler_item = sch_itm;
	}

	Scheduler GetSchedulerItem() {
		return this->scheduler_item;
	}
	*/

	void MarkAsDeleted() {
		this->deleted = true;
	}

	void UnMarkAsDeleted() {
		this->deleted = false;
	}

	bool IsDeleted() {
		return this->deleted;
	}

};

bool AscTimestamp_order(const Event& a, const Event& b)
{
	return a.timestamp < b.timestamp;
};

class Calendar {
private: 
	vector<Event> list_of_events;
public:
	Calendar(vector<Scheduler> list_of_events_to_handle) {
		//this->list_of_events = {};

		for (unsigned int i = 0; i < list_of_events_to_handle.size(); i++) {
			//list_of_events_to_handle[i]
			Event new_event_to_add = Event();
			new_event_to_add.SetEventID(i);
			new_event_to_add.SetEventType("scheduler");
			new_event_to_add.SetSchedulerID(list_of_events_to_handle[i].GetID());
			new_event_to_add.SetTimeStamp(list_of_events_to_handle[i].GetTOA());			
			this->list_of_events.push_back(new_event_to_add);
		}
	}

	void AddEvent(int scheduler_id, double timestamp, string event_type) {
		// riordino solo all'aggiunta di un evento? 		
		Event new_event_to_add = Event();
		new_event_to_add.SetEventID(list_of_events.size());
		new_event_to_add.SetEventType(event_type);
		new_event_to_add.SetSchedulerID(scheduler_id);
		new_event_to_add.SetTimeStamp(timestamp);				
		this->list_of_events.push_back(new_event_to_add);

		OrderEvents();
	}

	Event GetNextEvent() {		
		//OrderEvents(); // ordinamento e getto il primo ....
		for (unsigned int i = 0; i < this->list_of_events.size(); i++) {
			if (!list_of_events[i].IsDeleted()) {
				return this->list_of_events[i];
			}
		}	
		return {};
	}

	void OrderEvents() {
		// bisogna ordinare solo i non cancellati.
		std::sort(this->list_of_events.begin(), this->list_of_events.end(), AscTimestamp_order);		
	}

	void DeleteEvent(int event_id) {
		for (unsigned int i = 0; i < this->list_of_events.size(); i++) {
			//if (!list_of_events[i].IsDeleted()) {
			if(list_of_events[i].GetEventID() == event_id){
				this->list_of_events[i].MarkAsDeleted();
				this->list_of_events.erase(list_of_events.begin()+i); // TODO: check if it is needed
				break;
			}
		}
	}

	bool IsEmpty() {
		//int tot_element_deleted = 0;

		//for (unsigned int i = 0; i < this->list_of_events.size(); i++) {
		//	if ((list_of_events[i].IsDeleted())) {
		//		tot_element_deleted++;
		//	}			
		//}

		//if (this->list_of_events.size() != tot_element_deleted) {
		//	return false;
		//}
		//else {
		//	return true;
		//}

		if (this->list_of_events.size() > 0) {
			return false;
		}
		else {
			return true;
		}
	}	
};

class ResourceMonitor {
private:
	double totalAvailableResources;
	double totalNetworkResources;

	vector<ResourceMonitorTrace> resource_status;

public:
	ResourceMonitor() {
		this->totalAvailableResources = 0;
		this->totalNetworkResources = this->totalAvailableResources;
		resource_status = {};
	};

	ResourceMonitor(int n_devices, Device* list_of_devices) {
		this->totalAvailableResources = 0;
		for (int i = 0; i < n_devices; i++) {
			this->totalAvailableResources = this->totalAvailableResources + list_of_devices[i].GetTotalPower();
		}
		this->totalNetworkResources = this->totalAvailableResources;
		resource_status = {};
	};

	double GetTotalAvailRes() {
		return this->totalAvailableResources;
	};

	double GetTotalNetworkRes() {
		return this->totalNetworkResources;
	};

	void SetTotalAvailRes(double newres) {
		this->totalAvailableResources = newres;
	}

	void SetTotalNetworkRes(double newres) {
		this->totalNetworkResources = newres;
	}

	void AddTraceRecord(double timestamp, double service_requirement, string record_type) {
		ResourceMonitorTrace new_data;

		new_data.timestamp = timestamp;

		if (record_type == "Allocate") {
			new_data.totalAvailableResources = this->totalAvailableResources - service_requirement;		
			new_data.single_variation = -service_requirement;
		}else if(record_type == "Deallocate") {
			new_data.totalAvailableResources = this->totalAvailableResources + service_requirement;
			new_data.single_variation = +service_requirement;
		}	
		
		this->totalAvailableResources = new_data.totalAvailableResources;
		new_data.totalNetworkResources = this->totalNetworkResources;		
			
		this->resource_status.push_back(new_data);
	}

	
	vector<ResourceMonitorTrace> GetResourceMonitor() {
		return this->resource_status;
	}

};