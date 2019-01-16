from PIL import Image, ImageDraw

im = Image.open("keyboard.bmp")
draw = ImageDraw.Draw(im)

file = open("log.txt")
x = -1
y = -1
for line in file.readlines():
	p = line.split(" ")
	curx = int(float(p[0]))
	cury = int(float(p[1]))
	if (x != -1 or y != -1):
		draw.line((x,y,curx,cury), fill=128)
	x = curx
	y = cury
im.save("path.bmp")
del draw