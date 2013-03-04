Motion-learning
===============
%% clean up
close all;clear all;
%% Extract frames from video
video = VideoReader('Image15.avi');
TotalFrames = get(video,'numberOfFrames');
FrameWidth = video.Width;
FrameHieght = video.Height;
data = zeros(FrameHieght,FrameWidth,TotalFrames);
for i = 1:TotalFrames;
    temp =read(video,i);
    data(:,:,i) = temp(:,:,2);%storing blue channel
end
%% creating delta image
for i=1:TotalFrames-1;
     delta(:,:,i) = data(:,:,i+1) - data(:,:,i);
end
%list of salient/important fibers
for i =1:FrameWidth;
    for j = 1:FrameHieght;
        temp_fiber = delta(j,i,:);
        %need to find a trick to do this loop faster
        for k = 1:TotalFrames-1;
            data_fiber(k) = temp_fiber(1,1,k);
        end
        fourier_data = fftshift(fft(data_fiber));
        fourier_data_mag = abs(fourier_data);
        fourier_data_phase = unwrap(angle(fourier_data));
        vote(j,i) = sum(fourier_data_mag);
    end
end
% %from output it appears that taking sum will also
% work but the output is not so good unfortunately 
% The output of the above fiber method is much better
% as it has higher brightness value for the areas
% where there is motion in the video

sum_data = zeros(FrameHieght,FrameWidth);
sum_delta = zeros(FrameHieght,FrameWidth);
for i=1:TotalFrames-1;
sum_data = sum_data + abs(data(:,:,i));
sum_delta = sum_delta + abs(delta(:,:,i));
end
%% saliency video
for i=1:TotalFrames -1;
    saliency(:,:,i) = ESR(delta(:,:,i),5);
end
sum_saliency = zeros(FrameHieght,FrameWidth);
for i = 1:TotalFrames-1;
    sum_saliency = sum_saliency + saliency(:,:,i);
end
figure,imshow(saliency(:,:,10),[]),title('10th saliency map')
figure,imshow(sum_saliency,[]),title('sum of saliency images');
figure,imshow(vote,[]),title('Important regions computed using fft approach');
figure,imshow(sum_data,[]),title('Using simple sum approach');
figure, imshow(sum_delta,[]),title('Using sum of delta images');
%% clean up
close all;clear all;
%% Extract frames from video
video = VideoReader('Image15.avi');
TotalFrames = get(video,'numberOfFrames');
FrameWidth = video.Width;
FrameHieght = video.Height;
data = zeros(FrameHieght,FrameWidth,TotalFrames);
for i = 1:TotalFrames;
    temp =read(video,i);
    data(:,:,i) = temp(:,:,2);%storing blue channel
end
%% creating delta image
for i=1:TotalFrames-1;
     delta(:,:,i) = data(:,:,i+1) - data(:,:,i);
end
%list of salient/important fibers
for i =1:FrameWidth;
    for j = 1:FrameHieght;
        temp_fiber = delta(j,i,:);
        %need to find a trick to do this loop faster
        for k = 1:TotalFrames-1;
            data_fiber(k) = temp_fiber(1,1,k);
        end
        fourier_data = fftshift(fft(data_fiber));
        fourier_data_mag = abs(fourier_data);
        fourier_data_phase = unwrap(angle(fourier_data));
        vote(j,i) = sum(fourier_data_mag);
    end
end
% %from output it appears that taking sum will also
% work but the output is not so good unfortunately 
% The output of the above fiber method is much better
% as it has higher brightness value for the areas
% where there is motion in the video

sum_data = zeros(FrameHieght,FrameWidth);
sum_delta = zeros(FrameHieght,FrameWidth);
for i=1:TotalFrames-1;
sum_data = sum_data + abs(data(:,:,i));
sum_delta = sum_delta + abs(delta(:,:,i));
end
%% saliency video
for i=1:TotalFrames -1;
    saliency(:,:,i) = ESR(delta(:,:,i),5);
end
sum_saliency = zeros(FrameHieght,FrameWidth);
for i = 1:TotalFrames-1;
    sum_saliency = sum_saliency + saliency(:,:,i);
end
figure,imshow(saliency(:,:,10),[]),title('10th saliency map')
figure,imshow(sum_saliency,[]),title('sum of saliency images');
figure,imshow(vote,[]),title('Important regions computed using fft approach');
figure,imshow(sum_data,[]),title('Using simple sum approach');
figure, imshow(sum_delta,[]),title('Using sum of delta images');
