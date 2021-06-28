clc
clear all
close all
T = readtable('Temp.csv');

A = T(1:24,1);
x1 = A{:,:};
B = T(1:24,2);
y1 = B{:,:};


X1 = log(x1);
%X2 = log(x2);
Y = log(y1);
X =[ones(length(x1),1) X1];
[B,~,~,~,STATS]= regress(Y,X);
alpha= exp(B(1));
beta=B(2);
%gamma=B(3);
ymodel= alpha*x1.^beta;

 