# Software Architecture

---

## 1. Overview

The project follows a modular architecture where the user interface, gameplay logic, rendering system, entity management, and hardware abstraction are separated into independent modules. This organization makes the project easier to maintain, extend, and debug.

The gameplay is driven by periodic timer events and button events. During each timer tick, the game processes player input, updates all entities, performs collision detection, updates the game state, and renders the next frame on the OLED display.

## 2. High-Level Architecture

```mermaid
flowchart TD

Application

Application --> Screens
Application --> Game
Application --> Driver

Game --> Engine
Game --> Entity
Game --> Assets
Game --> Sound
Game --> GameConfig
Game --> Lane

Engine --> GameManager
Engine --> Renderer
Engine --> Collision
Engine --> Input

Driver --> OLED
Driver --> Button
Driver --> Timer
Driver --> Buzzer
```

## 3. Module Responsibilities

| Module | Responsibility |
|----------|----------------|
| Screens | Manage game screens and user interaction |
| Engine | Control gameplay logic and rendering |
| Entity | Define all game objects |
| Assets | Store sprite resources |
| Sound | Manage sound effects and provide sound enable/disable control |
| Driver | Provide hardware abstraction for OLED, buttons, timer, and buzzer |
### Engine Components

| Component | Responsibility |
|-----------|----------------|
| Game Manager | Initialize and update gameplay |
| Renderer | Draw game objects on the OLED |
| Collision | Detect collisions using AABB |
| Input | Process hardware button events |
### Game Entities

| Entity | Description |
|---------|-------------|
| Player | User-controlled aircraft |
| Bullet | Player projectile |
| Enemy | Regular enemies |
| Boss | Final stage boss |
| Boss Bullet | Boss projectile |

## 4. Runtime Flow

The runtime flow describes how the game processes one frame during execution. Every game tick updates user input, game entities, collision detection, rendering, and audio before displaying the next frame on the OLED display.

```mermaid
flowchart LR

A[Timer Update]

A --> B[Read Button Events]

B --> C[Update Player]

C --> D[Update Bullets]

D --> E[Update Enemies]

E --> F[Update Boss]

F --> G[Update Boss Bullets]

G --> H[Collision Detection]

H --> I[Update HP / Score / Level]

I --> J[Update Game State]

J --> K[Spawn Boss or Enemy]

K --> L[Render Frame]

L --> M[OLED Display]

M --> A
```
## 5. Game Logic Sequences

### 5.1 Game Startup Sequence

The startup sequence initializes the game engine, resets the gameplay state, creates all game entities, and starts the periodic game update timer.

```mermaid
sequenceDiagram

participant Screen
participant GameManager
participant Player
participant Enemy
participant Boss
participant Timer

Screen->>GameManager: SCREEN_ENTRY
GameManager->>GameManager: game_init()
GameManager->>GameManager: peashooter_reset()

GameManager->>Player: player_init()
GameManager->>Enemy: enemy_init()
GameManager->>Boss: boss_init()

GameManager->>GameManager: bullet_init()
GameManager->>GameManager: boss_bullet_init()

GameManager->>Timer: timer_set()
```
### 5.2 Player Input Sequence

When the player presses the MODE button, the game checks whether the player can fire. If a bullet slot is available, a new bullet is created and a firing sound is played.

```mermaid
sequenceDiagram

participant Player
participant Screen
participant Bullet
participant Sound

Player->>Screen: Press UP / DOWN / MODE

alt UP or DOWN

Screen->>Screen: Check move delay

Screen->>Player: Change lane

Player->>Player: lane_to_x()

Player->>Player: Update X position

else MODE

Screen->>Screen: Check fire delay

Screen->>Bullet: bullet_has_free_slot()

Bullet-->>Screen: Slot available

Screen->>Bullet: bullet_spawn()

Screen->>Screen: Reset fire timer

Screen->>Sound: sound_play(FIRE)

end
```
### 5.3 Enemy Spawn Sequence

Enemy spawning is performed periodically based on the current game level and difficulty mode. The game determines when a new enemy should appear, selects an enemy type, and places it into an available lane.

```mermaid
sequenceDiagram

participant Timer
participant Game
participant Enemy

Timer->>Game: Spawn timer expired

Game->>Game: Boss active?

alt Boss not active

Game->>Game: Check difficulty

Game->>Game: Select enemy type

Game->>Game: Select lane

Game->>Enemy: enemy_spawn()

end
```

### 5.4 Gameplay Update Sequence

