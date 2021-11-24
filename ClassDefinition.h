class Service
{
	int service_id;
	double power_cost;
	int cpu_req; //tolto power_cost -> [cycles/sec]
	
	public:
		Service(){
			this->service_id = 0;
			this->power_cost = 0.0;
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

struct Friend_Record{	
	int friend_device_id; // id_dispositivo_amico
	string type_rel;	
	double sociality_factor;	
};

class Device{
	
	
	public: 
		int device_id, id_owner, id_manufacturer, location, device_class, clock_speed;
		vector<Friend_Record> friend_list{};
		//Friend_Record* friend_list; 
		double total_power, remaining_power;
		vector<int> services_id_list;  // vettore di id -> elenco di servizi che può fare
		vector<int> master_node_id_list;
				
		Device(){
			this->device_id 		= 0;
			
			this->total_power 		= 0.0; 
			this->remaining_power	= 0.0;
			this->id_owner			= 0;
			this->id_manufacturer	= 0;
			this->location			= 0;
			this->device_class		= 0;
			this->clock_speed 		= 0;
			
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
		
//		SetFriendRecord(vector<Friend_Record> new_friend_records){
//			
//			int length = new_friend_records.size();
//			vector<Friend_Record> empty_friend_list;
//			this->friend_list = empty_friend_list;			
//			for(int i=0;i<length;i++){
//				this->friend_list.push_back(new_friend_records[i]);
//			}			
//			
//		};
		
		void AddFriendRecord(Friend_Record new_friend_records){
			this->friend_list.push_back(new_friend_records);
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
			cout << "## ID Device:" 			<< this->device_id;
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
	
};

//struct Registered_Device{	
//	int device_id;
//	vector<Friend_Record> friend_info;
//};		
//	


struct Reputation {
	int id_service_provider;
	int id_service_requester;
	int feedback;
	int num_feedback;
	double reputation_value;
};

class Master
{
	int master_id;	
	int location;
	vector<int> assigned_services;
	//vector<Registered_Device> registered_devices;
	vector<int> registered_devices;
	vector<int> list_of_reputation;
	
	//Reputation[];
	
	
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
			
			for(int i=0;i<this->registered_devices.size();i++){
				if(this->registered_devices[i] == devices_to_delete){
					index_to_delete = i;
					break;
				}
			}
			this->registered_devices.erase(this->registered_devices.begin()+index_to_delete);		
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
	
};
