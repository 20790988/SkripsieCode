function Plotter(data,fs)
    l = size(data);
    figure;
    if l(2) == 1
        l = l(1);
        t = 1:l;
        t = t/fs;
        plot(t,data);
        xlabel('t (s)')
        ylabel('Amplitude')
    else
        l = l(1);
        t = 1:l;
        t = t/fs;
        ch1 = data(:,1);
        ch2 = data(:,2);
        ax1 = subplot(2,1,1);
        plot(t,ch1);
        xlabel('t (s)')
        ylabel('Amplitude')
        title('Ch1')
        ax2 = subplot(2,1,2);
        plot(t,ch2);
        xlabel('t (s)')
        ylabel('Amplitude')
        title('Ch2')
        linkaxes([ax1 ax2],'x')
    end
    
    
end
