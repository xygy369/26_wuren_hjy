%% 第二问：轨迹跟踪
clear; clc; close all

% 车辆参数
lfr = 2.168 + 1.907; % 轴距 L
dt = 0.01;
v = 15; 
sim_steps = 2000;    %总步长

% 参考轨迹 (正弦曲线)
X_ref = 0:0.1:200; %0  0.1   0.2  0.3   200一串数字
Y_ref = 10 * sin(X_ref / 15); 

% 初始车辆状态 
X = X_ref(1); Y = Y_ref(1) + 3; phi = 0; %初始位置
X_vec = zeros(1, sim_steps); Y_vec = zeros(1, sim_steps);%生成表格


for ii = 1:sim_steps
    X_vec(ii) = X; Y_vec(ii) = Y;
    
    
    % ===============================================================
    
    % ================= TODO 2.1: 实现某种跟踪算法 =================
   
    %纯跟踪
    % 参数Ld：前视距离（3到4倍的轴距
    Ld = 10;
    
    % ① 找到参考轨迹上离车最近的点
    dist = sqrt((X_ref - X).^2 + (Y_ref - Y).^2);
    [~, idx] = min(dist);   %忽略最小值本身，保留索引
    
    % ② 从最近点向后搜索，找到距离等于 Ld 的点作为目标点
    target_idx = idx;
    for j = idx:length(X_ref)
        d = sqrt((X_ref(j) - X)^2 + (Y_ref(j) - Y)^2);
        if d >= Ld
            target_idx = j;
            break;
        end
    end
    X_target = X_ref(target_idx);
    Y_target = Y_ref(target_idx);
    
    % ③ 计算夹角 α 并归一化到 [-pi, pi]
    alpha = atan2(Y_target - Y, X_target - X) - phi;
    alpha = mod(alpha + pi, 2*pi) - pi;
    
    % ④ 纯跟踪转向角公式
    sigma = atan2(2 * lfr * sin(alpha), Ld);

    % ===============================================================

    % ================= TODO 2.2: 车辆状态更新 =================
    % 提示: 将刚才求得的转向角 sigma 代入运动学模型（复用第一问代码），更新 X, Y, phi。
    
    
    phi_dot = v * tan(sigma) / lfr;
    phi     = phi + phi_dot * dt;
    X       = X + v * cos(phi) * dt;
    Y       = Y + v * sin(phi) * dt;
    
    % ===============================================================
    
    % 到达终点提前结束
    if X >= X_ref(end), break; end
end   %以上操作每0.01s重复执行一次

% 绘图对比
figure; hold on; grid on;
plot(X_ref, Y_ref, 'k--', 'LineWidth', 2);
plot(X_vec(1:ii), Y_vec(1:ii), 'r-', 'LineWidth', 2);
legend('参考规划轨迹', '实际行驶轨迹');
title(['Pure Pursuit 跟踪 (Ld = ', num2str(Ld), 'm)']);
xlabel('X [m]'); ylabel('Y [m]'); axis equal;