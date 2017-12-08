clear;
close all;

% Global variables
file = '../log/batarang_population.csv';
outputname = '../img/boxplots.png';
to_plot = [4,7];

% Read in data
data = csvread(file,0,0);

% Parse data
x = [];
g = [];
labels = [];
runs = max(data(:,1));
cnt = 1;
for i = to_plot
    temp = data(data(:,1)==i, 2);
    x = [x;temp];
    g = [g;cnt*ones(size(temp))];
    labels = [labels;sprintf('% *s ', 2, num2str(i))];
    cnt = cnt + 1;
end
temp = data(:,2);
x = [x;temp];
g = [g;cnt*ones(size(temp))];
labels = [labels;'All'];

% Plot data
figure();
boxplot(x,g,'labels',labels);
ylim([-400,max(max(x))+10]);
xlabel('Run');
ylabel('Fitness');

% Save image
fig.PaperPositionMode = 'auto';
print(outputname,'-dpng','-r0')
