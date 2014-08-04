%!/opt/local/bin/octave -q
%
% (c) Copyright M. Shahab SANJARI 2011
%
% this is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% barion is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
%
% along with this software.  If not, see <http://www.gnu.org/licenses/>.

% new signal generator

close all;
clear all;
clc;
clf;


Fs = 11225;                    % Sampling frequency 1kHz
Ts = 1/Fs;                     % Sample time
L = 32700;                     % Length of signal
t = (0:L-1)*Ts;                % Time vector

y=cos(2*pi*440*t)+i*sin(2*pi*400*t);%+2*sin(2*pi*100*t); % 1V signal = 1 Watt auf 1 Ohm

     wavwrite(strcat('testsignal.wav'), abs(y), Fs, 32);

plot(t(1:100), real(y(1:100)));
title('Time Signal');
xlabel('Time [s]');
ylabel('Real y(t) [V]');


figure;
plot(t(1:100), imag(y(1:100)));
title('Time Signal');
xlabel('Time [s]');
ylabel('Imag y(t) [V]');


NFFT = 2^nextpow2(L); % Next power of 2 from length of y

Y=fft(y,NFFT)/L; % man kann auf 1 normieren

figure;
f= (Fs/NFFT).*(-NFFT/2:NFFT/2-1);
plot(f,abs(fftshift(Y)));
title('Shifted Double-Sided Amplitude Spectrum of y(t)');
xlabel('Frequency [Hz]');
ylabel('|Y(f)| [V]');

figure;
f= (Fs/NFFT).*(-NFFT/2:NFFT/2-1);
plot(f,abs(Y));
title('Non Shifted Double-Sided Amplitude Spectrum of y(t)');
xlabel('Frequency [Hz]');
ylabel('|Y(f)| [V]');


f2=figure;
f= (Fs/NFFT).*(0:NFFT/2-1);
Y(2:NFFT/2)=Y(2:NFFT/2)*2;
plot(f, abs(Y(1:NFFT/2)));
title('Single-Sided Amplitude Spectrum of y(t)');
xlabel('Frequency [Hz]');
ylabel('|Y(f)| [V]');

f3=figure;
f= (Fs/NFFT).*(0:NFFT/2-1);
plot(f, abs(Y(1:NFFT/2)).^2);
title('Single-Sided Power Spectrum of y(t)');
xlabel('Frequency [Hz]');
ylabel('|Y(f)|^2 [W]');

figure;
f= (Fs/NFFT).*(0:NFFT/2-1);
plot(f, 10*log10((abs(Y(1:NFFT/2)).^2)/0.001));
title('Single-Sided Power Spectrum of y(t)');
xlabel('Frequency [Hz]');
ylabel('|Y(f)|^2 [dBm]');

