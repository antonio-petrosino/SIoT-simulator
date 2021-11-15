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
	int id_device, id_owner, id_manufacturer, location, device_class, clock_speed;
	Friend_Record* friend_list; 
	float total_power, remaining_power;
	int* services_list;	// vettore di id -> elenco di servizi che può fare
	int* master_node_list; // vettore di id -> lista dei master node a cui è registrato
	
	public: 
		Devices(){
			this->id_device 		= 0;
			this->total_power 		= 0.0; 
			this->remaining_power	= 0.0;
			this->id_owner			= 0;
			this->id_manufacturer	= 0;
			this->location			= 0;
			this->device_class		= 0;
			this->clock_speed 		= 0;
		};
		
		GenerateDevice(int id_d, float tot_p, int id_o, int id_man, int loc, int dev_c, int clock_s){
			this->id_device 		= id_d;
			this->total_power 		= tot_p; 
			this->remaining_power	= tot_p;
			this->id_owner			= id_o;
			this->id_manufacturer	= id_man;
			this->location			= loc;
			this->device_class		= dev_c;
			this->clock_speed 		= clock_s;		

		};
		
		
		SetFriendRecord(Friend_Record* new_friend_records){
			int length = sizeof(new_friend_records)/ sizeof(Friend_Record);
			this->friend_list = new Friend_Record[length];			
			for(int i=0;i<length;i++){
				this->friend_list[i] = new_friend_records[i];				
			}			
		};
		
		SetServicesList(int* new_service_list){			
			int length = sizeof(new_service_list)/ sizeof(int);
			this->services_list = new int[length];			
			for(int i=0;i<length;i++){
				this->services_list[i] = new_service_list[i];				
			}
		};
		
		SetMasterNodeList(int* new_master_list){
			int length = sizeof(new_master_list)/ sizeof(int);
			this->master_node_list = new int[length];			
			for(int i=0;i<length;i++){
				this->master_node_list[i] = new_master_list[i];				
			}
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
