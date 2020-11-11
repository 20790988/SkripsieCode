[data08_1, fs]= audioread('c:\Users\Kobus\OneDrive - Stellenbosch University\Skripsie\Progress Reports\2020-10-28\Test1\data08_1.wav');
[data08_2, fs]= audioread('c:\Users\Kobus\OneDrive - Stellenbosch University\Skripsie\Progress Reports\2020-10-28\Test1\data08_2.wav');

decoder(data08_1,'data08_1.csv');
decoder(data08_2,'data08_2.csv');

analyser('data08_1.csv');
analyser('data08_2.csv');