i=length(yaw);

figure;
plot([1:i],yaw,'r-',[1:i],theta,'b-');
legend('yaw','theta');

figure;
plot([1:i],speed,'r-',[1:i],rho,'b-');
legend('y-speed','rho');
