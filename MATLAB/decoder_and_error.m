function decoder_and_error(data)

    k = edge(data(:,1),0.175);
    l = count_peaks(k,72);
    m = trunc_zeros(l);
    n = toASCII(m);
    [r1,r2] = concat_info(n);
    
    error = 0;
    file_error = fopen('errors.txt','w');
    for j = 2:length(r1)
        previous_string = r1(j-1);

        chars = convertStringsToChars(previous_string);

        seconds = binarystring2int(chars(2:5));
        seconds = seconds+ binarystring2int(chars(7:9))*10;
        mins = binarystring2int(chars(11:14));
        mins = mins + binarystring2int(chars(16:18))*10;
        hours = binarystring2int(chars(21:24));
        hours = hours + binarystring2int(chars(26:27))*10;

        days = binarystring2int(chars(31:34));
        days = days + binarystring2int(chars(36:39))*10;
        days = days + binarystring2int(chars(41:42))*100;

        years = 2000;
        years = years + binarystring2int(chars(51:54));
        years = years + binarystring2int(chars(56:59))*10;
        
        predicted_string = next_message(seconds, mins, hours, days, years);
        temp_error = sum(predicted_string ~= convertStringsToChars(r1(j)));
        
        if temp_error>0
            fprintf(file_error,'\n\nMessage %d Index %d\nPred %s\nReal %s\n%d errors',j,r2(j),predicted_string,r1(j),temp_error);
            fprintf(file_error,'\n%.2d:%.2d:%.2d %d days %d',hours,mins,seconds,days,years);
        end
        error = error +temp_error;
    end
    
    fprintf('%d symbols, %d errors',100*length(r1),error);
    fclose(file_error);

    function edges = edge(input,level)
    % Detects where 1 dimensional input rises above level
    %   1 in output array -> threshold was crossed between this sample and the
    %   previous.
    %   0 otherwise.

        edges = zeros(size(input));
        for i = [2:length(input)-1]
            if (input(i-1)<level & input(i)>= level)
                edges(i) = 1;
            end
        end
    end

    function message = count_peaks(data,timeout)
    % Detects longest series of 1's with maximum distance between 1's of
    % timeout

        len = length(data);
        message = zeros([length(data) 2]);
        char_pos = 1;
        reading = 0;

        num_peaks = 0;
        last_one = 1;
        for i = 1:len
            if reading
                if (data(i) == 1)
                    num_peaks = num_peaks +1;
                    last_one = i;
                else
                    if (i-last_one>=timeout)
                        message(char_pos,1) = num_peaks;
                        message(char_pos,2) = i;
                        char_pos = char_pos+1;
                        num_peaks = 0;
                        reading = 0;
                    end
                end
            else
                if (data(i) == 1)
                    num_peaks = num_peaks +1;
                    reading = 1;
                    last_one = i;
                end
            end
        end
    end

    function new = trunc_zeros(input)
    %truncates array before first zero element
        last_zero_index = length(input);
        for i = [1:length(input)]
            if input(i) == 0
                last_zero_index = i;
                break
            end
        end
        new = input(1:last_zero_index-1,:);
    end

    function new = trunc_empty(input)
    %truncates array before first zero element
        last_zero_index = length(input);
        for i = [1:length(input)]
            if input(i) == ""
                last_zero_index = i;
                break
            end
        end
        new = input(1:last_zero_index-1,:);
    end

    function new = toASCII(input)
    % Detects where 1 dimensional input rises above level
    %   1 in output array -> threshold was crossed between this sample and the
    %   previous.
    %   0 otherwise.
        new = input;
        for i = [1:length(input)]
            switch new(i,1)
                case 2
                    new(i,1) = '0';
                case 5
                    new(i,1) = '1';
                case 8
                    new(i,1) = 'P';
                otherwise
                    new(i,1) = 'E';
            end
        end
    end

    function [result,result_indices] = concat_info(input)
        result_indices = zeros([length(input) 1]);
        result = strings([length(input) 1]);
        count_strings = 1;
        for i = [2:length(input)]
            if (input(i-1,1) == 'P' && input(i,1) == 'P' && length(input)-i>=100)
                result_indices(count_strings) = input(i,2);
                temp = char(input(i:i+99,1).');
                temp = convertCharsToStrings(temp);
                result(count_strings) = (temp);
                count_strings = count_strings +1;
            end
        end
        result_indices = trunc_zeros(result_indices);
        result = trunc_empty(result);

    end

    function num = binarystring2int(str)
        num = 0;
        len = size(str);
        len = len(2);
        for i = 1:len
            num = num + str2double(str(i))*power(2,i-1);
        end
    end
    
    function string = next_message(sec, min, hour, day_of_year, year)
        
        if sec+1 == 60
            min = min+1;
            sec = 0;
            if min == 60
                hour = hour+1;
                min = 0;
            end
        else 
            sec = sec+1;
        end
        
        string = '00000000P000000000P000000000P000000000P000000000P000000000P000000000P000000000P000000000P000000000P';
        string(1:1+3) = insert_binary_into_string(mod(sec, 10),4);
        string(6:6+2) = insert_binary_into_string(sec / 10,3);

        string(10:10+3) = insert_binary_into_string(mod(min, 10),4);
        string(15:15+2) = insert_binary_into_string(min / 10, 3);

        string(20:20+3) = insert_binary_into_string(mod(hour, 10),4);
        string(25:25+1) = insert_binary_into_string(hour / 10, 2);
        
        string(30:30+3) = insert_binary_into_string(mod(day_of_year, 10),4);
        string(35:35+3) = insert_binary_into_string(mod(day_of_year, 100) / 10,4);
        string(40:40+1) = insert_binary_into_string(day_of_year / 100,2);

        string(50:50+3) = insert_binary_into_string(mod(year,10),4);
        string(55:55+3) = insert_binary_into_string(mod(year,100) / 10,4);

        seconds_of_day = hour * 3600 + min * 60 + sec;

        for i = 0:16

            if (i < 9)

                string(80 + i) = '0' + bitand(bitshift(seconds_of_day, -i) , 1);

            else

                string(80 + i + 1) = '0' + bitand(bitshift(seconds_of_day, -i) , 1);
            end
        end
    
        string = ['P' string];
    end

    function ret_string = insert_binary_into_string(num, len)
        num = floor(num);
        ret_string = blanks(len);
        
        if (num < 10 && num < power(2, len))
        
            for i = 0:len-1
            
                ret_string(i+1) = '0' + bitand(bitshift(num, -i) , 1);
            end
        
        else
        
            for i = 0:len-1
            
                ret_string(i+1) = 'E';
            
            end
        end
    end
    
end