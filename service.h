class Service
{
	int service_id;
	int cpu_req; //tolto power_cost -> [cycles/sec]
	
	public:
		Service(){
			this->service_id = 0;
			this->cpu_req 	= 0;
		};
		
		SetService(int s_id, int cpu_r){
			this->service_id 	= s_id;
			this->cpu_req 		= cpu_r; 
		};
	
};


Service initialization_services(){
	
	Service new_obj;
	
	new_obj.SetService(10,1);
	
	
	return new_obj;
}



