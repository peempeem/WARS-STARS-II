import math
from PIL import Image, ImageFont, ImageDraw
from ast import literal_eval


class IntelHex:
    DATA    = 0
    EOF     = 1
    ESA     = 2
    SSA     = 3
    ELA     = 4
    SLA     = 5

    def __init__(self, data_width=32):
        self._hex_nums = math.ceil(data_width / 4)
        self._data = []
        self._addr = 0
    
    def write(self, data, record_type=DATA):
        str = f"{data:0{self._hex_nums}x}"
        if len(str) > self._hex_nums:
            print("Data is too large!! (Skipping)")
            return False
        line = f":{int(self._hex_nums / 2):02x}{self._addr:04x}{record_type:02x}{str}"
        sum = 0
        for i in range(1, len(line), 2):
            sum = sum + literal_eval(f"0x{line[i:i+2]}")
        sum = (256 - (sum % 256)) % 256
        line = f"{line}{sum:02x}"
        self._data.append(line)
        self._addr += 1
        return True
    
    def save(self, filename):
        with open(f"{filename}.hex", "w") as f:
            for line in self._data:
                f.write(f"{line}\n")
            f.write(":00000001FF\n")
        
        print(f"Saved {len(self._data)} entries to {filename}.hex")



def text2bmp(font_path, out_name, width=32, height=64, bias=4):
    font = ImageFont.truetype(font_path, width + bias)
    images = []
    
    for i in range(0, 128):
        img = Image.new(mode="L", size=(width, height))
        mask = Image.Image()._new(font.getmask(chr(i)))
        dx = int((img.width - mask.width) / 2) + 1
        dy = int((img.height - mask.height) / 2)
        img.paste(mask, (dx, dy))
        images.append(img)
    
    writer = IntelHex(width)
    for i in range(0, 128):
        for y in range(images[i].height):
            num = 0
            for x in range(images[i].width):
                if (images[i].getpixel((x, y)) != 0):
                    num = num + 1
                num = num << 1
            #print(f"{num:0{images[i].width}b}")
            writer.write(num)
    writer.save(out_name)


if __name__ == "__main__":
    text2bmp("C:/Users/James/OneDrive/Documents/ECE385/final/WARS-STARS-II/assets/fonts/telegrama/telegrama_render.otf", "assets/large_font")
    text2bmp("C:/Users/James/OneDrive/Documents/ECE385/final/WARS-STARS-II/assets/fonts/space-mono/SpaceMono-Regular.ttf", "assets/normal_font", width=8, height=16)