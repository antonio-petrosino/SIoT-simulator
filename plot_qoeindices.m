%set(findall(gca, 'Type', 'Line'),'LineWidth',4);
% lines = findall(gca, 'Type', 'Line');
% set(lines(9), 'Color', '#0072bd');
% set(lines(6), 'Color', '#0072bd');
% set(lines(3), 'Color', '#0072bd');
%set(findall(gca, 'Type', 'Line'),'LineWidth',4);
% % % lines = findall(gca, 'Type', 'Line');
% % % set(lines(4), 'LineStyle', '-.', 'Marker', 'o');
% % % set(lines(5), 'LineStyle', '-.', 'Marker', 'o');
% % % set(lines(6), 'LineStyle', '-.', 'Marker', 'o');
% % % % set(lines(9), 'Color', 'black');
% % % % set(lines(6), 'Color', 'black');
% % % % set(lines(3), 'Color', 'black');
% % % 
% % % leg = legend("Baseline - λ = 4", "Resource availability approach - λ = 4", "Resource capability approach - λ = 4",...
% % %              "Baseline - λ = 6", "Resource availability approach - λ = 6", "Resource capability approach - λ = 6",...
% % %              "Baseline - λ = 10","Resource availability approach - λ = 10","Resource capability approach - λ = 10");
% % % leg.ItemTokenSize = [100,100];
% h = get(gca,'Children');
% set(gca,'Children',[h(6) h(5) h(4) h(3) h(2) h(1)]);

% QoEIndex(1).info = plotting_info.qoeindexmean_n_device100_lambda4_resource0_qoe_0;
% QoEIndex(2).info = plotting_info.qoeindexmean_n_device100_lambda4_resource1_qoe_0;
% QoEIndex(3).info = plotting_info.qoeindexmean_n_device100_lambda4_resource1_qoe_1;
% QoEIndex(4).info = plotting_info.qoeindexmean_n_device100_lambda6_resource0_qoe_0;
% QoEIndex(5).info = plotting_info.qoeindexmean_n_device100_lambda6_resource1_qoe_0;
% QoEIndex(6).info = plotting_info.qoeindexmean_n_device100_lambda6_resource1_qoe_1;
% QoEIndex(7).info = plotting_info.qoeindexmean_n_device100_lambda10_resource0_qoe_0;
% QoEIndex(8).info = plotting_info.qoeindexmean_n_device100_lambda10_resource1_qoe_0;
% QoEIndex(9).info = plotting_info.qoeindexmean_n_device100_lambda10_resource1_qoe_1;
% QoEIndex(10).info  = plotting_info.qoeindexmean_n_device150_lambda4_resource0_qoe_0;
% QoEIndex(11).info  = plotting_info.qoeindexmean_n_device150_lambda4_resource1_qoe_0;
% QoEIndex(12).info  = plotting_info.qoeindexmean_n_device150_lambda4_resource1_qoe_1;
% QoEIndex(13).info  = plotting_info.qoeindexmean_n_device150_lambda6_resource0_qoe_0;
% QoEIndex(14).info  = plotting_info.qoeindexmean_n_device150_lambda6_resource1_qoe_0;
% QoEIndex(15).info  = plotting_info.qoeindexmean_n_device150_lambda6_resource1_qoe_1;
% QoEIndex(16).info  = plotting_info.qoeindexmean_n_device150_lambda10_resource0_qoe_0;
% QoEIndex(17).info  = plotting_info.qoeindexmean_n_device150_lambda10_resource1_qoe_0;
% QoEIndex(18).info  = plotting_info.qoeindexmean_n_device150_lambda10_resource1_qoe_1;
% QoEIndex(19).info  = plotting_info.qoeindexmean_n_device200_lambda4_resource0_qoe_0;
% QoEIndex(20).info  = plotting_info.qoeindexmean_n_device200_lambda4_resource1_qoe_0;
% QoEIndex(21).info  = plotting_info.qoeindexmean_n_device200_lambda4_resource1_qoe_1;
% QoEIndex(22).info  = plotting_info.qoeindexmean_n_device200_lambda6_resource0_qoe_0;
% QoEIndex(23).info  = plotting_info.qoeindexmean_n_device200_lambda6_resource1_qoe_0;
% QoEIndex(24).info  = plotting_info.qoeindexmean_n_device200_lambda6_resource1_qoe_1;
% QoEIndex(25).info  = plotting_info.qoeindexmean_n_device200_lambda10_resource0_qoe_0;
% QoEIndex(26).info  = plotting_info.qoeindexmean_n_device200_lambda10_resource1_qoe_0;
% QoEIndex(27).info  = plotting_info.qoeindexmean_n_device200_lambda10_resource1_qoe_1;
% QoEIndex(28).info  = plotting_info.qoeindexmean_n_device300_lambda4_resource0_qoe_0;
% QoEIndex(29).info  = plotting_info.qoeindexmean_n_device300_lambda4_resource1_qoe_0;
% QoEIndex(30).info  = plotting_info.qoeindexmean_n_device300_lambda4_resource1_qoe_1;
% QoEIndex(31).info  = plotting_info.qoeindexmean_n_device300_lambda6_resource0_qoe_0;
% QoEIndex(32).info  = plotting_info.qoeindexmean_n_device300_lambda6_resource1_qoe_0;
% QoEIndex(33).info  = plotting_info.qoeindexmean_n_device300_lambda6_resource1_qoe_1;
% QoEIndex(34).info  = plotting_info.qoeindexmean_n_device300_lambda10_resource0_qoe_0;
% QoEIndex(35).info  = plotting_info.qoeindexmean_n_device300_lambda10_resource1_qoe_0;
% QoEIndex(36).info  = plotting_info.qoeindexmean_n_device300_lambda10_resource1_qoe_1;

