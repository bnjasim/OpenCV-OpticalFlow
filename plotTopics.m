% Plotting 29 topics discovered by lda
function plotTopics(c)
	% c is a 13824 dimension vector containing beta parameter values
	im = imread('snaps/1.png');
	
	l = find(c > max(c)*0.25);
	length(l)
		
	imshow(im); hold on;
		
	for j = 1:length(l)
		d = mod(l(j),4); % the direction
		v = floor((l(j)-1)/4);
		x = mod(v,72) * 10 +5;
		y = floor(v/72) * 10 +5;
		
		if(d==1)
			color = 'r>';
		elseif(d==2)
			color = 'b<';
		elseif(d==3)
			color = 'gv';
		else
			color = 'y^';
		end	
		plot(x,y,color);
		hold on;
	end
end
