figure(1);
% avg_info(1).final_malicious_vector = movmean(avg_info(1).final_malicious_vector, 500);
% 
% avg_info(2).final_malicious_vector = movmean(avg_info(2).final_malicious_vector, 500);
% avg_info(3).final_malicious_vector = movmean(avg_info(3).final_malicious_vector, 500);

for i = 1:length(avg_info)
    if max(avg_info(i).final_time_vector_4_malicious) < 2000
        next_idx = length(avg_info(i).final_time_vector_4_malicious);
        avg_info(i).final_time_vector_4_malicious(next_idx+1) = 2000;
        avg_info(i).final_malicious_vector(next_idx+1) = avg_info(i).final_malicious_vector(next_idx);
    end
end

plot(avg_info(1).final_time_vector_4_malicious, movmean(avg_info(1).final_malicious_vector/4,25));
hold on;
plot(avg_info(2).final_time_vector_4_malicious, movmean(avg_info(2).final_malicious_vector/4,25));
hold on;
plot(avg_info(3).final_time_vector_4_malicious, movmean(avg_info(3).final_malicious_vector/3,25));
legend("Baseline", "Resource availability approach", "Resource capability approach");
xlabel('Time [s]');
ylabel('Detected Malicious Node [#]');
set(gca,'FontSize',22);
grid on;
% 
% 
% avg_struct = struct();
% info_index = 0;
% for i=1:length(avg_info)
%     for j=1:length(avg_info(i).delay_array)
%         info_index = info_index + 1;
%         splitted_sim_name = split(avg_info(i).sim_name, ' ');
%         
%         %title = split(avg_info(i).sim_name, 'resource');
%         %avg_struct(info_index).scenario = title(1);        
%         avg_struct(info_index).scenario = avg_info(i).sim_name;
%         
%         if str2double(splitted_sim_name(8))==0
%             avg_struct(info_index).approach = "Baseline Approach";
%         elseif str2double(splitted_sim_name(10))==0
%             avg_struct(info_index).approach = "Resource Availability Approach";
%         else
%             avg_struct(info_index).approach = "Proposed Approach";
%         end
%         
%         avg_struct(info_index).delay_value = avg_info(i).delay_array(j);       
%         disp(info_index);
%     end
% end
% 


% riga1 = sum(avg_info(3).processing_time_vect(1:20))/20;
% riga2 = sum(avg_info(6).processing_time_vect(1:20))/20;
% riga3 = sum(avg_info(9).processing_time_vect(1:20))/20;
% riga4 = sum(avg_info(12).processing_time_vect(1:20))/20;
% riga5 = sum(avg_info(15).processing_time_vect(1:20))/20;
% riga6 = sum(avg_info(18).processing_time_vect(1:20))/20;
% riga7 = sum(avg_info(21).processing_time_vect(1:20))/20;
% riga8 = sum(avg_info(24).processing_time_vect(1:20))/20;
% riga9 = sum(avg_info(27).processing_time_vect(1:20))/20;
% riga10 = sum(avg_info(30).processing_time_vect(1:20))/20;
% riga11 = sum(avg_info(33).processing_time_vect(1:20))/20;
% riga12 = sum(avg_info(36).processing_time_vect(1:20))/20;