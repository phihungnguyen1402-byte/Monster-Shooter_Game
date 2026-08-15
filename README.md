# Monster_Shooter_Game

## Introduction

- This kit integrates 1.54" OLED LCD, 3 push buttons, and 1 buzzer, which would be sufficient to create a small video game with an event driven paradigm.
- It also includes RS485, Qwiic Connect System, and Grove Ecosystems, suitable for prototyping other practical applications in embedded systems.

## Gameplay Demo

<p align="center">
  <img src="resources/images/screens/monster-shooter-banner.gif" width="1000">
</p>

<h3 align="center">Normal Gameplay</h3>

<div align="center">

<video
src="
https://github.com/user-attachments/assets/533b893c-3640-409c-a472-ff8249c435cf"
controls
width="640">
</video>

</div>

## Documentation

| File | Description |
|---|---|
| [README.md](README.md) | Main project overview, hardware information, gameplay rules, and game objects. |
| [docs/01-guide_getting-started.md](docs/01-guide_getting-started.md) | Getting started guide for building and running the Monster Shooter game. |
| [docs/02-project-overview.md](docs/02-project-overview.md) | Project overview, game structure, gameplay flow, and main game components. |
| [docs/03-software-architecture.md](docs/03-software-architecture.md) | Software architecture, object relationships, runtime flow, and event-driven design of the game. |
### I. Hardware:
[<img src="hardware/images/ak-embedded-base-kit-version-3.jpg" width="480"/>](<https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu>)

The game is designed to run on the AK Embedded Base Kit. It utilizes a **1.54" OLED display** for graphics rendering, **three push buttons** for user input, and a **buzzer** for audio feedback. For more information about the board, see [AK Embedded Base Kit](https://epcb.vn/products/ak-embedded-base-kit-lap-trinh-nhung-vi-dieu-khien-mcu).

## II. Game Description and Objects

Monster Shooter is an embedded shooting game developed for the AK Embedded Base Kit. 
The player controls a spaceship, destroys different types of monsters, avoids enemy attacks, 
and fights the boss to progress through the game.

### Objects in the Game

The game consists of several interactive objects that control the gameplay:

| Bitmap | Object Name | Description |
|:---:|:---|:---|
| <img src="resources/images/bitmap/player.png" width="110"> | **Player** | The main player-controlled spaceship. The player moves horizontally and fires projectiles to destroy enemies. |
| <img src="resources/images/bitmap/bullet_fighter.png" width="110"> | **Player Bullet** | A projectile specifically fired by the Player Tank. It travels upward and damages enemies and the boss. |
| <img src="resources/images/bitmap/player_tank.png" width="110"> | **Player Tank** | An alternative player character style that can be selected in the game settings. It has a robust design. |
| <img src="resources/images/bitmap/bullet_lightning.png" width="110"> | **Tank Bullet** | A projectile fired by the Player. It travels upward and damages enemies and the boss. |
| <img src="resources/images/bitmap/player_archer.png" width="110"> | **Player Archer** | Another alternative player character style available for selection, designed for precision. |
| <img src="resources/images/bitmap/bullet_arrow.png" width="110"> | **Archer Bullet** | An arrow-shaped projectile fired by the Player Archer. It travels upward and damages enemies and the boss. |
| <img src="resources/images/bitmap/enemy.png" width="110"> | **Normal Enemy** | The standard enemy type. It moves toward the player and can damage the player on collision. |
| <img src="resources/images/bitmap/enemy_tank.png" width="110"> | **Tank Enemy** | A durable enemy with higher health and requires more attacks to destroy. |
| <img src="resources/images/bitmap/enemy_fast.png" width="110"> | **Fast Enemy** | A fast-moving enemy that increases gameplay difficulty. |
| <img src="resources/images/bitmap/boss.png" width="110"> | **Boss** | A powerful enemy that appears at the boss level. The boss has high HP and launches projectiles at the player. |

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/demo.png" width="350" /></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 1:</em></strong> Demo Screen </p>

<table align="center">
  <tr>
    <td align="center"><img src="resources/images/settings.png" width="350" /></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 2:</em></strong> Settings Screen </p>

By navigating the menu, players can access the **Settings Screen**, which offers the following options to configure the game: 
- **PLAY**: Start a new match and enter the main game.
- **SOUND**: Toggle the in-game audio ON or OFF.
- **MODE**: Select the difficulty level from 3 available modes (e.g., Normal, Hard).
- **STYLE**: Choose the player's character or spaceship design (e.g., Fighter).


<table align="center">
  <tr>
    <td align="center"><img src="resources/images/play.png" width="350" /></td>
  </tr>
