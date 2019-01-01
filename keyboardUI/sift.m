a = imread('mouse.bmp');
picSize = size(a);
x = picSize(1);
y = picSize(2);

mask = zeros(x, y, 3);
for i = 1:x
    for j = 1:y
        if (a(i, j, 1) == 51 && a(i, j, 2) == 51 && a(i, j, 3) == 51)
            mask(i, j, 1) = 255;
            mask(i, j, 2) = 255;
            mask(i, j, 3) = 255;
        end
    end
end

imshow(mask);
imwrite(mask, 'mask.bmp');