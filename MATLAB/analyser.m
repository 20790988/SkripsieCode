function analyser(filename) 
    tab = readtable(filename);
    arr = table2array(tab);

    check_control_bits(arr(:,9),arr(:,10));

    figure;
    d = diff(arr(:,1));
    plot(d,'s');
    ylabel('Number of samples per second');
    xlabel('seconds');
    fprintf('\nSamples per second\n');
    collate(d);

    d_seconds_of_day = diff(arr(:,7));
    fprintf('\nDifference between seconds\n');
    collate(d_seconds_of_day);

    function diffs = diff(row)
        diffs = zeros([length(row)-1 1]);
        for i = [2:length(row)]
            diffs(i-1) = row(i) - row(i-1);
        end
    end

    function collate(diff)
        values = [min(diff):max(diff)];
        counts = zeros(length(values),1);

        for i = 1:length(diff)
            for j = 1:length(values)
                if (diff(i) == values(j))
                    counts(j) = counts(j)+1;
                end
            end
        end

        fprintf('values: counts\n');
        for i = 1:length(values)
            if(counts(i)~=0)
                fprintf('%d: %4d\n',values(i),counts(i));
            end
        end
    end

    function check_control_bits(b1,b2)
        control = false;
        for i = 1:length(b1)
            if(b1(i)~=0 || b2(i)~=0)
                fprintf("Non-zero control bit in message nr %d",i);
                control = true;
            end
        end

        if (~control)
            fprintf("All control bits zero");
        end
    end
end