</table>
<p align="center"><strong><em>Figure 3:</em></strong> Gameplay Screen </p>

Once **PLAY** is selected, players transition to the **Gameplay Screen**. This screen displays the active combat area along with a top status bar tracking the current **SCORE**, **LEVEL**, and remaining **HP** (Health Points).


### III. How to Play:

- Press **DOWN** to move the player left.
- Press **UP** to move the player right.
- Press **MODE** to fire bullets.
- Destroy enemies to increase the score.
- Avoid enemy collisions and boss bullets.
- Defeat the boss to progress through the game.
- The game ends when the player's HP reaches zero.

### IV. Basic Game Sequence Logic

The following sequence diagram describes the basic runtime logic of **Monster Shooter**, including game initialization, player input, enemy spawning, bullet movement, collision detection, level progression, boss warning, boss battle, and game-over handling.

```mermaid
sequenceDiagram

    participant Player
    participant Screen
    participant Game
    participant Bullet
    participant Enemy
    participant Boss
    participant BossBullet
    participant Collision

    note over Player,Screen: SCREEN_ENTRY

    Player->>Screen: Start Game
    Screen->>Game: game_init()
    Screen->>Game: peashooter_reset()

    Game->>Game: Reset score
    Game->>Game: Reset level
    Game->>Game: Reset player HP
    Game->>Bullet: Reset bullets
    Game->>Enemy: Reset enemies
    Game->>Boss: Reset boss
    Game->>BossBullet: Reset boss bullets

    Screen->>Screen: STATE(GAME_PLAY)

    note over Player,Screen: GAME_PLAY

    loop Game Update Tick

        Player->>Screen: Input
        Screen->>Game: Update player

        Game->>Bullet: Update bullets
        Game->>Enemy: Update enemies

        alt Boss level reached

            Game->>Game: Check boss level

            alt Boss not spawned

              Game->>Screen: Show Boss Warning
              Game->>Game: Start warning timer

              loop Warning Timer
                  Game->>Screen: Update warning display
                  Game->>Game: Decrease warning timer
              end

              Game->>Boss: boss_spawn()

            end

            Screen->>Boss: boss_update()
            Screen->>BossBullet: boss_bullet_update()

        else Normal level

            Game->>Enemy: Spawn enemy

            alt Normal Enemy

                Enemy->>Game: NORMAL enemy

            else Tank Enemy

                Enemy->>Game: TANK enemy

            else Fast Enemy

                Enemy->>Game: FAST enemy

            end

        end

        note over Bullet,Enemy: Player Bullet Collision

        Bullet->>Collision: Check Bullet vs Enemy

        alt Bullet hits Enemy

            Collision->>Enemy: enemy_damage()

            alt Enemy destroyed

                Enemy->>Game: Remove enemy
                Enemy->>Game: Increase score
                Game->>Game: Check level progression
                Game->>Game: Play explosion sound

            else Enemy survives

                Enemy->>Game: Keep enemy alive
                Game->>Game: Play hit sound

            end

        end

        note over Bullet,Boss: Player Bullet vs Boss

        Bullet->>Collision: Check Bullet vs Boss

        alt Bullet hits Boss

            Collision->>Boss: boss_damage()

            alt Boss destroyed

                Boss->>Game: Remove boss
                Game->>Game: Increase score +100
                Game->>Game: Increase level
                Game->>Screen: Return to normal gameplay

            else Boss survives

                Boss->>Game: Continue boss battle

            end

        end

        note over Enemy,Player: Enemy vs Player

        Enemy->>Collision: Check Enemy vs Player

        alt Enemy reaches player

            Collision->>Game: Decrease Player HP
            Enemy->>Game: Remove enemy

            alt HP > 0

                Game->>Screen: Continue Game

            else HP == 0

                Game->>Screen: GAME_OVER

            end

        end

        note over BossBullet,Player: Boss Bullet vs Player

        BossBullet->>Collision: Check Boss Bullet vs Player

        alt Boss Bullet hits Player

            Collision->>Game: Decrease Player HP
            BossBullet->>Game: Remove boss bullet

            alt HP > 0

                Game->>Screen: Continue Boss Battle

            else HP == 0

                Game->>Screen: GAME_OVER

            end

        end

    end

    note over Screen,Game: GAME_OVER

    Screen->>Game: Stop gameplay
    Game->>Screen: Show Game Over
    Screen->>Player: Display Final Score
```
## Contact & Support
``` Note
Thank you for visiting this repository.
If you have any questions, suggestions, or feedback about this project, feel free to contact me directly.
```
