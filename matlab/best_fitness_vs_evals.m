clear;
close all;

% Global variables
file = '../log/rasbpi_layout.csv';
outputname = '../img/fitness_vs_evals.png';

% Read in data
data = csvread(file,1,0);

% Narrow down to only data from the best run
[max,max_idx] = max(data);
[min,min_idx] = min(data);
best_run = data(max_idx(5),:);
best_run = best_run(1);
idxs = find(data(:,1)==best_run);
best_data = data(idxs,:);

fig = figure(); 

% Plot data
hold on;
plot(best_data(:,2),best_data(:,4),'--','color','blue');
plot(best_data(:,2),best_data(:,5),'-','color','blue');
hold off;
xlabel('Generation');
ylabel('Fitness');
ylim([-1500,max(5)+100]);
title(['Fitness vs Evals for Best Run (run ' num2str(best_run) ')']);

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0')
