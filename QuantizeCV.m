% Function to plot out optical flow in 10x10 windows found using OpenCV cvCalcOpticalFlowLK
% and to quantize the averaged flow to 4 directions-right,left,up or down

    im = imread('302.png'); % find the OpticalFlow of each pixel in u+vi form
	im1 = rgb2gray(im);
    [H,W] = size(im1); % H = 480, W = 720 for MIT dataset
    nh = H/10; nw = W/10; % Assuming they are divisible by 10
    u = load('ux.m');
	v = load('vy.m');
    A = u + i*v; % The averaged optical flow in 10x10 windows
    Z = zeros(size(A)); % To store quantized optical flow 
    
    for x = 1:nh
        for y = 1:nw
            %A(x,y) = sum(sum( V(1+(x-1)*10:x*10 , 1+(y-1)*10:y*10) ))/100; % average of a 10x10 cell
            % Now having obtained the optical flow, we want to quantize it
            % ie, restricting the flow direction to either right,left,up or down
            if(abs(A(x,y)) > 0.25) % tune it(increase) to remove unwanted flows 
                if(abs(real(A(x,y))) > abs(imag(A(x,y))))
                    Z(x,y) = sign(real(A(x,y)));
                else
                    Z(x,y) = sign(imag(A(x,y)))*i;
                end
            end
        end
    end
    
plot_flow(im1, Z);
               
            
    
    
    