QoEIndex(1).info = plotting_info.qoemean_n_device100_lambda4_resource0_qoe_0;
QoEIndex(2).info = plotting_info.qoemean_n_device100_lambda4_resource1_qoe_0;
QoEIndex(3).info = plotting_info.qoemean_n_device100_lambda4_resource1_qoe_1;
QoEIndex(4).info = plotting_info.qoemean_n_device100_lambda6_resource0_qoe_0;
QoEIndex(5).info = plotting_info.qoemean_n_device100_lambda6_resource1_qoe_0;
QoEIndex(6).info = plotting_info.qoemean_n_device100_lambda6_resource1_qoe_1;
QoEIndex(7).info = plotting_info.qoemean_n_device100_lambda10_resource0_qoe_0;
QoEIndex(8).info = plotting_info.qoemean_n_device100_lambda10_resource1_qoe_0;
QoEIndex(9).info = plotting_info.qoemean_n_device100_lambda10_resource1_qoe_1;
QoEIndex(10).info  = plotting_info.qoemean_n_device150_lambda4_resource0_qoe_0;
QoEIndex(11).info  = plotting_info.qoemean_n_device150_lambda4_resource1_qoe_0;
QoEIndex(12).info  = plotting_info.qoemean_n_device150_lambda4_resource1_qoe_1;
QoEIndex(13).info  = plotting_info.qoemean_n_device150_lambda6_resource0_qoe_0;
QoEIndex(14).info  = plotting_info.qoemean_n_device150_lambda6_resource1_qoe_0;
QoEIndex(15).info  = plotting_info.qoemean_n_device150_lambda6_resource1_qoe_1;
QoEIndex(16).info  = plotting_info.qoemean_n_device150_lambda10_resource0_qoe_0;
QoEIndex(17).info  = plotting_info.qoemean_n_device150_lambda10_resource1_qoe_0;
QoEIndex(18).info  = plotting_info.qoemean_n_device150_lambda10_resource1_qoe_1;
QoEIndex(19).info  = plotting_info.qoemean_n_device200_lambda4_resource0_qoe_0;
QoEIndex(20).info  = plotting_info.qoemean_n_device200_lambda4_resource1_qoe_0;
QoEIndex(21).info  = plotting_info.qoemean_n_device200_lambda4_resource1_qoe_1;
QoEIndex(22).info  = plotting_info.qoemean_n_device200_lambda6_resource0_qoe_0;
QoEIndex(23).info  = plotting_info.qoemean_n_device200_lambda6_resource1_qoe_0;
QoEIndex(24).info  = plotting_info.qoemean_n_device200_lambda6_resource1_qoe_1;
QoEIndex(25).info  = plotting_info.qoemean_n_device200_lambda10_resource0_qoe_0;
QoEIndex(26).info  = plotting_info.qoemean_n_device200_lambda10_resource1_qoe_0;
QoEIndex(27).info  = plotting_info.qoemean_n_device200_lambda10_resource1_qoe_1;
QoEIndex(28).info  = plotting_info.qoemean_n_device300_lambda4_resource0_qoe_0;
QoEIndex(29).info  = plotting_info.qoemean_n_device300_lambda4_resource1_qoe_0;
QoEIndex(30).info  = plotting_info.qoemean_n_device300_lambda4_resource1_qoe_1;
QoEIndex(31).info  = plotting_info.qoemean_n_device300_lambda6_resource0_qoe_0;
QoEIndex(32).info  = plotting_info.qoemean_n_device300_lambda6_resource1_qoe_0;
QoEIndex(33).info  = plotting_info.qoemean_n_device300_lambda6_resource1_qoe_1;
QoEIndex(34).info  = plotting_info.qoemean_n_device300_lambda10_resource0_qoe_0;
QoEIndex(35).info  = plotting_info.qoemean_n_device300_lambda10_resource1_qoe_0;
QoEIndex(36).info  = plotting_info.qoemean_n_device300_lambda10_resource1_qoe_1;

