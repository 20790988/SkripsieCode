function Localisation2(points,diffs,c)
    
    p1 = points(1,:);
    p2 = points(2,:);
    d = diffs*c;
   
    
    if (abs(d)>dist(p1,p2))
        fprintf('Difference larger than focus point spacing\n')
    else
        figure(1);
        series1 = plot(p1(1),p1(2),'ro');
        hold on
        plot(p2(1),p2(2),'ro');
        series2 = fimplicit(@(x,y) sqrt( (x-p1(1)).^2 +(y-p1(2)).^2)  - sqrt( (x-p2(1)).^2 +(y-p2(2)).^2) + d,'b');
        hold off
%         xlim([-200 200]);
%         ylim([-200 200]);
        legend([series1 series2],'Recorders','Locus of locations');
        xlabel('x (m)')
        ylabel('y (m)')
        title(sprintf("Time delay = %4.e s",diffs));
        grid on;
    end

    function distance = dist(a,b)
        distance = 0;
        for i = 1:length(a)
            distance = distance + (a(i)-b(i)).^2;
        end
        distance = sqrt(distance);
    end
end