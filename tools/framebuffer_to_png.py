from PIL import Image
import re
import sys

if len(sys.argv) != 3:
    print("Usage:")
    print("python3 framebuffer_to_png.py dump.txt output.png")
    exit()

with open(sys.argv[1], "r") as f:
    text = f.read()

numbers = re.findall(r'0x([0-9A-Fa-f]{2})', text)

data = [int(x, 16) for x in numbers]

print("Found", len(data), "bytes")

if len(data) != 1024:
    print("Expected 1024 bytes")
    exit()

img = Image.new("1", (128, 64))

for page in range(8):
    for x in range(128):
        byte = data[page * 128 + x]

        for bit in range(8):
            y = page * 8 + bit

            if byte & (1 << bit):
                img.putpixel((x, y), 1)

img.save(sys.argv[2])

print("Saved", sys.argv[2])
