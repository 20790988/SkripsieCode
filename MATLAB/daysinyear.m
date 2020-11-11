function day_of_year = daysinyear(year, month, day)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    days_in_month = [ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 ];
    day_of_year = day;
    while ()
        day_of_year = day_of_year+ days_in_month(i);
    end

    if (mod(year, 4) == 0 && month > 2)
        day_of_year = day_of_year +1;
    end

end

