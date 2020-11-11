c0 = 343;

side_length = 100;
r0 = [0 0;
      side_length 0;
      side_length/2 sqrt(side_length^2*(1 - 0.25))];

% r0 = [side_length 0; 2*side_length 0; 3*side_length 0];
  
r0 = r0 + side_length;

[x1, y1] = meshgrid(-0.5*side_length:10:1.5*side_length);
x1 = x1+side_length;
y1 = y1+side_length;

z = zeros(length(x1),length(y1));

syms x positive;
syms y positive;

total = length(x1)*length(y1);
count = 1;

for i = 1:length(x1)
    for j = 1:length(y1)
        fprintf("%4d/%4d\n",count,total);
        
        tau1 = place([x1(i,j) y1(i,j)],r0,c0);
        tau1(1) = tau1(1);
        eqns = [sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(2,1)).^2 +(y-r0(2,2)).^2) + tau1(1)*c0 ==0,sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(3,1)).^2 +(y-r0(3,2)).^2) + tau1(2)*c0==0];
        vars = [x y];
        m = solve(eqns,vars);
        fprintf("placed %d %d, solved %.2f %.2f\n",x1(i,j),y1(i,j),m.x,m.y);
        z(i,j) = dist([m.x m.y],[x1(i,j) y1(i,j)]);
        count = count+1;
    end
end

function diffs = place(p, r0, c0)
    diffs = [0 0];
    diffs(1) = (dist(p,r0(2,:))-dist(p,r0(1,:)))/c0;
    diffs(2) = (dist(p,r0(3,:))-dist(p,r0(1,:)))/c0;
end

function distance = dist(a,b)
    distance = 0;
    for i = 1:length(a)
        distance = distance + (a(i)-b(i)).^2;
    end
    distance = sqrt(distance);
end