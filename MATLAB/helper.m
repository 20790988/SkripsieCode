function s0 = helper(data1,data2,filename1,filename2,seconds_of_interest)
%     data1 = data14_1;
%     data2 = data14_2;
%     filename1 = 'data14_1.csv';
%     filename2 = 'data14_2.csv';
%     seconds_of_interest = [1 3 6 9 13 15 19 21 24 27 29];

    fs = 48000;

    c0 = 343;

    r0 = [0 0; 10 0];

    s0 = zeros(length(seconds_of_interest),2);

    tab = readtable(filename1);
    arr1 = table2array(tab);

    tab = readtable(filename2);
    arr2 = table2array(tab);

    second_of_day_0 = max(arr1(1,7),arr2(1,7))

    sound_of_interest1 = zeros(length(seconds_of_interest),fs*1.5);
    sound_of_interest2 = zeros(length(seconds_of_interest),fs*1.5);

    for i = 1:length(seconds_of_interest)
        for j1 = 1:length(arr1)
            if (arr1(j1,7)== second_of_day_0+seconds_of_interest(i))
                sound_of_interest1(i,:) = data1(arr1(j1,1):arr1(j1,1)+fs*1.5-1,2).';
                sound_of_interest1(i,:) = sound_of_interest1(i,:);
                %fprintf('arr1 i = %d second = %d start = %d\n',i,arr1(j1,7),arr1(j1,1));
                
                figure(1);
                plot(sound_of_interest1(i,:));
                xlabel('Samples')
                ylabel('Amplitude')
                title('a) Sound from recorder 1')
                grid on
            end
        end

        for j2 = 1:length(arr2)
            if (arr2(j2,7)== second_of_day_0+seconds_of_interest(i))
                sound_of_interest2(i,:) = data2(arr2(j2,1):arr2(j2,1)+fs*1.5-1,2).';
                sound_of_interest2(i,:) = sound_of_interest2(i,:);
                %fprintf('arr2 i = %d second = %d start = %d\n',i,arr2(j2,7),arr2(j2,1));
                
                figure(2);
                plot(sound_of_interest2(i,:));
                xlabel('Samples')
                ylabel('Amplitude')
                title('b) Sound from recorder 2')
                grid on
            end
        end

        [r,lags] = xcorr(sound_of_interest1(i,:),sound_of_interest2(i,:));
        [r,lags] = trim_r_lags(r,lags,r0,fs);
        [M,I] = max(r);
        time_delay = (lags(I))/fs
        
        figure(3);
        stem(lags/48000,r,'MarkerSize',0.1);
        xlabel('Delay (s)')
        ylabel('Magnitude')
        title('c) Cross-correlation')
        grid on
        figure(4);

        if (abs(time_delay*c0)>dist(r0(1,:),r0(2,:)))
            fprintf('Difference larger than focus point spacing for second %d\n',seconds_of_interest(i));
            plot([0,0])
        else
            x = sym('x');
            y = sym('y');
            eqns = [sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(2,1)).^2 +(y-r0(2,2)).^2) - time_delay*c0 ==0,y==0];
            vars = [x y];
            m = vpasolve(eqns,vars);
            s0(i,1) = m.x;
            s0(i,2) = m.y;
            leg1 = plot(r0(1,1),r0(1,2),'ro');
            text(r0(1,1),r0(1,2)+0.1,' 1');
            hold on;
            grid on;
            plot(r0(2,1),r0(2,2),'ro');
            text(r0(2,1),r0(2,2)+0.1,' 2');
            leg3 = plot(s0(i,1),s0(i,2),'bo');
            leg2 = fimplicit(@(x,y) sqrt( (x-r0(1,1)).^2 +(y-r0(1,2)).^2)  - sqrt( (x-r0(2,1)).^2 +(y-r0(2,2)).^2) - time_delay*c0,'b');
            plot([0 10],[0 0],'b')
            hold off;
            xlabel('x (m)')
            ylabel('y (m)')
            title ('d) Position of recorders and source')
            xlim([0 10])
            ylim([-3 3])
            
            legend([leg1 leg2 leg3],'Recorders','Loci','Source');
        end
    end

    for i_3 = 1:length(s0)
        fprintf('%.4f ',s0(i_3,1))
    end
    
    s0 = s0(:,1);

    function distance = dist(a,b)
        distance = 0;
        for i_2 = 1:length(a)
            distance = distance + (a(i_2)-b(i_2)).^2;
        end
        distance = sqrt(distance);
    end

    function [new_r,new_lags] = trim_r_lags(r,lags,r0,fs)
        max_lags = cast((dist(r0(1,:),r0(2,:))*fs/343),'int64');
        max_lags = cast(max_lags,'double');
        start_index = 0;
        stop_index = 0;

        for i_1 = 1:length(lags)
            if (lags(i_1)==-max_lags)
                start_index = i_1;
            else if (lags(i_1)==max_lags)
                    stop_index = i_1;
                end
            end
        end
        new_r = r(start_index:stop_index);
        new_lags = lags(start_index:stop_index);
    end
end