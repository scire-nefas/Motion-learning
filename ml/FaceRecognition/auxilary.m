% Copyright 2013 Syed Tabish, Palash Pandya. All rights reserved.
files = dir;
mkdir('data');
for i = 3:size(files,1);
    dirname = num2str(files(i).name);
    image = imread(dirname); 
    dirname = dirname(1:9);
    mkdir(dirname);
    cd(dirname);
    filename = [num2str(length(dir)),'.bmp'];
    %pixels = uint8(CMUPIEData(i).pixels);
    imwrite(image,filename);
    cd ..;
end

files = dir;
for i = 3:size(files,1)
 movefile(files(i).name,['00',num2str(i-1)]);
 files(i).name
end


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% fd = fopen('TEST3.txt','r');
% i = 1;
% data = [];
% while( ~feof(fd) )
%   data = [data;transpose(fscanf(fd,'%d %d %f',3))];
%   i = i+1;
% end
% data
% fclose(fd);
% 
% 
% eigen_count = ceil(max(data(:,1))/10);
% K_count = ceil(max(data(:,2))/2);
% % for j = 1:K_count
% %   st = 1;
% %   figure;
% %   plot(data(st:K_count:end,1),data(st:K_count:end,3),'b*');
% %   hold on;
% %   plot(data(st:K_count:end,1),data(st:K_count:end,3));
% %   title('change in accuracy with Number of Eigen Vectors');
% % end
% % st = 1*K_count;
% % figure;
% % plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'b*');
% % hold on;
% % plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3));
% % title('change in accuracy with change in K value');
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% st = 1;
% figure;
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'b*');
% hold on;
% plot(data(st:K_count:end,1),data(st:K_count:end,3));
% st = 2;
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'r*');
% plot(data(st:K_count:end,1),data(st:K_count:end,3), 'Color',[1 0 0]);
% st = 3;
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'g*');
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'Color',[0 1 0]);
% st = 3;
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'k*');
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'Color',[0 0 0]);
% st = 4;
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'y*');
% plot(data(st:K_count:end,1),data(st:K_count:end,3),'Color',[1 1 0]);
% xlabel('Number Of Eigen Vector Used -->');
% ylabel('Accuracy(%) -->');
% title('Change In Accuracy With Number Of Eigen Vectors');
% %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% st = 1*K_count;
% figure;
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'b*');
% hold on;
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'Color',[0 0 1]);
% st = 2*K_count;
% 
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'r*');
% hold on;
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'Color',[1 0 0]);
% st = 3*K_count;
% 
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'g*');
% hold on;
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'Color',[0 1 0]);
% st = 4*K_count;
% 
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'k*');
% hold on;
% plot(data(st+1:st+K_count,2),data(st+1:st+K_count,3),'Color',[0 0 0]);
% 
% xlabel('Value Of K in K-NN classifier -->');
% ylabel('Accuracy(%)-->');
% title('change in accuracy with change in K value');