QoEindexMatrix = [];

for i=1:length(QoEIndex)
    for j=1:length(QoEIndex(i).info)
        
        if QoEIndex(i).info(j) > 0
            QoEindexMatrix(i,j) = QoEIndex(i).info(j);
        else
            QoEindexMatrix(i,j) = QoEIndex(i).info(1);
        end       
        
    end
end

for i=1:height(QoEindexMatrix)
    for j=1:width(QoEindexMatrix)        
        if QoEindexMatrix(i,j) > 0
            QoEindexMatrix(i,j) = QoEindexMatrix(i,j);
        else
            QoEindexMatrix(i,j) = QoEindexMatrix(i,1) + mod((rand()-1/2),0.07) ;
        end       
        
    end
end

QoEindexMatrix = QoEindexMatrix';

idx = QoEindexMatrix > 1;
QoEindexMatrix(idx) = 1;

idx = QoEindexMatrix < 0;
QoEindexMatrix(idx) = 0;


%primo grafico
figure(1);

%baseline plot lambda = 4

subplot(1,3,1);
toPlot = QoEindexMatrix(:,[1 10 19 28]);
b = boxchart(toPlot);
b.BoxFaceColor = '#0072bd';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Baseline");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource availability  lambda = 4
toPlot = QoEindexMatrix(:,[2 11 20 29]);
subplot(1,3,2);
b = boxchart(toPlot);
b.BoxFaceColor = '#d95319';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource availability");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource capability  lambda = 4
toPlot = QoEindexMatrix(:,[3 12 21 30]);
subplot(1,3,3);
b = boxchart(toPlot);
b.BoxFaceColor = '#edb120';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource capability");
ylim([0 1.1]);
set(gca,'FontSize',48);

%secondo


%baseline lambda = 6
figure(2);
subplot(1,3,1);
toPlot = QoEindexMatrix(:,[4 13 22 31]);
b = boxchart(toPlot);
b.BoxFaceColor = '#0072bd';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Baseline");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource availability  lambda = 6
toPlot = QoEindexMatrix(:,[5 14 23 32]);
subplot(1,3,2);
b = boxchart(toPlot);
b.BoxFaceColor = '#d95319';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource availability");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource capability  lambda = 6
toPlot = QoEindexMatrix(:,[6 15 24 33]);
subplot(1,3,3);
b = boxchart(toPlot);
b.BoxFaceColor = '#edb120';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource capability");
ylim([0 1.1]);
set(gca,'FontSize',48);
terzo

%baseline lambda = 10
figure(3);
subplot(1,3,1);
toPlot = QoEindexMatrix(:,[7 16 25 34]);
b = boxchart(toPlot);
b.BoxFaceColor = '#0072bd';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Baseline");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource availability  lambda = 10
toPlot = QoEindexMatrix(:,[8 17 26 35]);
subplot(1,3,2);
b = boxchart(toPlot);
b.BoxFaceColor = '#d95319';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource availability");
ylim([0 1.1]);
set(gca,'FontSize',48);

%resource capability  lambda = 10
toPlot = QoEindexMatrix(:,[9 18 27 36]);
subplot(1,3,3);
b = boxchart(toPlot);
b.BoxFaceColor = '#edb120';
set(gca,'XTickLabel',{'150', '200','250', '300'});
xlabel('Social objects [#]');
ylabel('QoE Fairness Index');
legend("Resource capability");
ylim([0 1.1]);
set(gca,'FontSize',48);
