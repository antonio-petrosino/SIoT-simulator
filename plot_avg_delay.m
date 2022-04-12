% for i=1:length(avg_info)
%      %idx = avg_info(i).delay_array > 0;
%      idx = find(avg_info(i).delay_array>0);
%      avg_info(i).delay_array = avg_info(i).delay_array(idx);
%      avg_info(i).delay_medio = mean(avg_info(i).delay_array);
%      x = avg_info(i).delay_array;
%      SEM = std(x)/sqrt(length(x));               % Standard Error
%      ts = tinv([0.025  0.975],length(x)-1);      % T-Score
%      avg_info(i).CI = mean(x) + ts*SEM;          % Confidence Intervals 
%      avg_info(i).higherror = avg_info(i).CI(2);   
%      avg_info(i).lowerror = avg_info(i).CI(1);   
%      avg_info(i).delay_error = avg_info(i).CI(2) - avg_info(i).CI(1);
% end

%clearvars -except avg_info avg_info_backup

%%% AVG DELAY lambda = 4
y7 =[avg_info(1:3).delay_medio;
    avg_info(10:12).delay_medio;
    avg_info(19:21).delay_medio;
    avg_info(28:30).delay_medio];

e7 =[avg_info(1:3).delay_error;
    avg_info(10:12).delay_error;
    avg_info(19:21).delay_error;
    avg_info(28:30).delay_error];
%figure(1);
%b = bar(y7);
%breakyaxis([10 30]);

% [ngroups,nbars] = size(y7);
% % Get the x coordinate of the bars
% x = nan(nbars, ngroups);
% for i = 1:nbars
%     x(i,:) = b(i).XEndPoints;
% end
% % Plot the errorbars
% errorbar(x',y7,e7,'k','linestyle','none');
% hold off
figure(1);
subplot(2,1,2);
bar(y7);
set(gca,'XTickLabel',{'100', '150','200', '300'});
xlabel('Social objects [#]');
ylabel('Average delay [s]');
ylim([0 2]);
subplot(2,1,1);
bar(y7);
ylim([25 350]);
%set(gca, 'YScale', 'log');
%set(gca,'XTickLabel',{'100', '150','200', '300'});
legend("Baseline", "Resource availability approach", "Resource capability approach");

%%% AVG DELAY lambda = 6
y7 =[avg_info(4:6).delay_medio;
    avg_info(13:15).delay_medio;
    avg_info(22:24).delay_medio;
    avg_info(31:33).delay_medio];
figure(2);
subplot(2,1,2);
bar(y7);
set(gca,'XTickLabel',{'100', '150','200', '300'});
xlabel('Social objects [#]');
ylabel('Average delay [s]');
ylim([0 2]);
subplot(2,1,1);
bar(y7);
ylim([25 350]);
%set(gca, 'YScale', 'log');
%set(gca,'XTickLabel',{'100', '150','200', '300'});
legend("Baseline", "Resource availability approach", "Resource capability approach");

%%% AVG DELAY lambda = 10
y7 =[avg_info(7:9).delay_medio;
    avg_info(16:18).delay_medio;
    avg_info(25:27).delay_medio;
    avg_info(34:36).delay_medio];
%figure(3);
figure(3);
subplot(2,1,2);
bar(y7);
set(gca,'XTickLabel',{'100', '150','200', '300'});
xlabel('Social objects [#]');
ylabel('Average delay [s]');
ylim([0 2]);
subplot(2,1,1);
bar(y7);
ylim([25 350]);
%set(gca, 'YScale', 'log');
%set(gca,'XTickLabel',{'100', '150','200', '300'});
legend("Baseline", "Resource availability approach", "Resource capability approach");

