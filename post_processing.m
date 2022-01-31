%% SAL: 
% file testuali elaborati
% -> InfoQueue.txt
% -> DeltaState.txt
% -> SchedInfo.txt
% mancanti e potrebbero non servire
% -> [UserInfo.txt, AvgRepInfo.txt]

%variabili della simulazione da plottare
n_services_to_find     = "6";
n_devices_to_find      = "80";
n_master_to_find       = "5";
lambda_to_find         = "10";
tot_sim_to_find        = "3000";
vect_seed_to_find      = ["1", "2", "3", "4", "5"];
resource_ctrl_to_find  = "1";
qoe_ctrl_to_find       = "0";


data_mining_mode = true;
close all;
seed_scenario = struct();

for seed_index=1:length(vect_seed_to_find)
   seed_to_find = vect_seed_to_find(seed_index);


if data_mining_mode == true
%% Inizializzazione
clc;
clearvars -except n_services_to_find n_devices_to_find n_master_to_find lambda_to_find...
    tot_sim_to_find seed_to_find resource_ctrl_to_find qoe_ctrl_to_find data_mining_mode...
    seed_index vect_seed_to_find seed_scenario; 

%cartella master simulazioni

folder_name = 'C:\Users\anton\source\repos\SSIoT\';
files_list = dir(folder_name);

% seed_scenario(seed_index) = struct('user_info', [], 'info_queue', [], 'total_empty_list', []...
%     , 'total_accomplished', [], 'time_queue', [], 'avg_available_resources', [], 'avg_delay', []...
%     ,'avg_available_resources_movmean', []);


for i=1:str2double(n_devices_to_find)
    seed_scenario(seed_index).user_info(i) = struct();    
end


%% Elaborazione
for i=1:length(files_list)
    %trovo tutte le cartelle presenti nella cartella master
    if files_list(i).isdir == 1 
        dir_name = files_list(i).name;
        dir_name = strrep(dir_name, '-', '_');
        fname=split(dir_name, '_');
        
        % ------------------------%
        % index | variable
        % ------------------------%
        % 4     | n_services      |
        % 7     | n_devices       |
        % 10    | n_masters       |
        % 12    | lambda          |
        % 15    | tot_sim         |
        % 17    | seed            |
        % 20    | resource_ctrl   |
        % 23    | qoe_ctrl        |
        % ------------------------%
        
        if length(fname) > 22            
            n_services_selected_sim     = string(fname(4));
            n_devices_selected_sim      = string(fname(7));
            n_masters_selected_sim      = string(fname(10));
            lambda_selected_sim         = string(fname(12));
            tot_sim_selected_sim        = string(fname(15));
            seed_selected_sim           = string(fname(17));
            resource_ctrl_selected_sim  = string(fname(20));
            qoe_ctrl_selected_sim       = string(fname(23));

            
            if n_services_to_find == n_services_selected_sim...
                    && n_devices_to_find == n_devices_selected_sim...
                    && n_master_to_find == n_masters_selected_sim ...
                    && lambda_to_find == lambda_selected_sim ...
                    && tot_sim_to_find == tot_sim_selected_sim ...
                    && seed_to_find == seed_selected_sim ...
                    && resource_ctrl_to_find == resource_ctrl_selected_sim ...
                    && qoe_ctrl_to_find == qoe_ctrl_selected_sim 
                    
                    new_Path_to_explore = folder_name + string(files_list(i).name) + '\';
                    selected_simulation_files_list = dir(new_Path_to_explore);
                    
                    
                    for j=1:length(selected_simulation_files_list)
                        %trovo tutti i file testuali presenti nella master
                        %% Code
                        if selected_simulation_files_list(j).name =="InfoQueue.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            sim_file_name = selected_simulation_files_list(j).name;                            
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                            read_row = fgetl(buffer_lettura); % leggo la riga dell'intestazione
                            
                            k = 0;
                            while ~feof(buffer_lettura)
                                k = k +1;
                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                seed_scenario(seed_index).info_queue(k) = str2num(string(extracted_data(1)));
                                seed_scenario(seed_index).total_empty_list(k) = str2num(string(extracted_data(2)));
                                seed_scenario(seed_index).total_accomplished(k) = str2num(string(extracted_data(3)));
                                seed_scenario(seed_index).time_queue(k) = str2num(string(extracted_data(4)));
                            end
                            
                        end
                        
                        %% DeltaValue
                        if selected_simulation_files_list(j).name =="DeltaState.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            
                            sim_file_name = selected_simulation_files_list(j).name;                            
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                            %read_row = fgetl(buffer_lettura); % leggo la riga dell'intestazione
                            
                            get_avg_value_enabled = false;
                            selected_user_ID = -1;
                            selected_service_ID = -1;
                                    
                            while ~feof(buffer_lettura)
                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');

                                if string(extracted_data(1)) == 'USER INFO ' && get_avg_value_enabled == false
                                    selected_user_ID = str2double(string(extracted_data(3)));
                                    selected_service_ID = str2double(string(extracted_data(5)));
                                elseif string(extracted_data(1)) == 'DELTA INFO' && get_avg_value_enabled == false
                                elseif string(extracted_data(1)) == 'AVGVALUE' && get_avg_value_enabled == false
                                    get_avg_value_enabled = true;
                                elseif string(extracted_data(1)) == 'END DELTA INFO' && get_avg_value_enabled == true
                                    get_avg_value_enabled = false;
                                elseif string(extracted_data(1)) == 'END USER INFO'
                                    selected_user_ID = -1;
                                    selected_service_ID = -1;
                                else
                                    if selected_user_ID>0 && selected_service_ID > 0 && get_avg_value_enabled == true     
                                        
