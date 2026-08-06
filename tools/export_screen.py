import re
from PIL import Image

WIDTH = 128
HEIGHT = 64


def convert(buffer):
    img = Image.new("1", (WIDTH, HEIGHT))

    for page in range(8):
        for x in range(WIDTH):
            index = page * WIDTH + x

            if index >= len(buffer):
                continue

            value = buffer[index]

            for bit in range(8):
                y = page * 8 + bit

                if y >= HEIGHT:
                    continue

                pixel = 255 if (value & (1 << bit)) else 0

                img.putpixel(
                    (x, y),
                    pixel
                )

    return img


with open("dump.txt", "r") as f:
    text = f.read()


numbers = re.findall(
    r"0x[0-9A-Fa-f]{2}",
    text
)


buffer = [
    int(x, 16)
    for x in numbers
]


print(
    "Framebuffer bytes:",
    len(buffer)
)


img = convert(buffer)

img = img.resize(
    (256,128)
)

img.save(
    "resources/images/screens/gameplay.png"
)

print(
    "Saved gameplay.png"
)
