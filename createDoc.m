% creates documents in SVMLight format
tic

fid = fopen('trainDocCvpt40','w');
%vw = load('BagOfWordsCVpt40.txt');

for i = 1:size(vw,1)
	b = vw(i,:);
	c = find(b>10); % indices where count > 10
	n = length(c);
	for j = 1:n
		fprintf(fid,'%d:%d ',c(j),b(c(j)));
	end
	fprintf(fid,'\n');
end
fclose(fid);
toc	