%                                         if isfield(seed_scenario(seed_index).user_info(selected_user_ID), 'service')
%                                             service_length = length(seed_scenario(seed_index).user_info(selected_user_ID).service);
%                                         else
%                                             service_length = 0;
%                                         end
                                        seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).name = selected_service_ID;
                                        
                                        if isfield(seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID), 'valore_storicizzato')
                                            user_length_hystory = length(seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato);
                                        else
                                            user_length_hystory = 0;
                                            seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory+1).delta = 0.9;
                                            seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory+1).time = 0;
                                            user_length_hystory = 1;
                                        end
                                        
                                        seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory+1).delta = str2double(string(extracted_data(1)));
                                        seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory+1).time = str2double(string(extracted_data(2)));
                                    end
                                    
                                end
                            end
                            
                        end
                        %% Mean Delay                       
                        if selected_simulation_files_list(j).name =="SchedInfo.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            
                            sim_file_name = selected_simulation_files_list(j).name;   
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                                        
                            schedule_index = 0;
                            while ~feof(buffer_lettura)
                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                a = 100;
                                if contains(extracted_data, '%%%%%%%%%%%%%%%% SCHEDULER ITEM(')
                                    schedule_index = schedule_index + 1;
                                    extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                    
                                    %schedule_info(schedule_index) = struct;                                    
                                    
                                    %id_action	
                                    %time_of_arrival	
                                    %service_requester	
                                    %requested_service	
                                    %handling_master_node	
                                    %choosen_service_provider	
                                    %end_timestamp	
                                    %elaboration_time
                                    %number_of_reschedule	
                                    
                                    schedule_info(schedule_index).id_action                 = str2num(string(extracted_data(1)));
                                    schedule_info(schedule_index).time_of_arrival           = str2num(string(extracted_data(2)));
                                    schedule_info(schedule_index).service_requester         = str2num(string(extracted_data(3)));
                                    schedule_info(schedule_index).requested_service         = str2num(string(extracted_data(4)));
                                    schedule_info(schedule_index).handling_master_node      = str2num(string(extracted_data(5)));
                                    schedule_info(schedule_index).choosen_service_provider	= str2num(string(extracted_data(6)));
                                    schedule_info(schedule_index).end_timestamp             = str2num(string(extracted_data(7)));
                                    schedule_info(schedule_index).elaboration_time          = str2num(string(extracted_data(8)));
                                    schedule_info(schedule_index).number_of_reschedule      = str2num(string(extracted_data(9)));
                                    
                                    seed_scenario(seed_index).avg_delay(schedule_index) = schedule_info(schedule_index).end_timestamp - schedule_info(schedule_index).time_of_arrival;
                                    
                                    
                                %else if
                                end
                            end
                            
                        end
                        
                        %% Resource Monitor                       
                        if selected_simulation_files_list(j).name =="ResourceMonitor.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            
                            sim_file_name = selected_simulation_files_list(j).name;   
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                            extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                            resource_monitor_index = 0;
                            
                            while ~feof(buffer_lettura)
                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                resource_monitor_index = resource_monitor_index +1;
                                
                                resource_monitor(resource_monitor_index).totalAvailableResources    = str2num(string(extracted_data(1)));
                                resource_monitor(resource_monitor_index).totalNetworkResources      = str2num(string(extracted_data(2)));
                                resource_monitor(resource_monitor_index).timestamp                  = str2num(string(extracted_data(3)));
                                resource_monitor(resource_monitor_index).single_variation           = str2num(string(extracted_data(4)));
                                
                                seed_scenario(seed_index).avg_available_resources(resource_monitor_index)         = (resource_monitor(resource_monitor_index).totalAvailableResources / resource_monitor(resource_monitor_index).totalNetworkResources) * 100;
                                seed_scenario(seed_index).time_avg_available_resources(resource_monitor_index)    = (resource_monitor(resource_monitor_index).timestamp);
                            end
                            
                            
                        end
                        
                        
                        %% User Details                      
                        if selected_simulation_files_list(j).name =="UserInfo.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            
                            sim_file_name = selected_simulation_files_list(j).name;   
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                            extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                            resource_monitor_index = 0;
                            
                            while ~feof(buffer_lettura)
                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                resource_monitor_index = resource_monitor_index +1;
                                
                                user_id = str2num(string(extracted_data(1)));
                                seed_scenario(seed_index).user_info(user_id).malicious_node = str2num(string(extracted_data(9)));                              
                            end
                            
                            
                        end
                        
                        
                    end
            end
            
        end
        
        
        
    end    
end
end 

