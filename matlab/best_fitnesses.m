clear;
close all;

% Global variables
file = '../log/rasbpi_layout.csv';
outputname = '../img/best_fitness.png';

% Read in data
data = csvread(file,1,0);

% Find last (aka best) fitness from each run
best = [];
j = 1;
for i=2:size(data)
    if(data(i,1) ~= data(i-1,1))
        best(j) = data(i-1,5);
        j = j + 1;
    end
end

fig = figure(); 

% Plot data
hold on;
plot(1:1:length(best),best,'-','color','blue');
hold off;
xlabel('Run');
ylabel('Fitness');
title('Best Fitness per Run');

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0');
