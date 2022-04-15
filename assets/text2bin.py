import math
from PIL import Image, ImageFont, ImageDraw


class IntelHex:
    DATA    = 0
    EOF     = 1
    ESA     = 2
    SSA     = 3
    ELA     = 4
    SLA     = 5

    def __init__(self, data_width=32):
        self._hex_nums = math.ceil(data_width / 8)
        self._data = []
        self._addr = 0
    
    def write(self, data, record_type=DATA):
        str = f"{data:0{self._hex_nums}x}"
        print(str)
        if len(str) > self._hex_nums:
            print("Data is too large!! (Skipping)")
            return False
        line = f":{self._hex_nums:02x}{self._addr:04x}{record_type:02x}{str}"
        print(line)
        sum = 0
        for i in range(1, int(len(line) / 2), 2):
            print(i, line[i:i + 1])
            #sum += int(hex, )
        return True



def text2bmp(font_path, out_name, width=32, height=64):
    font = ImageFont.truetype(font_path, width)
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
    writer = IntelHex()
    writer.write(16)
    #text2bmp("C:/Users/James/OneDrive/Documents/ECE385/final/WARS-STARS-II/assets/fonts/telegrama/telegrama_render.otf", "title_font")