%% PLOT1: code
figure(1);
hold on;
plot(seed_scenario(seed_index).time_queue, seed_scenario(seed_index).info_queue);
%plot(info_queue);
ylabel('Number of queued requests [#]');
xlabel('Simulation time [s]');
xline(str2num(tot_sim_to_find));
hold on;
%plot(time_queue, total_accomplished);
grid on;
%legend('Queued', 'Accomplished');


%% PLOT2: delta value per il riconoscimento malevoli
figure(2);
%legend_string = "";
valid_user_plotted = 0;
for i=1:length(seed_scenario(seed_index).user_info)
    %%if length(seed_scenario(seed_index).user_info(i).service) > 0
    for j=1:length(seed_scenario(seed_index).user_info(i).service)
        user_and_service_vector_to_plot = [];
        time_uas_vector = [];
        if ~isempty(seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato)
            valid_user_plotted = valid_user_plotted +1;
            index_user_info = 0;
            
            for k=1:length(seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato)
                index_user_info = index_user_info + 1;
                user_and_service_vector_to_plot(index_user_info) = seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato(k).delta;
                time_uas_vector(index_user_info) = seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato(k).time;
            end
            
            [temp, order_index] = sort(time_uas_vector);            
            user_and_service_vector_to_plot = user_and_service_vector_to_plot(order_index);
            time_uas_vector = time_uas_vector(order_index);
            
            hold on;
            grid on;
            
            if seed_scenario(seed_index).user_info(i).malicious_node == 1
                plot(time_uas_vector, user_and_service_vector_to_plot,'-x','LineWidth',2);
            else           
                plot(time_uas_vector, user_and_service_vector_to_plot);                
            end
            
            legend_string(valid_user_plotted) = "user n: " + i + " service n: " + j + ".";
            
        end
    end
end
xline(str2num(tot_sim_to_find));
legend(legend_string);

%% PLOT3
figure(3);
tot_avg_delay = [];
for i=1:length(seed_scenario)
    tot_avg_delay = [tot_avg_delay seed_scenario(i).avg_delay];
end 
ecdf(tot_avg_delay);
hold on;
%ecdf(seed_scenario(seed_index).avg_delay);
title('ECDF Delay');
seed_scenario(seed_index).avg_delay = mean(seed_scenario(seed_index).avg_delay);

%% PLOT4 Avg available resource
figure(4);
hold on;
%media mobile implementata
seed_scenario(seed_index).avg_available_resources_movmean = movmean(seed_scenario(seed_index).avg_available_resources, 250);

plot(seed_scenario(seed_index).time_avg_available_resources, seed_scenario(seed_index).avg_available_resources_movmean);
ylabel('Available resources [%]');
xlabel('Simulation time [s]');
xline(str2num(tot_sim_to_find));
ylim([50 100]);

%% end procedure
disp("End. seed:" + seed_to_find);
fclose('all');
end
%% PLOT 1 - CODE - mediato su tutti i seed
time_limit = zeros(length(seed_scenario), 1);
for i=1:length(seed_scenario)
time_limit(i) = max(seed_scenario(i).time_queue);
end
max_time_limit = ceil(max(time_limit));

final_time_vector = [];
final_queue_vector = [];
for time=(1):max_time_limit    
    values_slotted = [];
    for i=1:length(seed_scenario)        
        index_to_consider = find(seed_scenario(i).time_queue < time & seed_scenario(i).time_queue >= (time-1) );        
        value_to_consider = seed_scenario(i).info_queue(index_to_consider);        
        values_slotted = [values_slotted value_to_consider];
    end    
    final_time_vector = [final_time_vector (time-1)];    
    if length(values_slotted)>0
        final_queue_vector = [final_queue_vector mean(values_slotted)];    
    else
        final_queue_vector = [final_queue_vector final_queue_vector(length(final_queue_vector)-1)];
    end    
end

figure(1);
hold on;
final_queue_vector = movmean(final_queue_vector, 25);
plot(final_time_vector, final_queue_vector,'LineWidth',2)

%% PLOT 4
time_limit = zeros(length(seed_scenario), 1);
for i=1:length(seed_scenario)
time_limit(i) = max(seed_scenario(i).time_avg_available_resources);
end
max_time_limit = ceil(max(time_limit));

final_time_vector = [];
final_queue_vector = [];
for time=(1):max_time_limit    
    values_slotted = [];
    for i=1:length(seed_scenario)        
        index_to_consider = find(seed_scenario(i).time_avg_available_resources < time & seed_scenario(i).time_avg_available_resources >= (time-1) );        
        value_to_consider = seed_scenario(i).avg_available_resources(index_to_consider);        
        values_slotted = [values_slotted value_to_consider];
    end    
    final_time_vector = [final_time_vector (time-1)];    
    if length(values_slotted)>0
        final_queue_vector = [final_queue_vector mean(values_slotted)];    
    else
        final_queue_vector = [final_queue_vector final_queue_vector(length(final_queue_vector)-1)];
    end    
end

figure(4);
hold on;
final_queue_vector = movmean(final_queue_vector, 25);
plot(final_time_vector, final_queue_vector,'LineWidth',2)
