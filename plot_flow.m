% Function to visualize OpticalFlow
function plot_flow(im, Z)
    % im is the original image
    % Z is the quantized flow vector(u+iv) matrix
    [H, W, d] =  size(im);
    %[nh,nw] = size(Z); % # windows 
    x = [1:10:W];
    y = [1:10:H];
    u = real(Z);
    v = imag(Z);
    
    imshow(im); hold on;
    quiver(x,y,u,v);
    
end