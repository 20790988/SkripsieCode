results = results_day2_trim;
l = size(results);
l = l(1);

is_clap_day1 = [1 1 0 0 1 1 0 0];
is_clap_day2 = [1 1 1 0 0 0 0 0 1 1];

is_condenser_day1 = [1 1 1 1 0 0 0 0];
is_condenser_day2 = [1 1 1 1 1 0 0 0 0 0];

% actuals = [9.9 9 8 7 6 5];
 actuals = [9.9 9 8 7 6 5 4 3 2 1 0.1];

% diffs = zeros(size(results));
% for i = 1:l
%     for j = 1:length(actuals)
%         if results(i,j)~=0
%             diffs(i,j) = results(i,j) - actuals(j)
%         end
%     end
% end
% figure(1)
% histogram(diffs((diffs~=0)),'binwidth',0.1)
% 
% diffs_day1 = diffs;

temp_diffs_day2 = diffs_day2(is_clap_day2==1,:);
temp_diffs_day1 = diffs_day1(is_clap_day1==1,:);
figure(1)
% collect = [temp_diffs_day2((temp_diffs_day2~=0)); temp_diffs_day1((temp_diffs_day1~=0))];
collect = [temp_diffs_day2((temp_diffs_day2~=0)); temp_diffs_day1((temp_diffs_day1~=0))];
histogram(collect,'binwidth',0.1,'Normalization','probability')
mu2 = std(collect)
% title(sprintf('Sound: Clapping\n N=%d\n 2nd moment about origin = %.4f',length(collect),mu2))
xlabel('Localisation error (m)')
ylabel('Fraction of counts')
grid

figure(2)
temp_diffs_day2 = diffs_day2(is_clap_day2==0,:);
temp_diffs_day1 = diffs_day1(is_clap_day1==0,:);
collect = [temp_diffs_day2((temp_diffs_day2~=0)); temp_diffs_day1((temp_diffs_day1~=0))];
histogram(collect,'binwidth',0.1,'Normalization','probability')
mu2 = std(collect)
% title(sprintf('Sound: Spoon-in-mug\n N=%d\n 2nd moment about origin = %.4f',length(collect),mu2))
xlabel('Localisation error (m)')
ylabel('Fraction of counts')
grid

figure(3)
temp_diffs_day2 = diffs_day2(is_condenser_day2==1,:);
temp_diffs_day1 = diffs_day1(is_condenser_day1==1,:);
collect = [temp_diffs_day2((temp_diffs_day2~=0)); temp_diffs_day1((temp_diffs_day1~=0))];
histogram(collect,'binwidth',0.1,'Normalization','probability')
mu2 = std(collect)
% title(sprintf('Microphone: Both Cheap\n N=%d\n 2nd moment about origin = %.4f',length(collect),mu2))
xlabel('Localisation error (m)')
ylabel('Fraction of counts')
grid

figure(4)
temp_diffs_day2 = diffs_day2(is_condenser_day2==0,:);
temp_diffs_day1 = diffs_day1(is_condenser_day1==0,:);
collect = [temp_diffs_day2((temp_diffs_day2~=0)); temp_diffs_day1((temp_diffs_day1~=0))];
histogram(collect,'binwidth',0.1,'Normalization','probability')
mu2 = std(collect)
% title(sprintf('Microphone: One Cheap, One Fancy\n N=%d\n 2nd moment about origin = %.4f',length(collect),mu2))
xlabel('Localisation error (m)')
ylabel('Fraction of counts')
grid
