clear all

side_length = 100;
r0 = [0 0;
      side_length 0;
      side_length/2 sqrt(side_length^2*(1 - 0.25))];

%s0 [s0x s0y]
s0 = [0 0];

%r0 [r0x(1) r0y(1);
%	r0x(2) r0y(2);
%	r0x(3) r0y(3)];

c0 = 343;

tau1 = place([0 -10],r0,c0);

% A = [2*r0(2,1) 2*r0(2,2) 2*(c0^2)*tau1(1);
% 	2*r0(3,1) 2*r0(3,2) 2*(c0^2)*tau1(2)];
% 	
% b = [-(c0^2)*(tau1(1)^2) + r0(2,1).^2 + r0(2,2).^2;
% 	-(c0^2)*(tau1(2)^2) + r0(3,1).^2 + r0(3,2).^2];
% 
% m = lsqminnorm(A,b);

syms x y T1
eqns = [sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(2,1)).^2 +(y-r0(2,2)).^2) + tau1(1)*c0 ==0,sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(3,1)).^2 +(y-r0(3,2)).^2) + tau1(2)*c0==0];
vars = [x y];
m = solve(eqns,vars)

% [u, s, v] = svd(A');
% v = v';
% s_temp = inv(s(1:2,1:2));
% u_temp = u';
% u_temp = u_temp(:,1:2);
% m = v*s_temp*u_temp.'*b;

figure(1);
series1 = plot(r0(1,1),r0(1,2),'ro');
hold on
plot(r0(2,1),r0(2,2),'ro');
plot(r0(3,1),r0(3,2),'ro');
series2 = plot(m.x,m.y,'bo');
% series2 = plot(m(1),m(2),'bo');
series3 = fimplicit(@(x,y) sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(2,1)).^2 +(y-r0(2,2)).^2) + tau1(1)*c0,'b');
fimplicit(@(x,y) sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(3,1)).^2 +(y-r0(3,2)).^2) + tau1(2)*c0,'b');
hold off
grid on;

xlim([-100 200]);
ylim([-100 200]);

xlabel('x (m)');
ylabel('y (m)');
legend([series1 series3 series2],'Recorders','Locus of locations','Solution');

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

