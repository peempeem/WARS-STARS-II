import string
from PIL import Image, ImageFont, ImageDraw


def text2bmp(font_path, out_name, width=32, height=64):
    font = ImageFont.truetype(font_path, width)
    
    print("found")
    
    images = []
    
    for i in range(0, 127):
        img = Image.new(mode="L", size=(width, height))
        mask = Image.Image()._new(font.getmask(chr(i)))
        dx = int((img.width - mask.width) / 2)
        dy = int((img.height - mask.height) / 2)
        img.paste(mask, (dx, dy))
        images.append(img)
    
    for i in range(0, 127):
        #img = images[i].convert(mode="1")
        #img.show()
        for y in range(images[i].height):
            for x in range(images[i].width):
                if (images[i].getpixel((x, y)) != 0):
                    print("x", end="")
                else:
                    print(" ", end="")
            print("")



if __name__ == "__main__":
    text2bmp("C:/Users/James/OneDrive/Documents/ECE385/final/WARS-STARS-II/assets/fonts/telegrama/telegrama_render.otf", "title_font")