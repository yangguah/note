function [mag,ori] = mygradient(I)
%
% compute image gradient magnitude and orientation at each pixel
%
%
assert(ndims(I)==2,'input image should be grayscale');

dx = imfilter(I,[-1;1]);
dy = imfilter(I,[-1,1]);

mag = sqrt( (dx).^2 + (dy).^2 );
ori =  atan( dy ./dx );
mag(isnan(ori)) = 0;
ori(isnan(ori)) = 0;
assert(all(size(mag)==size(I)),'gradient magnitudes should be same size as input image');
assert(all(size(ori)==size(I)),'gradient orientations should be same size as input image');
