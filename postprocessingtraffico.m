close all;
clc;
clear;

folder_name = "C:\Users\gianc\Documents\GitHub\SSIoT\simtest\";
n_services_to_find = "1";
vect_n_devices_to_find = "25"; % ["100", "150","200", "300"]
n_master_to_find = "1";
vect_lambda_to_find = ["2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22"]; %["4", "6", "10"]
vect_resource_ctrl_to_find = ["1"]; % ["0","1"]
tot_sim_to_find = "10000";
vect_qoe_ctrl_to_find = ["0"];%["0", "1"]
vect_seed_to_find = ["1", "2", "3", "4", "5", ...
    "6", "7", "8","9","10"]; %[1-20]

%esempio di nome cartella da esplorare 
%Sim-n_services_1-n_devices_25-n_master_1-lambda_2.000000-tot_sim_10000-seed_1-resource_ctrl_1-qoe_ctrl_0

%provo ad accedere ad un file per vedere se funziona
for nlambda = 1:length(vect_lambda_to_find)
    for nseed = 1:length(vect_seed_to_find)
        path = folder_name+"Sim-n_services_1-n_devices_25-n_master_1-lambda_"+vect_lambda_to_find(nlambda)+".000000-tot_sim_10000-seed_"+vect_seed_to_find(nseed)+"-resource_ctrl_1-qoe_ctrl_0\";
        fid = fopen (path+"traffico_perso.txt",'r');
        linea=fgetl(fid); %leggo intestazione
        while ~ feof(fid)
            linea=fgetl(fid);
            extracted_data = strsplit(linea, '\t');
            prob_perdita = str2double(string(extracted_data(1)));
            traffic_loss = str2double(string(extracted_data(2)));
            traffic_matrix(nlambda+1,nseed) = traffic_loss;
        end
        fclose(fid);
    end
end

traffic_matrix_mean = mean(traffic_matrix, 2);

vector_traffic=[];


for i=1:size(traffic_matrix,1)
 vector_traffic=[vector_traffic traffic_matrix(i,:)];
end

g1 = repmat({'lambda=1'},10 ,1);
g2 = repmat({'lambda=2'},10 ,1);
g3 = repmat({'lambda=3'},10 ,1);
g4 = repmat({'lambda=4'},10 ,1);
g5 = repmat({'lambda=5'},10 ,1);
g6 = repmat({'lambda=6'},10 ,1);
g7 = repmat({'lambda=7'},10 ,1);
g8 = repmat({'lambda=8'},10 ,1);
g9 = repmat({'lambda=9'},10 ,1);
g10 = repmat({'lambda=10'},10 ,1);
g11 = repmat({'lambda=11'},10 ,1);
g12 = repmat({'lambda=12'},10 ,1);
g13 = repmat({'lambda=13'},10 ,1);
g14 = repmat({'lambda=14'},10 ,1);
g15 = repmat({'lambda=15'},10 ,1);
g16 = repmat({'lambda=16'},10 ,1);
g17 = repmat({'lambda=17'},10 ,1);
g18 = repmat({'lambda=18'},10 ,1);
g19 = repmat({'lambda=19'},10 ,1);
g20 = repmat({'lambda=20'},10 ,1);
g21 = repmat({'lambda=21'},10 ,1);
g22 = repmat({'lambda=22'},10 ,1);

g_plot = [g1;g2;g3;g4;g5;g6;g7;g8;g9;g10;g11;g12;g13;g14;g15;g16;g17;g18;g19;g20;g21;g22];

boxplot(vector_traffic',g_plot);
hold on
x = 1:22;
%y2 = [0, 0.0108275, 0.0159785, 0.0210346, 0.0260452, 0.031042, 0.0360361, 0.041018, 0.0459632, 0.0508383, 0.0556078, 0.0602381, 0.0647005, 0.068973, 0.0730405, 0.0768949, 0.0805342, 0.0839616, 0.0871842, 0.0902117, 0.11]
%y2_new = [0, 0.0100275, 0.0131604, 0.0176165, 0.0200346, 0.0266912, 0.0313142, 0.0359347, 0.0404859, 0.0501298, 0.0541316, 0.0578872, 0.0613908, 0.0646475, 0.0706692, 0.0768949, 0.0902117, 0.107368, 0.111762, 0.116, 0.14, 0.19];
y2_new = [0, 0.0070275, 0.0101604, 0.0146165, 0.0180346, 0.0226912, 0.0273142, 0.0319347, 0.0364859, 0.0401298, 0.0501298	, 0.0541316, 0.0578872, 0.0646475,0.0706692, 0.0768949, 0.0902117, 0.107368, 0.111762, 0.12892, 0.14, 0.19];
plot(x,y2_new,'LineWidth',2)
hold on
plot(x,traffic_matrix_mean,'--',...
    'LineWidth',1,...
    'MarkerSize',10,...
    'MarkerEdgeColor','b');
legend('modello','simulatore')
xlim([4 22]);
ylim([0 0.6]);
 


figure(2);
%ylim([0 2]);
%set(gca, 'YScale', 'log');


plot(x,traffic_matrix_mean,'LineWidth',2);
xlim([4 22]);
ylim([0 0.4]);

xlabel('lambda')
ylabel('intensità traffico perso')

hold on
%y2 = [0, 0.0108275, 0.0159785, 0.0210346, 0.0260452, 0.031042, 0.0360361, 0.041018, 0.0459632, 0.0508383, 0.0556078, 0.0602381, 0.0647005, 0.068973, 0.0730405, 0.0768949, 0.0805342, 0.0839616, 0.0871842, 0.0902117, 0.11]
plot(x,y2_new,'LineWidth',2)
legend('simulatore','modello')

for i = 1:length(traffic_matrix_mean)
    errore_relativo(i) = abs(traffic_matrix_mean(i) - y2_new(i))/traffic_matrix_mean(i);
    errore_assoluto(i) = abs(traffic_matrix_mean(i) - y2_new(i));
    scarto(i) = norm(traffic_matrix_mean(i) - y2_new(i),2);
    scartoqm (i)= rms(traffic_matrix_mean(i) - y2_new(i));
end
% hold on
figure(3)
plot(x,errore_assoluto,'MarkerSize',2,'MarkerEdgeColor','black');
hold on;
%plot(x,scartoqm,'MarkerSize',2,'MarkerEdgeColor','black');
xlim([2 22]);
xlabel('lambda')
ylabel('scarto %')





