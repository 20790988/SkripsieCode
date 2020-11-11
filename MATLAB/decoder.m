function decoder(data,filename)

    k = edge(data(:,1),0.175);
    l = count_peaks(k,72);
    m = trunc_zeros(l);
    n = toASCII(m);
    [r1,r2] = concat_info(n);
    print_to_file(filename,r1,r2);

    % [r1,r2] = concat_info(toASCII(trunc_zeros(count_peaks(edge(data(:,1),0.175),72))));

    print_to_screen(r1,r2);

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

    function print_to_screen(result,result_indices)
    for i = [1:length(result)]
        str = result(i);

        chars = convertStringsToChars(str);

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

        seconds_of_day = binarystring2int([chars(81:89) chars(91:98)]);

        checkseconds = hours*3600 + mins*60 + seconds;

        fprintf("Index %7d: %02d:%02d:%02d %3d days of year %4d\t %d(%d) seconds of day\n",result_indices(i),hours,mins,seconds,days,years,seconds_of_day,checkseconds);
    end
    end

    function print_to_file(filename,result,result_indices)
    file = fopen(filename,'w');
    fprintf(file,'Index,Hours,Mins,Seconds,Days,Year,Seconds_of_day,Check_seconds_of_day,Control_bits1,Control_bits2\n');

    for i = [1:length(result)]
        str = result(i);

        chars = convertStringsToChars(str);

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

        control_bits1 = chars(61:69);
        control_bits2 = chars(71:79);

        seconds_of_day = binarystring2int([chars(81:89) chars(91:98)]);

        checkseconds = hours*3600 + mins*60 + seconds;

        fprintf(file,"%d,%d,%d,%d,%d,%d,%d,%d,%s,%s\n",result_indices(i),hours,mins,seconds,days,years,seconds_of_day,checkseconds,control_bits1,control_bits2);
    end
    fclose(file);
    end

end