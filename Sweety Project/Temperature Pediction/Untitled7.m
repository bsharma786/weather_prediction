h=openfig('TimeResponse__max_temp_ANN.fig');
h=findobj(gca,'Type','Line');
x=get(h,'Xdata');
y=get(h,'Ydata');
A=[];
A(:,1)=y;
A(:,2)=x;
dlmwrite('data.txt',A,',')