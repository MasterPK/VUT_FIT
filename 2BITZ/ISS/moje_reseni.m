%1
% Naètení ze souboru
[signal, vzorkovaci_frekvence]=audioread('xkrehl04.wav');
delka=length(signal);
cas=delka/vzorkovaci_frekvence;
pocet_symbolu=delka/16;

%2
% Prealokace nulami, rychlejší
s=zeros(1,pocet_symbolu);
index=1;
%Procházení každého osmého po 16 vzorcích
for index_binarnich = 8:16:delka
   if signal(index_binarnich) > 0
       s(index)=1;     
   elseif signal(index_binarnich) < 0
       s(index)=0;
   end
   index=index+1;
end

% normalizace os na 20 ms
vzorky=1:delka;
osa1=vzorky/vzorkovaci_frekvence;
osa2=(vzorky*16-8)/vzorkovaci_frekvence;
plot(osa1(1:320),signal(1:320),osa2(1:20),s(1:20),'o');
figure;

%3

B = [0.0192 -0.0185 -0.0185 0.0192];
A = [1.0000 -2.8870 2.7997 -0.9113];

% Nalezeni nulovych bodu a póly pøenosové funkce
zplane(B,A);
figure;

% Ovìøí jestli je stabilní a zároveò ukáže impulsní odezvu pro pøíklad 4
ukazmito(B,A,vzorkovaci_frekvence);
figure;
%4
% dolní propus
% výstupem je první graf v funkci ukazmito

%5

sf=filter(B,A,signal);
plot(osa1(1:320),signal(1:320));
hold on;
plot(osa1(1:320),sf(1:320));
%posun je zhruba 15 vzorkù doprava, zjištìno od ruky, dalo by se pøesnìji
%ale nejde zadávat jiná než celá èísla pøi vykreslování
%6
hold on;
sf_shift=circshift(sf,-15);
plot(osa1(1:320),sf_shift(1:320));
hold on;

ssb=zeros(1,pocet_symbolu);
index=1;
%Procházení každého osmého po 16 vzorcích
for index_binarnich = 8:16:delka
   if sf_shift(index_binarnich) > 0
       ssb(index)=1;     
   elseif sf_shift(index_binarnich) < 0
       ssb(index)=0;
   end
   index=index+1;
end
plot(osa2(1:20),ssb(1:20),'o');
figure;
%7
spatne=0;
%procházení všech prvkù, -15 kvùli posunu aby to sedìlo
for index_binarnich = 1:pocet_symbolu-15
   if xor(ssb(index_binarnich),s(index_binarnich))==1
       spatne=spatne+1;    
   end
end
% Spatne bity
disp(spatne);
% Chybovost v procentech
procenta_spatne=spatne/(pocet_symbolu-15)*100;
disp(procenta_spatne);
hold off;
%8
fourier_puvodni=fft(signal);
polovina_fourier_puvodni=abs(fourier_puvodni(1:8000));

fourier_filtrovany=fft(sf);
polovina_fourier_filtrovany=abs(fourier_filtrovany(1:8000));

osa_hz=1:8000;
plot(osa_hz,polovina_fourier_puvodni);
xlabel('f [Hz]');
figure;
plot(osa_hz,polovina_fourier_filtrovany);
xlabel('f [Hz]');
figure;

%9
hustota_pravdepodobnosti = hist(signal)/length(signal);
bar(hustota_pravdepodobnosti);
figure;

%10
[a, b] = xcorr(signal,'biased');
plot(b, a);
xlim([-50 50]);
figure;

%11
k0=a(delka); %delka=pocet vzorku
k1=a(delka+1);
k16=a(delka+16);

%12
x = linspace(min(signal), max(signal), 100);
[h,p,r] = hist2opt(signal(1:delka-1), signal(2:delka), x);
imagesc(-x,x,p);
axis xy;
colorbar;

%13
%vystup funkce hist2opt, integral se rovna 1 tudiz se jedná o správnou
%funkci

%14
%funkce hist2opt vraci vysledek R[1]
disp(r);
