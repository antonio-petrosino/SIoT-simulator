class Service
{
	int service_id;
	float power_cost;
	int cpu_req; //tolto power_cost -> [cycles/sec]
	
	public:
		Service(){
			this->service_id = 0;
			this->power_cost = 0.0;
			this->cpu_req 	= 0;
		};
		
		SetService(int s_id, float power_c, int cpu_r){
			this->service_id 	= s_id;
			this->power_cost 	= power_c;
			this->cpu_req 		= cpu_r; 
		};
		
		PrintService(){			
			cout << "ID Servizio:" << this->service_id;
			cout << " - Power cost: " << this->power_cost;
			cout << " - CPU Requirement: " << this->cpu_req << endl;
		}
	
};

struct Friend_Record{	
	string type_rel;
	int friend_device;
	float sociality_factor;	
};

class Device{
	int device_id, id_owner, id_manufacturer, location, device_class, clock_speed;
	vector<Friend_Record> friend_list;
	//Friend_Record* friend_list; 
	float total_power, remaining_power;
	vector<int> services_list;
	vector<int> master_node_list;
	//int* services_list;	// vettore di id -> elenco di servizi che può fare
	//int* master_node_list; // vettore di id -> lista dei master node a cui è registrato
	
	public: 
		Devices(){
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
			
			vector<int> empty_services_list;
			this->services_list = empty_services_list;	
			
			vector<int> empty_master_node_list;
			this->master_node_list = empty_master_node_list;
		};
		
		GenerateDevice(int id_d, float tot_p, int id_o, int id_man, int loc, int dev_c, int clock_s){
			this->device_id 		= id_d;
			this->total_power 		= tot_p; 
			this->remaining_power	= tot_p;
			this->id_owner			= id_o;
			this->id_manufacturer	= id_man;
			this->location			= loc;
			this->device_class		= dev_c;
			this->clock_speed 		= clock_s;		

		};
		
		
		SetFriendRecord(vector<Friend_Record> new_friend_records){
			int length = new_friend_records.size();
			vector<Friend_Record> empty_friend_list;
			this->friend_list = empty_friend_list;			
			for(int i=0;i<length;i++){
				this->friend_list.push_back(new_friend_records[i]);
			}			
		};
		
		SetServicesList(vector<int> new_service_list){			
			int length = new_service_list.size();
			vector<int> empty_services_list;
			this->services_list = empty_services_list;			
			for(int i=0;i<length;i++){
				this->services_list.push_back(new_service_list[i]);
			}
		};
		
		SetMasterNodeList(vector<int> new_master_list){			
			int length = new_master_list.size();
			vector<int> empty_master_node_list;
			this->master_node_list = empty_master_node_list;
			
			for(int i=0;i<length;i++){		
				this->master_node_list.push_back(new_master_list[i]);
			}
		};
		
				
		PrintDevice(){	
			cout << endl;
			cout << "## ID Device:" 			<< this->device_id;
			cout << " - ID owner: " 		<< this->id_owner;
			cout << " - ID manufacturer: " 	<< this->id_manufacturer;
			cout << " - Location: "			<< this->location;
			cout << " - Device class: "		<< this->device_class;
			cout << " - Clock speed: "		<< this->clock_speed;
			cout << " - Total power: "		<< this->total_power;
			cout << " - Remain. power: "	<< this->remaining_power;			
			
			// print friend
			cout << " - Friends list:";
			for (Friend_Record i:this->friend_list){
				cout <<" [" << i.type_rel<<":"<<i.friend_device<<":"<<i.sociality_factor<< "] -";
			}
			
			// print Services
			cout << " - Services list:";
			for (int i:this->services_list){
				cout <<" [" << i<< "] -";
			}
			
			// print MasterNode				
			cout << " - Master node list:";
			for (int i:this->master_node_list){
				cout <<" [" << i<< "] -";
			}
			
			cout << " ##"<<endl;
		};
	
		
		
	
};


/*class Master
{
	int master_id;	
	int location;
	
	//Service[];
	//Devices[];
	//Reputation[];
	
	
	
	public:
		Master(){
			this->service_id = 0;
			this->power_cost = 0.0;
			this->cpu_req 	= 0;
		};
		
		SetMaster(int s_id, float power_c, int cpu_r){
			this->service_id 	= s_id;
			this->power_cost 	= power_c;
			this->cpu_req 		= cpu_r; 
		};
		
		PrintMaster(){			
			cout << "ID Servizio:" << this->service_id;
			cout << " - Power cost: " << this->power_cost;
			cout << " - CPU Requirement: " << this->cpu_req << endl;
		}
	
};*/
