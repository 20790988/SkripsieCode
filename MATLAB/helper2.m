results = zeros(10,11);

results(1,:) = helper(data04_1,data04_2,'data04_1.csv','data04_2.csv',[1 4 7 10 13 16 19 22 26 29 31]).';
results(2,:) = helper(data05_1,data05_2,'data05_1.csv','data05_2.csv',[1 4 7 10 13 16 19 22 25 28 31]).';
results(3,:) = helper(data06_1,data06_2,'data06_1.csv','data06_2.csv',[1 4 7 10 13 16 19 22 25 28 30]).';
results(4,:) = helper(data07_1,data07_2,'data07_1.csv','data07_2.csv',[2 5 8 11 13 16 19 22 24 27 29]).';
results(5,:) = helper(data08_1,data08_2,'data08_1.csv','data08_2.csv',[2 5 7 11 14 17 19 22 25 27 30]).';
results(6,:) = helper(data09_1,data09_2,'data09_1.csv','data09_2.csv',[3 6 9 13 16 19 22 25 28 31 34]).';
results(7,:) = helper(data11_1,data11_2,'data11_1.csv','data11_2.csv',[1 4 7 10 13 16 19 22 25 27 30]).';
results(8,:) = helper(data12_1,data12_2,'data12_1.csv','data12_2.csv',[2 4 7 10 13 15 18 21 23 26 28]).';
results(9,:) = helper(data13_1,data13_2,'data13_1.csv','data13_2.csv',[1 4 7 10 13 16 19 22 25 28 30]).';
results(10,:) = helper(data14_1,data14_2,'data14_1.csv','data14_2.csv',[1 3 6 9 13 15 19 21 24 27 29]).';

% results = zeros(8,6);
% 
% results(1,:) = helper(data06_1,data06_2,'data06_1.csv','data06_2.csv',[3 7 11 15 18 22]).';
% results(2,:) = helper(data07_1,data07_2,'data07_1.csv','data07_2.csv',[1 4 8 11 14 17]).';
% results(3,:) = helper(data08_1,data08_2,'data08_1.csv','data08_2.csv',[4 8 8 16 20 24]).';
% results(4,:) = helper(data09_1,data09_2,'data09_1.csv','data09_2.csv',[4 11 15 19 13 26]).';
% results(5,:) = helper(data15_1,data15_2,'data15_1.csv','data15_2.csv',[9 12 16 19 23 26]).';
% results(6,:) = helper(data16_1,data16_2,'data16_1.csv','data16_2.csv',[7 10 13 16 19 22]).';
% results(7,:) = helper(data17_1,data17_2,'data17_1.csv','data17_2.csv',[9 12 15 18 23 25]).';
% results(8,:) = helper(data18_1,data18_2,'data18_1.csv','data18_2.csv',[1 4 7 9 12 15]).';

