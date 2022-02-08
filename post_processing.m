%% SAL:
% file testuali elaborati
% -> InfoQueue.txt
% -> DeltaState.txt
% -> SchedInfo.txt
% mancanti e potrebbero non servire
% -> [UserInfo.txt, AvgRepInfo.txt]
close all; clc; clear;                 
folder_name = 'C:\Users\anton\source\repos\SSIoT\';
folder_name = 'C:\Users\anton\OneDrive - Politecnico di Bari\SSIoT\Definitive con seed\';
%variabili della simulazione da plottare
n_services_to_find = "6";
vect_n_devices_to_find = ["80", "100", "150", "300"]; % ["80", "100", "150", "300"]
n_master_to_find = "5";
vect_lambda_to_find = ["4", "6", "10"]; %["4", "6", "10"]
vect_resource_ctrl_to_find = ["0","1"];
tot_sim_to_find = "3000";
vect_qoe_ctrl_to_find = ["0", "1"];%["0", "1"]
vect_seed_to_find = ["1", "2", "3", "4", "5", ...
                     "6", "7", "8", "9", "10", ...
                     "11", "12", "13", "14", "15", ...
                     "16", "17", "18", "19", "20"]; %[1-20]

sim_legend_string = [];
plotting_info = struct();                
for nd = 1:length(vect_n_devices_to_find)
    for nl = 1:length(vect_lambda_to_find)
        for nrc = 1:length(vect_resource_ctrl_to_find)
            for nqoe = 1:length(vect_qoe_ctrl_to_find)
                
                if vect_resource_ctrl_to_find(nrc)=="0" && vect_qoe_ctrl_to_find(nqoe)=="1"
                    continue;
                end
                
                clc;
                clearvars -except n_services_to_find n_master_to_find tot_sim_to_find ...
                vect_seed_to_find sim_legend_string folder_name nd nl nrc nqoe...
                n_services_to_find vect_n_devices_to_find n_master_to_find vect_lambda_to_find ...
                vect_resource_ctrl_to_find tot_sim_to_find vect_qoe_ctrl_to_find vect_seed_to_find ...
                plotting_info;
             
                n_devices_to_find = vect_n_devices_to_find(nd);
                lambda_to_find = vect_lambda_to_find(nl);
                resource_ctrl_to_find = vect_resource_ctrl_to_find(nrc);
                qoe_ctrl_to_find = vect_qoe_ctrl_to_find(nqoe); %[0-1]
             
                sim_name_for_legend = "n devices " + n_devices_to_find + " lambda " + lambda_to_find + " resource ctrl " + ...
                  resource_ctrl_to_find + " QoE " + qoe_ctrl_to_find;
                disp(sim_name_for_legend);
                sim_legend_string = [sim_legend_string sim_name_for_legend];
                %sim_legend_string(length(sim_legend_string)+1) = sim_name_for_legend;
                %sim_legend_string = sim_legend_string + sim_name_for_legend;
                %disp(sim_legend_string);
                data_mining_mode = true;
                %close all;
                
                seed_scenario = struct();
                
                parfor seed_index = 1:length(vect_seed_to_find)
                    seed_to_find = vect_seed_to_find(seed_index);
                    
                    seed_scenario(seed_index) = struct();
                    seed_scenario(seed_index).number_of_valid_sim = 0;
                 
                    if data_mining_mode == true
                        %% Inizializzazione
