<div align="center">
  <img src="assets/logo.png" alt="FallStar Engine Logo" width="420">

  # ⭐ FallStar Engine (v1.1)

  **A lightweight 2D game engine in C++ built on top of raylib and Box2D.**
  
  **Лёгкий 2D-игровой движок на C++, построенный поверх raylib и Box2D.**

  [English](#-english) • [Русский](#-русский)
</div>

---

## 🇬🇧 English

### 🌠 Philosophy

> *Every game is like a falling star: it sparks from an idea, shines brightly, flies at the peak of its popularity, and eventually fades away gracefully — never truly vanishing, leaving a warm trace in players' memories.*

**FallStar Engine** is built for creating complete, self-contained 2D games with a clear beginning, climax, and strong finale — without unnecessary bloat or endless complexity.

### ✨ Features (v1.1)

* **Window & Lifecycle Management:** `LoadStar()` (fullscreen with auto-monitor detection), `LoadMiniStar()` (800×450 windowed mode), and unified resource cleanup via `CloseStar()`.
* **Collision System (Box2D v3.2):**
  * Supports static walls (`Static`), dynamic bodies (`Dynamic`), and trigger zones (`Trigger`).
  * Accurate collision detection and resolution even for **rotated rectangles (OBB)**.
  * Built-in debug hitbox renderer (`DrawDebug()`).
* **2D Camera:** Smooth target following (`CameraUpdate`) with `Delta Time` support.
* **Cyrillic Out of the Box:** `loadRusFont()` and `drawRusText()` with automatic loading of full Russian character codepoints (including `Ё/ё`).
* **Automatic Resource Manager:** Tracks loaded textures, fonts, and sounds for safe unloading on exit.
* **JSON Support:** Integrated `nlohmann/json` for configs and upcoming save systems.

### 🛠️ Installation & Build (Arch Linux)

1. **Install dependencies:**
   ```bash
   sudo pacman -S raylib box2d nlohmann-json gcc
   ```

2. **Clone and compile:**
   ```bash
   git clone https://github.com
   cd FallStarEngine

   g++ main.cpp Engine/FallStarEngine.cpp -IEngine -o game -lraylib -lbox2d -lm
   ./game
   ```

### 🚀 Quick Start

```cpp
#include "FallStarEngine.hpp"

int main() {
    // Birth of a star (initialize window and audio)
    eng::LoadMiniStar();

    // Setup collision system
    eng::CollisionSystem physics;
    eng::Collider player = { { 100.0f, 200.0f, 36.0f, 36.0f }, 0.0f, eng::BodyType::Dynamic };
    eng::Collider wall   = { { 320.0f, 180.0f, 180.0f, 30.0f }, 35.0f, eng::BodyType::Static };

    physics.Register(&player);
    physics.Register(&wall);

    // Setup 2D camera
    Camera2D cam = { 0 };
    eng::CameraLoad(&cam, { 400.0f, 225.0f }, 0.0f, 1.0f);

    while (!WindowShouldClose()) {
        Vector2 pos = { player.bounds.x, player.bounds.y };
        eng::ExampleTopDownMovement(&pos);
        player.bounds.x = pos.x;
        player.bounds.y = pos.y;

        physics.Update();
        eng::CameraUpdate(&cam, { player.bounds.x, player.bounds.y });

        BeginDrawing();
        ClearBackground({ 25, 28, 36, 255 });

        BeginMode2D(cam);
            eng::RectD(player.bounds, GREEN);
            physics.DrawDebug();
        EndMode2D();

        eng::TextD("FallStar Engine v1.1", 20, 20, 20, WHITE);
        EndDrawing();
    }

    // Fading of a star (free all memory and close window)
    eng::CloseStar();
    return 0;
}
```

### 🗺️ Roadmap

- [x] Basic rendering & raylib wrappers (`v1.0`)
- [x] Cyrillic font support (`v1.0`)
- [x] Box2D v3.2 collision & trigger system (`v1.1`)
- [x] 2D camera & resource cleanup manager (`v1.1`)
- [ ] Startup splash screen with engine logo (`v1.1`)
- [ ] Save system & level loading via `nlohmann/json` (`v1.2`)
- [ ] Sprite sheet animator (`v1.2`)

---

## 🇷🇺 Русский

### 🌠 Философия проекта

> *Каждая игра подобна падающей звезде: она зарождается из искры идеи, ярко вспыхивает, летит на пике своей популярности и однажды красиво угасает — не исчезая бесследно, а оставляя тёплый след в памяти игроков.*

**FallStar Engine** создан для разработки законченных, цельных 2D-игр со своим началом, кульминацией и сильным финалом — без лишней громоздкости и бесконечного усложнения.

### ✨ Возможности (v1.1)

* **Управление окном и жизненным циклом:** функции `LoadStar()` (полноэкранный режим с автоопределением монитора), `LoadMiniStar()` (оконный режим 800×450) и единая очистка ресурсов через `CloseStar()`.
* **Система коллизий на базе Box2D v3.2:**
  * Поддержка статичных стен (`Static`), динамических тел (`Dynamic`) и зон-триггеров (`Trigger`).
  * Точная проверка столкновений и выталкивание даже для **повёрнутых под углом прямоугольников (OBB)**.
  * Встроенная отладочная отрисовка хитбоксов (`DrawDebug()`).
* **2D-камера:** плавное слежение за целью (`CameraUpdate`) с учётом `Delta Time`.
* **Кириллица «из коробки»:** функции `loadRusFont()` и `drawRusText()` с автоматической загрузкой всех кодовых точек русского алфавита (включая `Ё/ё`).
* **Автоматический менеджер ресурсов:** отслеживание загруженных текстур, шрифтов и звуков с безопасной выгрузкой при закрытии игры.
* **Поддержка JSON:** интеграция с `nlohmann/json` для конфигов и будущей системы сохранений.

### 🛠️ Установка и сборка (Arch Linux)

1. **Установка зависимостей:**
   ```bash
   sudo pacman -S raylib box2d nlohmann-json gcc
   ```

2. **Клонирование и компиляция:**
   ```bash
   git clone https://github.com
   cd FallStarEngine

   g++ main.cpp Engine/FallStarEngine.cpp -IEngine -o game -lraylib -lbox2d -lm
   ./game
   ```

### 🚀 Быстрый старт

```cpp
#include "FallStarEngine.hpp"

int main() {
    // Зарождение звезды (инициализация окна и аудио)
    eng::LoadMiniStar();

    // Настройка системы коллизий
    eng::CollisionSystem physics;
    eng::Collider player = { { 100.0f, 200.0f, 36.0f, 36.0f }, 0.0f, eng::BodyType::Dynamic };
    eng::Collider wall   = { { 320.0f, 180.0f, 180.0f, 30.0f }, 35.0f, eng::BodyType::Static };

    physics.Register(&player);
    physics.Register(&wall);

    // Настройка 2D-камеры
    Camera2D cam = { 0 };
    eng::CameraLoad(&cam, { 400.0f, 225.0f }, 0.0f, 1.0f);

    while (!WindowShouldClose()) {
        Vector2 pos = { player.bounds.x, player.bounds.y };
        eng::ExampleTopDownMovement(&pos);
        player.bounds.x = pos.x;
        player.bounds.y = pos.y;

        physics.Update();
        eng::CameraUpdate(&cam, { player.bounds.x, player.bounds.y });

        BeginDrawing();
        ClearBackground({ 25, 28, 36, 255 });

        BeginMode2D(cam);
            eng::RectD(player.bounds, GREEN);
            physics.DrawDebug();
        EndMode2D();

        eng::TextD("FallStar Engine v1.1", 20, 20, 20, WHITE);
        EndDrawing();
    }

    // Угасание звезды (освобождение всей памяти и закрытие окна)
    eng::CloseStar();
    return 0;
}
```

### 🗺️ Планы развития (Roadmap)

- [x] Базовый рендеринг и обёртки над raylib (`v1.0`)
- [x] Поддержка кириллических шрифтов (`v1.0`)
- [x] Система коллизий и триггеров на Box2D v3.2 (`v1.1`)
- [x] 2D-камера и менеджер очистки ресурсов (`v1.1`)
- [ ] Стартовый сплеш-скрин с логотипом движка (`v1.1`)
- [ ] Система сохранений и загрузки уровней через `nlohmann/json` (`v1.2`)
- [ ] Аниматор спрайт-листов (`v1.2`)

---

## 📄 License / Лицензия

Distributed under the **MIT License**. / Проект распространяется под лицензией **MIT**.
