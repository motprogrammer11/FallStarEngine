#pragma once
#include "raylib.h"
#include <vector>
namespace eng {
    bool CheckCollisionBox2D(Rectangle rec1, float rotDeg1, Rectangle rec2, float rotDeg2);
    enum class BodyType {
        Static,  // Стены, пол (не двигаются, их не выталкиваем)
        Dynamic, // Игрок, враги (двигаются, выталкиваются из Static)
        Trigger  // Монетки, зоны перехода (не выталкивают, только фиксируют вход)
    };

    struct Collider {
        Rectangle bounds;      // Позиция (верхний левый угол) и размер
        float rotation = 0.0f; // Угол в градусах
        BodyType type = BodyType::Static;
        int tag = 0;           // Например: 1 = стена, 2 = лава, 3 = финиш
        bool isColliding = false; // Флаг: пересекается ли с чем-то прямо сейчас
    };

    // Базовый менеджер коллизий
    class CollisionSystem {
    public:
        std::vector<Collider*> colliders;

        void Register(Collider* col);
        void Unregister(Collider* col);

        // Главный метод: вызывать в конце кадра после движения объектов
        void Update();

        // Отрисовка хитбоксов для отладки (F1 / Debug mode)
        void DrawDebug();
    };

    // Низкоуровневая математика (уже на базе Box2D v3.2)
    bool CheckBox2D(const Collider& a, const Collider& b);
    bool ResolveBox2D(Collider& mover, const Collider& wall);
    // Проверка + автоматическое выталкивание первого прямоугольника из второго
    bool ResolveCollisionBox2D(Rectangle *mover, float moverRotDeg, Rectangle wall, float wallRotDeg);
    void CircleD(int centerX, int centerY, float radius, Color color);
    void RectD(Rectangle rec, Color color);
    void TextureD(Texture2D texture, int posX, int posY, Color color);
    void TextD(const char *text, int posX, int posY, int fontSize, Color color);
    void UnloadStandart();
    void LoadStar();
    void LoadMiniStar();
    void CloseStar();
    void CameraLoad(Camera2D *camera,Vector2 offset,float rotation,float zoom);
    void CameraUpdate(Camera2D *camera,Vector2 target);
    void DEBUG(int x,int y);
    void DebugUpdate();
    void ExampleTopDownMovement(Vector2 *vec);
    // Новые функции для работы с русским языком
    Font loadRusFont(const char *fileName, int fontSize);
    void drawRusText(Font font, const char *text, int posX, int posY, float fontSize, Color color);

} // namespace eng