Enemies are spawned periodically according to the current difficulty level. During each update, enemies move downward. If an enemy reaches the defense line or collides with the player, the player's HP decreases. Destroyed enemies are removed from the game.

```mermaid
sequenceDiagram

participant Timer
participant Screen
participant Player
participant Bullet
participant Enemy
participant Boss
participant Collision

Timer->>Screen: Update Tick

Screen->>Player: player_update()

Screen->>Bullet: bullet_update()

Screen->>Enemy: enemy_update()

Screen->>Boss: boss_update()

Screen->>Boss: boss_bullet_update()

Screen->>Collision: collision_check()

Screen->>Screen: Update HP

Screen->>Screen: Update Score

Screen->>Screen: Update Level

Screen->>Screen: Spawn Enemy

Screen->>Screen: Check Boss Spawn
```
### 5.5 Collision Detection Sequence

```mermaid
sequenceDiagram

participant Bullet
participant Collision
participant Enemy
participant BossBullet
participant Boss
participant Player
participant Game

Bullet->>Collision: Check Bullet vs Enemy

Collision->>Enemy: enemy_damage()

alt Enemy destroyed

Enemy-->>Game: Increase Score

Game->>Game: Update Level

end

Enemy->>Collision: Check Enemy vs Player

Collision->>Player: Reduce HP

BossBullet->>Collision: Check Boss Bullet vs Player

Collision->>Player: Reduce HP

Bullet->>Collision: Check Bullet vs Boss

Collision->>Boss: Reduce HP

alt Boss HP == 0

Boss-->>Game: Victory

end
```
### 5.6 Boss Battle

The boss battle begins after the player reaches the required level. Unlike regular enemies, the boss has multiple attack phases that change according to its remaining HP.

```mermaid
sequenceDiagram

participant Game
participant Boss
participant BossBullet
participant Player

Game->>Boss: boss_spawn()

loop Every Update

Boss->>Boss: Move

Boss->>Boss: Update fire timer

alt Fire timer expired

Boss->>Boss: Select attack pattern

alt HP > 20

Boss->>BossBullet: Spawn center bullet

else HP > 10

Boss->>BossBullet: Spawn two bullets

else HP <= 10

Boss->>BossBullet: Spawn two bullets

Boss->>Boss: Reduce fire delay

end

end

Player->>Boss: Bullet Hit

Boss->>Boss: Reduce HP

end

alt Boss HP == 0

Boss-->>Game: Victory

end
```
### 5.7 Game Ending

The game ends when either the player loses all HP or defeats the final boss.

```mermaid
sequenceDiagram

participant Player
participant Boss
participant Game
participant Screen

alt Player HP == 0

Player-->>Game: Game Over

Game->>Screen: Show Game Over Screen

else Boss HP == 0

Boss-->>Game: Victory

Game->>Screen: Show Victory Screen

end
```

## 6. Design Principles

The software architecture follows several embedded software engineering principles to improve maintainability and scalability.

### -Modular Design

Each subsystem is implemented in an independent module, including player control, enemy management, rendering, collision detection, and sound.

### -Event-Driven Programming

The game is updated through periodic timer events and button events instead of using a blocking loop.

### -Object Pool

Bullets, enemies, and boss bullets are allocated statically to eliminate dynamic memory allocation during gameplay.

### -Separation of Responsibilities

Rendering, game logic, input handling, collision detection, and sound management are implemented independently to reduce module coupling.

### -Reusability

All game entities share a common base entity structure, allowing common operations such as rendering and collision detection to be reused across different object types.
### -Static Memory Allocation

All gameplay objects, including players, enemies, bullets, and boss bullets, are allocated statically. This eliminates dynamic memory allocation during runtime, providing deterministic memory usage and improving reliability on resource-constrained embedded systems.
## 7. Source Code Organization

The software is organized into independent modules to simplify maintenance and future development.

| Directory              | Responsibility                                              |
| ---------------------- | ----------------------------------------------------------- |
| `sources/app/`         | Screen management and application logic                     |
| `sources/game/engine/` | Game manager, rendering, collision detection, and input     |
| `sources/game/entity/` | Player, enemy, boss, bullets, and shared entity definitions |
| `sources/game/assets/` | Bitmap sprites and graphical resources                      |
| `sources/game/`        | Shared gameplay configuration, lane mapping, and utilities  |
| `sources/driver/`      | OLED, buttons, timer, buzzer, and other hardware drivers    |