%                         clc;
%                         
%                         clearvars -except n_services_to_find n_devices_to_find n_master_to_find lambda_to_find...
%                         tot_sim_to_find seed_to_find resource_ctrl_to_find qoe_ctrl_to_find data_mining_mode...
%                         seed_index vect_seed_to_find seed_scenario sim_name_for_legend sim_legend_string ...
%                         folder_name nd nl nrc nqoe...
%                         n_services_to_find vect_n_devices_to_find n_master_to_find vect_lambda_to_find ...
%                         vect_resource_ctrl_to_find tot_sim_to_find vect_qoe_ctrl_to_find vect_seed_to_find ...
%                         plotting_info number_of_valid_sim;
                     
                        %cartella master simulazioni                  

                        files_list = dir(folder_name);
                     
                        % seed_scenario(seed_index) = struct('user_info', [], 'info_queue', [], 'total_empty_list', []...
                        %     , 'total_accomplished', [], 'time_queue', [], 'avg_available_resources', [], 'avg_delay', []...
                        %     ,'avg_available_resources_movmean', []);
                     
                        for i = 1:str2double(n_devices_to_find)
                            seed_scenario(seed_index).user_info(i) = struct();
                        end
                     
                        %% Elaborazione
                        for i = 1:length(files_list)
                            %trovo tutte le cartelle presenti nella cartella master
                            if files_list(i).isdir == 1
                                dir_name = files_list(i).name;
                                dir_name = strrep(dir_name, '-', '_');
                                fname = split(dir_name, '_');
                             
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
                                    n_services_selected_sim = string(fname(4));
                                    n_devices_selected_sim = string(fname(7));
                                    n_masters_selected_sim = string(fname(10));
                                    lambda_selected_sim = string(fname(12));
                                    tot_sim_selected_sim = string(fname(15));
                                    seed_selected_sim = string(fname(17));
                                    resource_ctrl_selected_sim = string(fname(20));
                                    qoe_ctrl_selected_sim = string(fname(23));
                                 
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
                                     
                                        seed_scenario(seed_index).number_of_valid_sim = 1;  
                                       
                                        for j = 1:length(selected_simulation_files_list)
                                            %trovo tutti i file testuali presenti nella master
                                            %% Code
                                            if selected_simulation_files_list(j).name == "InfoQueue.txt"...
                                                   && selected_simulation_files_list(j).isdir == 0 ...
                                                   && selected_simulation_files_list(j).bytes > 0
                                                sim_file_name = selected_simulation_files_list(j).name;
                                                buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                                read_row = fgetl(buffer_lettura); % leggo la riga dell'intestazione
                                             
                                                seed_scenario(seed_index).k = 0;
                                                while ~ feof(buffer_lettura)
                                                    seed_scenario(seed_index).k = seed_scenario(seed_index).k + 1;
                                                    extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                                    seed_scenario(seed_index).info_queue(seed_scenario(seed_index).k) = str2double(string(extracted_data(1)));
                                                    seed_scenario(seed_index).total_empty_list(seed_scenario(seed_index).k) = str2double(string(extracted_data(2)));
                                                    seed_scenario(seed_index).total_accomplished(seed_scenario(seed_index).k) = str2double(string(extracted_data(3)));
                                                    seed_scenario(seed_index).time_queue(seed_scenario(seed_index).k) = str2double(string(extracted_data(4)));
                                                end
                                             
                                            end
                                         
                                            %% DeltaValue
                                            if selected_simulation_files_list(j).name == "DeltaState.txt"...
                                                   && selected_simulation_files_list(j).isdir == 0 ...
                                                   && selected_simulation_files_list(j).bytes > 0
                                             
                                                sim_file_name = selected_simulation_files_list(j).name;
                                                buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                                %read_row = fgetl(buffer_lettura); % leggo la riga dell'intestazione
                                             
                                                get_avg_value_enabled = false;
                                                selected_user_ID = - 1;
                                                selected_service_ID = - 1;
                                             
                                                while ~ feof(buffer_lettura)
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
                                                        selected_user_ID = - 1;
                                                        selected_service_ID = - 1;
                                                    else
                                                        if selected_user_ID > 0 && selected_service_ID > 0 && get_avg_value_enabled == true
                                                         
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
                                                                seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory + 1).delta = 0.9;
                                                                seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory + 1).time = 0;
                                                                user_length_hystory = 1;
                                                            end
                                                         
                                                            seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory + 1).delta = str2double(string(extracted_data(1)));
                                                            seed_scenario(seed_index).user_info(selected_user_ID).service(selected_service_ID).valore_storicizzato(user_length_hystory + 1).time = str2double(string(extracted_data(2)));
                                                        end
                                                     
                                                    end
                                                end
                                             
                                            end
                                            %% Mean Delay
                                            if selected_simulation_files_list(j).name == "SchedInfo.txt"...
                                                   && selected_simulation_files_list(j).isdir == 0 ...
                                                   && selected_simulation_files_list(j).bytes > 0
                                             
                                                sim_file_name = selected_simulation_files_list(j).name;
                                                buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                             
                                                schedule_index = 0;
                                                while ~ feof(buffer_lettura)
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
                                                     
                                                        seed_scenario(seed_index).schedule_info(schedule_index).id_action = str2double(string(extracted_data(1)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).time_of_arrival = str2double(string(extracted_data(2)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).service_requester = str2double(string(extracted_data(3)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).requested_service = str2double(string(extracted_data(4)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).handling_master_node = str2double(string(extracted_data(5)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).choosen_service_provider = str2double(string(extracted_data(6)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).end_timestamp = str2double(string(extracted_data(7)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).elaboration_time = str2double(string(extracted_data(8)));
                                                        seed_scenario(seed_index).schedule_info(schedule_index).number_of_reschedule = str2double(string(extracted_data(9)));
                                                     
                                                        seed_scenario(seed_index).avg_delay(schedule_index) = seed_scenario(seed_index).schedule_info(schedule_index).end_timestamp - seed_scenario(seed_index).schedule_info(schedule_index).time_of_arrival;
                                                     
                                                        %else if
                                                    end
                                                end
                                             
                                            end
                                            
                                            %% User Details
                                            if selected_simulation_files_list(j).name == "UserInfo.txt"...
                                                   && selected_simulation_files_list(j).isdir == 0 ...
                                                   && selected_simulation_files_list(j).bytes > 0
                                             
                                                sim_file_name = selected_simulation_files_list(j).name;
                                                buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                                resource_monitor_index = 0;
                                             
                                                while ~ feof(buffer_lettura)
                                                    extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                                    resource_monitor_index = resource_monitor_index + 1;
                                                 
                                                    user_id = str2double(string(extracted_data(1)));
                                                    seed_scenario(seed_index).user_info(user_id).malicious_node = str2double(string(extracted_data(9)));
                                                    seed_scenario(seed_index).user_info(user_id).total_power = str2double(string(extracted_data(7)));
                                                end
                                             
                                            end
                                                                                        
                                            %% Resource Monitor
                                            if selected_simulation_files_list(j).name == "ResourceMonitor.txt"...
                                                   && selected_simulation_files_list(j).isdir == 0 ...
                                                   && selected_simulation_files_list(j).bytes > 0
                                             
                                                sim_file_name = selected_simulation_files_list(j).name;
                                                buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                                                extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                                resource_monitor_index = 0;
                                             
                                                while ~ feof(buffer_lettura)
                                                    extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                                                    resource_monitor_index = resource_monitor_index + 1;
                                                 
                                                    seed_scenario(seed_index).resource_monitor(resource_monitor_index).totalAvailableResources = str2double(string(extracted_data(1)));
                                                    seed_scenario(seed_index).resource_monitor(resource_monitor_index).totalNetworkResources = str2double(string(extracted_data(2)));
                                                    seed_scenario(seed_index).resource_monitor(resource_monitor_index).timestamp = str2double(string(extracted_data(3)));
                                                    seed_scenario(seed_index).resource_monitor(resource_monitor_index).single_variation = str2double(string(extracted_data(4)));
                                                 
                                                    % da cambiare se vuoi
                                                    % il total e non
                                                    % l'attivo
                                                    %seed_scenario(seed_index).avg_available_resources(resource_monitor_index) = (resource_monitor(resource_monitor_index).totalAvailableResources / resource_monitor(resource_monitor_index).totalNetworkResources) * 100;
                                                    
                                                    %seed_scenario(seed_index).avg_available_resources(resource_monitor_index) = ...
                                                    %    (resource_monitor(resource_monitor_index).totalAvailableResources / resource_monitor(resource_monitor_index).active_node_total_resources) * 100;
                                                    
                                                    seed_scenario(seed_index).busy_resources(resource_monitor_index) = seed_scenario(seed_index).resource_monitor(resource_monitor_index).totalNetworkResources - seed_scenario(seed_index).resource_monitor(resource_monitor_index).totalAvailableResources;
                                                    seed_scenario(seed_index).time_avg_available_resources(resource_monitor_index) = (seed_scenario(seed_index).resource_monitor(resource_monitor_index).timestamp);
                                                end
                                             
                                            end
                                         

                                         
                                        end
                                    end
                                 
                                end
                             
                            end
                        end
                    end
                            
%                     seed_scenario(seed_index).unique_choosen_provider = [];
%                     for si=1:length(schedule_info)
%                         seed_scenario(seed_index).unique_choosen_provider = [seed_scenario(seed_index).unique_choosen_provider schedule_info(si).choosen_service_provider];
%                     end                    
%                     seed_scenario(seed_index).unique_choosen_provider = unique(seed_scenario(seed_index).unique_choosen_provider);

                    if sum([seed_scenario(seed_index).number_of_valid_sim]) == 0
                        continue
                    end

                    seed_scenario(seed_index).active_node_total_resources = 0;
                    %for ucp = 1:length(seed_scenario(seed_index).unique_choosen_provider)                        
                    for ucp = 1:length(seed_scenario(seed_index).user_info)                        
                        %seed_scenario(seed_index).active_node_total_resources = seed_scenario(seed_index).active_node_total_resources + seed_scenario(seed_index).user_info(seed_scenario(seed_index).unique_choosen_provider(ucp)).total_power;
                        seed_scenario(seed_index).active_node_total_resources = seed_scenario(seed_index).active_node_total_resources + seed_scenario(seed_index).user_info(ucp).total_power;
                    end
                    
                    for upd = 1:length(seed_scenario(seed_index).busy_resources)
                        seed_scenario(seed_index).avg_available_resources(upd) = ...
                            ((seed_scenario(seed_index).active_node_total_resources - seed_scenario(seed_index).busy_resources(upd)) /(seed_scenario(seed_index).active_node_total_resources))* 100;                        
                    end                    
                    %seed_scenario(seed_index).avg_available_resources = ;

                    %% PLOT1: code
                    %f1 = figure(1);
                    %hold on;
                    %plot(seed_scenario(seed_index).time_queue, seed_scenario(seed_index).info_queue);                    
                    %ylabel('Number of queued requests [#]');
                    %xlabel('Simulation time [s]');
                    %xline(str2double(tot_sim_to_find));
                    %hold on;                    
                    %grid on;
                    %legend('Queued', 'Accomplished');
                 
                    %% PLOT2: delta value per il riconoscimento malevoli
                    %f2 = figure(2);                    
                    valid_user_plotted = 0;
                    for i = 1:length(seed_scenario(seed_index).user_info)                        
                        for j = 1:length(seed_scenario(seed_index).user_info(i).service)
                            user_and_service_vector_to_plot = [];
                            time_uas_vector = [];
                            if ~ isempty(seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato)
                                valid_user_plotted = valid_user_plotted + 1;
                                index_user_info = 0;
                             
                                for k = 1:length(seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato)
                                    index_user_info = index_user_info + 1;
                                    user_and_service_vector_to_plot(index_user_info) = seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato(k).delta;
                                    time_uas_vector(index_user_info) = seed_scenario(seed_index).user_info(i).service(j).valore_storicizzato(k).time;
                                end
                             
                                [temp, order_index] = sort(time_uas_vector);
                                user_and_service_vector_to_plot = user_and_service_vector_to_plot(order_index);
                                time_uas_vector = time_uas_vector(order_index);
                             
                                %hold on;
                                %grid on;
                             
                                if seed_scenario(seed_index).user_info(i).malicious_node == 1
                                    %plot(time_uas_vector, user_and_service_vector_to_plot, '-x', 'LineWidth', 2);
                                else
                                    %plot(time_uas_vector, user_and_service_vector_to_plot);
                                end
                             
                                %legend_string(valid_user_plotted) = "user n: " + i + " service n: " + j + ".";
                             
                            end
                        end
                    end
                    %xline(str2double(tot_sim_to_find));
                    %legend(legend_string);
                    %% PLOT3
                    %f3 = figure(3);                   

    %                 tot_avg_delay = [];
    %                 for i = 1:length(seed_scenario)
    %                     tot_avg_delay = [tot_avg_delay seed_scenario(i).avg_delay];
    %                 end                  

                    %ecdf(tot_avg_delay);
                    %hold on;                    
                    %title('ECDF Delay');

                    %seed_scenario(seed_index).avg_delay = mean(seed_scenario(seed_index).avg_delay);
                                 
                    %% PLOT4 Avg available resource
                    %f4 = figure(4);
                    %hold on;
                    %media mobile implementata
                    seed_scenario(seed_index).avg_available_resources_movmean = movmean(seed_scenario(seed_index).avg_available_resources, 250);
                 
                    %plot(seed_scenario(seed_index).time_avg_available_resources, seed_scenario(seed_index).avg_available_resources_movmean);
                    %ylabel('Available resources [%]');
                    %xlabel('Simulation time [s]');
                    %xline(str2double(tot_sim_to_find));
                    %ylim([50 100]);
                 
                    
                    seed_scenario(seed_index).master_processing_time = abs(mean([seed_scenario(seed_index).schedule_info.elaboration_time]));
                    %% end procedure
                    disp("End. seed:" + seed_to_find);
                    fclose('all');
                    
                end
                
                if sum([seed_scenario.number_of_valid_sim]) == 0
                    continue
                end
                
                %% Sezione per mediare sul seed-> tempi di processing e avg delay
                avg_delay_to_extract = mean([seed_scenario.avg_delay]);
                avg_processing_time_to_extract = mean([seed_scenario.master_processing_time])*1000;
                
                if vect_n_devices_to_find(nd) == "80"                    
                    if vect_lambda_to_find(nl) == "4"
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device80_lambda4_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device80_lambda4_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device80_lambda4_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda4_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device80_lambda4_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda4_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "6"                    
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device80_lambda6_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device80_lambda6_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device80_lambda6_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda6_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device80_lambda6_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda6_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "10"                   
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device80_lambda10_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device80_lambda10_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device80_lambda10_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda10_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device80_lambda10_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device80_lambda10_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    end
                    
                elseif vect_n_devices_to_find(nd) == "100"
                   if vect_lambda_to_find(nl) == "4"
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device100_lambda4_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device100_lambda4_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device100_lambda4_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda4_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device100_lambda4_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda4_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "6"                    
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device100_lambda6_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device100_lambda6_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device100_lambda6_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda6_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device100_lambda6_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda6_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "10"                   
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device100_lambda10_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device100_lambda10_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device100_lambda10_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda10_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device100_lambda10_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device100_lambda10_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    end
                elseif vect_n_devices_to_find(nd) == "150"
                  if vect_lambda_to_find(nl) == "4"
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device150_lambda4_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device150_lambda4_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device150_lambda4_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda4_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device150_lambda4_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda4_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "6"                    
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device150_lambda6_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device150_lambda6_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device150_lambda6_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda6_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device150_lambda6_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda6_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "10"                   
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device150_lambda10_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device150_lambda10_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device150_lambda10_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda10_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device150_lambda10_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device150_lambda10_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    end       
                elseif vect_n_devices_to_find(nd) == "300"
                  if vect_lambda_to_find(nl) == "4"
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device300_lambda4_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device300_lambda4_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device300_lambda4_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda4_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device300_lambda4_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda4_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "6"                    
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device300_lambda6_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device300_lambda6_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device300_lambda6_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda6_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device300_lambda6_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda6_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                    elseif vect_lambda_to_find(nl) == "10"                   
                        if vect_resource_ctrl_to_find(nrc) == "0"
                            plotting_info.n_device300_lambda10_resource0_qoe_0_avg_delay = avg_delay_to_extract;
                            plotting_info.n_device300_lambda10_resource0_qoe_0_processing = avg_processing_time_to_extract;
                        elseif vect_resource_ctrl_to_find(nrc) == "1"
                            if vect_qoe_ctrl_to_find(nqoe) == "0"
                                plotting_info.n_device300_lambda10_resource1_qoe_0_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda10_resource1_qoe_0_processing = avg_processing_time_to_extract;
                            elseif vect_qoe_ctrl_to_find(nqoe) == "1"
                                plotting_info.n_device300_lambda10_resource1_qoe_1_avg_delay = avg_delay_to_extract;
                                plotting_info.n_device300_lambda10_resource1_qoe_1_processing = avg_processing_time_to_extract;
                            end
                        end
                  end                    
                end
            
             
                %% PLOT 1 - CODE - mediato su tutti i seed
                time_limit = zeros(length(seed_scenario), 1);
                for i = 1:length(seed_scenario)
                    time_limit(i) = max(seed_scenario(i).time_queue);
                end
                max_time_limit = ceil(max(time_limit));
             
                final_time_vector = [];
                final_queue_vector = [];
                for time = (1):max_time_limit
                    values_slotted = [];
                    for i = 1:length(seed_scenario)
                        index_to_consider = find(seed_scenario(i).time_queue < time & seed_scenario(i).time_queue >= (time - 1));
                        value_to_consider = seed_scenario(i).info_queue(index_to_consider);
                        values_slotted = [values_slotted value_to_consider];
                    end
                    final_time_vector = [final_time_vector (time - 1)];
                    if ~isempty(values_slotted)
                        final_queue_vector = [final_queue_vector mean(values_slotted)];
                    else
                        final_queue_vector = [final_queue_vector final_queue_vector(length(final_queue_vector) - 1)];
                    end
                end
             
                figure(101);
                hold on;
                final_queue_vector = movmean(final_queue_vector, 25);
                %plot(final_time_vector, final_queue_vector, 'LineWidth', 2);
                semilogy(final_time_vector, final_queue_vector, 'LineWidth', 2);
             
                %% PLOT 2
             
                %% PLOT 3
                figure(103);
                hold on;
                tot_avg_delay = [];
                for i = 1:length(seed_scenario)
                    tot_avg_delay = [tot_avg_delay abs(seed_scenario(i).avg_delay)];
                end
                ecdf(tot_avg_delay);

             
                %% PLOT 4
                time_limit = zeros(length(seed_scenario), 1);
                for i = 1:length(seed_scenario)
                    time_limit(i) = max(seed_scenario(i).time_avg_available_resources);
                end
                max_time_limit = ceil(max(time_limit));
             
                final_time_vector = [];
                final_queue_vector = [];
                for time = (1):max_time_limit
                    values_slotted = [];
                    for i = 1:length(seed_scenario)
                        index_to_consider = find(seed_scenario(i).time_avg_available_resources < time & seed_scenario(i).time_avg_available_resources >= (time - 1));
                        value_to_consider = seed_scenario(i).avg_available_resources(index_to_consider);
                        values_slotted = [values_slotted value_to_consider];
                    end
                    final_time_vector = [final_time_vector (time - 1)];
                    if ~isempty(values_slotted)
                        final_queue_vector = [final_queue_vector mean(values_slotted)];
                    else
                        final_queue_vector = [final_queue_vector final_queue_vector(length(final_queue_vector) - 1)];
                    end
                end
                
                final_queue_vector = movmean(final_queue_vector, 25);
                
                figure(104);
                hold on;                
                plot(final_time_vector, final_queue_vector, 'LineWidth', 2);                
             
                pause(0);
            end
        end
    end
 
end
%close(f1);
%close(f2);
%close(f3);
%close(f4);

figure(101);
hold on;                
%plot(info_queue);
ylabel('Number of queued requests [#]');
xlabel('Simulation time [s]');
%xline(str2double(tot_sim_to_find));
xlim([0 str2double(tot_sim_to_find)]);
%plot(time_queue, total_accomplished);
grid on;
%legend('Queued', 'Accomplished');
legend(sim_legend_string);
xlim([0 2900]);

figure(103);
hold on;
%ecdf(seed_scenario(seed_index).avg_delay);
title('ECDF Delay');
%xlim([0 str2double(tot_sim_to_find)]);
legend(sim_legend_string);
xlim([0 200]);

figure(104);
hold on;
ylabel('Available resources [%]');
xlabel('Simulation time [s]');
%xline(str2double(tot_sim_to_find));
ylim([70 100]);
xlim([0 str2double(tot_sim_to_find)]);
legend(sim_legend_string);
xlim([0 2900]);