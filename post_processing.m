%% Inizializzazione
clc;
clear; 
%%close all;

%cartella master simulazioni
folder_name = 'C:\Users\anton\OneDrive - Politecnico di Bari\SSIoT\Simulazioni\';
files_list = dir(folder_name);


%variabili della simulazione da plottare
n_services_to_find     = "6";
n_devices_to_find      = "150";
n_master_to_find       = "5";
lambda_to_find         = "8";
tot_sim_to_find        = "2500";
seed_to_find           = "1";
resource_ctrl_to_find  = "1";
qoe_ctrl_to_find       = "1";

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
        
        if length(fname) > 1            
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
                                info_queue(k) = str2num(string(extracted_data(1)));
                                total_empty_list(k) = str2num(string(extracted_data(2)));
                                total_accomplished(k) = str2num(string(extracted_data(3)));
                                time_queue(k) = str2num(string(extracted_data(4)));
                            end
                            
                        end
                        
                        %% 
                        if selected_simulation_files_list(j).name =="DeltaState.txt"...
                                && selected_simulation_files_list(j).isdir == 0 ...
                                && selected_simulation_files_list(j).bytes > 0
                            
                            sim_file_name = selected_simulation_files_list(j).name;                            
                            buffer_lettura = fopen(strcat(new_Path_to_explore, sim_file_name));
                            %read_row = fgetl(buffer_lettura); % leggo la riga dell'intestazione
                            
                            extracted_data = strsplit(fgetl(buffer_lettura), '\t');
                            
                            
                            
                        end
                    end
            end
            
        end
        
        
        
    end    
end

plot(time_queue, info_queue);
ylabel('Number of requests [#]');
xlabel('Simulation time [s]');
xline(str2num(tot_sim_to_find));
hold on;
%plot(time_queue, total_accomplished);
grid on;
%legend('Queued', 'Accomplished');
disp("End.");
fclose('all');
