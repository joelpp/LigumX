from PIL import Image
from scipy import misc

img = Image.open("../textures/building_side1.png");
img.load();
imOut = img.convert("RGBA");
print(imOut);
imOut.save("../textures/building_side1.png");