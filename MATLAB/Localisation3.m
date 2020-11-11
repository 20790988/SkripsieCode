function Localisation3(points,diffs,c)
    %diffs
    %1 2
    %1 3
    %2 3
    
    p1 = points(1,:);
    p2 = points(2,:);
    p3 = points(3,:);
    d = diffs*c;
    d12 = d(1);
    d13 = d(2);

    if (abs(d12)>dist(p1,p2) || abs(d13)>dist(p1,p3))
        fprintf('Difference larger than focus point spacing\n')
    else
        figure(1);
        plot(p1(1),p1(2),'ro');
        hold on
        plot(p2(1),p2(2),'ro');
        plot(p3(1),p3(2),'ro');
        fimplicit(@(x,y) sqrt( (x-p1(1)).^2 +(y-p1(2)).^2)  - sqrt( (x-p2(1)).^2 +(y-p2(2)).^2) - d12,'b');
        fimplicit(@(x,y) sqrt( (x-p1(1)).^2 +(y-p1(2)).^2)  - sqrt( (x-p3(1)).^2 +(y-p3(2)).^2) - d13,'b');
        hold off

        grid on
    end

    function distance = dist(a,b)
        distance = 0;
        for i = 1:length(a)
            distance = distance + (a(i)-b(i)).^2;
        end
        distance = sqrt(distance);
    end

    
end