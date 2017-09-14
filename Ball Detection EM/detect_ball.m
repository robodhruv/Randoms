% Script for identifying the center of the ball(s) whose parameters have
% been learned as mu, sigma.
% Input: Image I; Parameters mu, sigma
% Other Parameters: Threshold, Structural Element
%
% Author: Dhruv Ilesh Shah

for i = 1:size(I, 1)
    for j = 1:size(I, 2)
        x = double(reshape(I(i, j, :), [1, 3]));
        p(i, j) = (exp(-0.5 * (x-mu)*inv(sigma)*(x-mu)'))/sqrt(2*pi*det(sigma));
    end
end

thres = 1e-4;

p1 = p > thres;
se = strel('disk', 4, 4); % Tune parameters accordingly
p2 = imerode(p1, se);

p3 = imdilate(p2, se);
p3 = imdilate(p3, se);
% Connect the components maybe? Dilation etc?
S = regionprops(p3, 'centroid');
centroids = cat(1, S.Centroid);

figure
imshow(p3)
hold on
plot(centroids(:,1),centroids(:,2), 'b*');
hold off

figure
imshow(I)
hold on
plot(centroids(:,1),centroids(:,2), 'k*')
hold off
