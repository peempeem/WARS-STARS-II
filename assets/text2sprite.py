from ctypes import sizeof
from PIL import Image, ImageFont, ImageDraw

string      = "WAR STARS II"
font_path   = "./fonts/space-age/space age.ttf"
size        = 64
out_path    = "./sprites"

if __name__ == "__main__":
    font = ImageFont.truetype(font_path, size)
    img = Image.new("RGBA", (0, 0))
    draw = ImageDraw.Draw(img)
    w, h = draw.textsize(string, font=font)
    o_w, o_h = font.getoffset(string)
    img = Image.new("RGBA", (w, h))
    draw = ImageDraw.Draw(img)
    draw.text((0, -o_h / 2), string, "white", font=font)
    filename = out_path + "/" + "_".join(string.split()) + ".png"
    img.save(filename)