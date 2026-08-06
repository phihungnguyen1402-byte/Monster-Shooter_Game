import os
import re
from PIL import Image

SPRITES = [

    (
        "application/sources/app/game/assets/sprite_player.h",
        "player_sprite",
        16,
        16,
        "resources/images/bitmap/player.png"
    ),

    (
        "application/sources/app/game/assets/sprite_player_archer.h",
        "archer_player_sprite",
        16,
        16,
        "resources/images/bitmap/player_archer.png"
    ),

    (
        "application/sources/app/game/assets/sprite_player_tank.h",
        "tank_player_sprite",
        16,
        16,
        "resources/images/bitmap/player_tank.png"
    ),

    (
        "application/sources/app/game/assets/sprite_enemy.h",
        "enemy_sprite",
        8,
        8,
        "resources/images/bitmap/enemy.png"
    ),

    (
        "application/sources/app/game/assets/sprite_enemy_fast.h",
        "enemy_fast_sprite",
        8,
        8,
        "resources/images/bitmap/enemy_fast.png"
    ),

    (
        "application/sources/app/game/assets/sprite_enemy_tank.h",
        "enemy_tank_sprite",
        8,
        8,
        "resources/images/bitmap/enemy_tank.png"
    ),

    (
        "application/sources/app/game/assets/sprite_boss.h",
        "boss_sprite",
        16,
        16,
        "resources/images/bitmap/boss.png"
    ),

    (
        "application/sources/app/game/assets/sprite_bullet_arrow.h",
        "arrow_bullet_sprite",
        8,
        6,
        "resources/images/bitmap/bullet_arrow.png"
    ),

    (
        "application/sources/app/game/assets/sprite_bullet_lightning.h",
        "lightning_bullet_sprite",
        8,
        8,
        "resources/images/bitmap/bullet_lightning.png"
    ),

    (
        "application/sources/app/game/assets/sprite_bullet_fighter.h",
        "fighter_bullet_sprite",
        8,
        4,
        "resources/images/bitmap/bullet_fighter.png"
    ),



]

os.makedirs("resources/images/bitmap", exist_ok=True)

for header, varname, width, height, output in SPRITES:

    print("Export:", output)

    with open(header) as f:
        text = f.read()

    binary_numbers = re.findall(r'0b[01]+', text)
    hex_numbers = re.findall(r'0x[0-9a-fA-F]+', text)

    data = []

    for x in binary_numbers:
        data.append(int(x, 2))

    for x in hex_numbers:
        data.append(int(x, 16))

    expected = height * (width // 8)

    if len(data) < expected:
        print(
            f"ERROR: {header}: need {expected} bytes, found {len(data)}"
        )
        continue

    img = Image.new("1", (width, height))

    bytes_per_row = width // 8

    for y in range(height):

        row = 0

        for b in range(bytes_per_row):
            row <<= 8
            row |= data[y * bytes_per_row + b]

        for x in range(width):

            bit = (row >> (width - 1 - x)) & 1

            img.putpixel((x, y), bit)

    img = img.resize((width * 20, height * 20), Image.NEAREST)

    img.save(output)

print()
print("Done.")