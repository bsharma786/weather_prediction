clc
clear all
close
T = readtable('Final_dataset.xlsx');
%dataset_temp = T{:,:}
x1 = T(:,3);
input = transpose(x1{:,:});

y1 = T(:,5);
target_avg_temp = transpose(y1{:,:});



z1 = T(:,11);
target_avg_hum = transpose(z1{:,:});












