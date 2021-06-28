function [Y,Xf,Af] = hum_NN(X,Xi,~)
%MYNEURALNETWORKFUNCTION neural network simulation function.
%
% Auto-generated by MATLAB, 28-Jun-2021 17:53:31.
%
% [Y,Xf,Af] = myNeuralNetworkFunction(X,Xi,~) takes these arguments:
%
%   X = 2xTS cell, 2 inputs over TS timesteps
%   Each X{1,ts} = 1xQ matrix, input #1 at timestep ts.
%   Each X{2,ts} = 1xQ matrix, input #2 at timestep ts.
%
%   Xi = 2x2 cell 2, initial 2 input delay states.
%   Each Xi{1,ts} = 1xQ matrix, initial states for input #1.
%   Each Xi{2,ts} = 1xQ matrix, initial states for input #2.
%
%   Ai = 2x0 cell 2, initial 2 layer delay states.
%   Each Ai{1,ts} = 10xQ matrix, initial states for layer #1.
%   Each Ai{2,ts} = 1xQ matrix, initial states for layer #2.
%
% and returns:
%   Y = 1xTS cell of 2 outputs over TS timesteps.
%   Each Y{1,ts} = 1xQ matrix, output #1 at timestep ts.
%
%   Xf = 2x2 cell 2, final 2 input delay states.
%   Each Xf{1,ts} = 1xQ matrix, final states for input #1.
%   Each Xf{2,ts} = 1xQ matrix, final states for input #2.
%
%   Af = 2x0 cell 2, final 0 layer delay states.
%   Each Af{1ts} = 10xQ matrix, final states for layer #1.
%   Each Af{2ts} = 1xQ matrix, final states for layer #2.
%
% where Q is number of samples (or series) and TS is the number of timesteps.

%#ok<*RPMT0>

% ===== NEURAL NETWORK CONSTANTS =====

% Input 1
x1_step1.xoffset = 1;
x1_step1.gain = 0.0666666666666667;
x1_step1.ymin = -1;

% Input 2
x2_step1.xoffset = 33;
x2_step1.gain = 0.0298507462686567;
x2_step1.ymin = -1;

% Layer 1
b1 = [2.8779562217189327278;-3.2917294225566369192;1.6929729525858208117;-0.86882260877002481791;-0.092763664011001806253;-0.84358248279065128905;-1.3552440404422081421;-1.877088389273879443;-3.1127327885213826875;-1.806459684143148392];
IW1_1 = [-1.9216074340946034393 -1.887555521345196885;1.3501521989741918084 -1.0953043569569675952;-2.0158844229270611592 -2.3443276745047012533;1.5964210571974284392 -1.5364133177358674232;-0.32975424046984103121 0.27553682973004306467;-1.8683813959307222774 -1.179840718190546589;-0.90601615814714830943 0.82464977036962316959;-2.1472306073751834532 -1.4780410026515502508;-0.93032640435351154906 -2.2268556931219216644;-0.91199829908213014829 0.69186873620453959166];
IW1_2 = [-0.90395412510625972047 0.010418869376920457392;-1.8237836062268217852 -0.74370173261273198051;1.5028631483107917077 0.91334751901870281365;-1.1552745389659762054 1.1727532474548807429;0.2298954886732309244 0.6555806936524855022;-0.92061335572578284481 0.77215521033025213971;-0.73798951259488387855 -0.0019327218520819103298;0.8528459337713027999 1.0649943853143506445;2.2004182366548921124 0.98073028111743332236;1.3922686955180039181 -2.9449703781236089561];

% Layer 2
b2 = -0.29708781942288653433;
LW2_1 = [-0.035095133658671243126 0.33603271925216415816 0.011965305436718748702 -0.54620602619413227696 0.87351675534647632571 -0.082502495184359592173 -0.63657708120666534057 0.042504408115127068069 0.0038250611866726021149 0.12715779771275959042];

% Output 1
y1_step1.ymin = -1;
y1_step1.gain = 0.0298507462686567;
y1_step1.xoffset = 33;

% ===== SIMULATION ========

% Format Input Arguments
isCellX = iscell(X);
if ~isCellX
    X = {X};
end
if (nargin < 2), error('Initial input states Xi argument needed.'); end

% Dimensions
TS = size(X,2); % timesteps
if ~isempty(X)
    Q = size(X{1},2); % samples/series
elseif ~isempty(Xi)
    Q = size(Xi{1},2);
else
    Q = 0;
end

% Input 1 Delay States
Xd1 = cell(1,3);
for ts=1:2
    Xd1{ts} = mapminmax_apply(Xi{1,ts},x1_step1);
end

% Input 2 Delay States
Xd2 = cell(1,3);
for ts=1:2
    Xd2{ts} = mapminmax_apply(Xi{2,ts},x2_step1);
end

% Allocate Outputs
Y = cell(1,TS);

% Time loop
for ts=1:TS
    
    % Rotating delay state position
    xdts = mod(ts+1,3)+1;
    
    % Input 1
    Xd1{xdts} = mapminmax_apply(X{1,ts},x1_step1);
    
    % Input 2
    Xd2{xdts} = mapminmax_apply(X{2,ts},x2_step1);
    
    % Layer 1
    tapdelay1 = cat(1,Xd1{mod(xdts-[1 2]-1,3)+1});
    tapdelay2 = cat(1,Xd2{mod(xdts-[1 2]-1,3)+1});
    a1 = tansig_apply(repmat(b1,1,Q) + IW1_1*tapdelay1 + IW1_2*tapdelay2);
    
    % Layer 2
    a2 = repmat(b2,1,Q) + LW2_1*a1;
    
    % Output 1
    Y{1,ts} = mapminmax_reverse(a2,y1_step1);
end

% Final Delay States
finalxts = TS+(1: 2);
xits = finalxts(finalxts<=2);
xts = finalxts(finalxts>2)-2;
Xf = [Xi(:,xits) X(:,xts)];
Af = cell(2,0);

% Format Output Arguments
if ~isCellX
    Y = cell2mat(Y);
end
end

% ===== MODULE FUNCTIONS ========

% Map Minimum and Maximum Input Processing Function
function y = mapminmax_apply(x,settings)
y = bsxfun(@minus,x,settings.xoffset);
y = bsxfun(@times,y,settings.gain);
y = bsxfun(@plus,y,settings.ymin);
end

% Sigmoid Symmetric Transfer Function
function a = tansig_apply(n,~)
a = 2 ./ (1 + exp(-2*n)) - 1;
end

% Map Minimum and Maximum Output Reverse-Processing Function
function x = mapminmax_reverse(y,settings)
x = bsxfun(@minus,y,settings.ymin);
x = bsxfun(@rdivide,x,settings.gain);
x = bsxfun(@plus,x,settings.xoffset);
end
