#include "FallStarEngine.hpp"
#include "nlohmann/json.hpp"
#include <raylib.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include "raymath.h"
#include "box2d/box2d.h"
namespace eng {
    bool ISdebug=false;
    static b2Transform GetTransform(const Collider& col) {
        b2Vec2 center = { col.bounds.x + col.bounds.width * 0.5f, col.bounds.y + col.bounds.height * 0.5f };
        return { center, b2MakeRot(col.rotation * DEG2RAD) };
    }

    bool CheckBox2D(const Collider& a, const Collider& b) {
        b2Polygon boxA = b2MakeBox(a.bounds.width * 0.5f, a.bounds.height * 0.5f);
        b2Polygon boxB = b2MakeBox(b.bounds.width * 0.5f, b.bounds.height * 0.5f);
        b2Transform xf = b2InvMulTransforms(GetTransform(a), GetTransform(b));

        return b2CollidePolygons(&boxA, &boxB, xf).pointCount > 0;
    }

    bool ResolveBox2D(Collider& mover, const Collider& wall) {
        b2Polygon boxA = b2MakeBox(mover.bounds.width * 0.5f, mover.bounds.height * 0.5f);
        b2Polygon boxB = b2MakeBox(wall.bounds.width * 0.5f, wall.bounds.height * 0.5f);

        b2Transform tA = GetTransform(mover);
        b2Transform tB = GetTransform(wall);
        b2Transform xf = b2InvMulTransforms(tA, tB);

        b2LocalManifold manifold = b2CollidePolygons(&boxA, &boxB, xf);

        if (manifold.pointCount > 0) {
            float maxPen = 0.0f;
            for (int i = 0; i < manifold.pointCount; i++) {
                float pen = -manifold.points[i].separation;
                if (pen > maxPen) maxPen = pen;
            }

            if (maxPen > 0.0f) {
                b2Vec2 worldNormal = b2RotateVector(tA.q, manifold.normal);
                mover.bounds.x -= worldNormal.x * maxPen;
                mover.bounds.y -= worldNormal.y * maxPen;
            }
            return true;
        }
        return false;
    }

    void CollisionSystem::Register(Collider* col) {
        colliders.push_back(col);
    }

    void CollisionSystem::Unregister(Collider* col) {
        colliders.erase(std::remove(colliders.begin(), colliders.end(), col), colliders.end());
    }

    void CollisionSystem::Update() {
        // 1. Сбрасываем флаги столкновений перед новым кадром
        for (auto* c : colliders) {
            c->isColliding = false;
        }

        // 2. Проверяем каждый динамический объект против остальных
        for (size_t i = 0; i < colliders.size(); i++) {
            Collider* a = colliders[i];
            if (a->type != BodyType::Dynamic) continue;

            for (size_t j = 0; j < colliders.size(); j++) {
                if (i == j) continue;
                Collider* b = colliders[j];

                if (b->type == BodyType::Static) {
                    // Твёрдая стена -> выталкиваем
                    if (ResolveBox2D(*a, *b)) {
                        a->isColliding = true;
                        b->isColliding = true;
                    }
                }
                else if (b->type == BodyType::Trigger) {
                    // Триггер -> только фиксируем пересечение
                    if (CheckBox2D(*a, *b)) {
                        a->isColliding = true;
                        b->isColliding = true;
                    }
                }
            }
        }
    }

    void CollisionSystem::DrawDebug() {
        for (auto* c : colliders) {
            Color color = GREEN;
            if (c->type == BodyType::Static) color = BLUE;
            if (c->type == BodyType::Trigger) color = YELLOW;
            if (c->isColliding) color = RED;

            Rectangle rec = {
                c->bounds.x + c->bounds.width * 0.5f,
                c->bounds.y + c->bounds.height * 0.5f,
                c->bounds.width,
                c->bounds.height
            };
            DrawRectanglePro(rec, { c->bounds.width * 0.5f, c->bounds.height * 0.5f }, c->rotation, Fade(color, 0.4f));
        }
    }
    void ExampleTopDownMovement(Vector2 *vec ){
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) vec->x += 2.0f;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))  vec->x -= 2.0f;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W))    vec->y -= 2.0f;
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S))  vec->y += 2.0f;
    }
    void DebugUpdate(){
        if (IsKeyUp(KEY_F3)){
            ISdebug=!ISdebug;
        }

    }
    void DEBUG(int x,int y ){
        if(ISdebug){
            DrawFPS(x,y );
        }

    }
    void CameraLoad(Camera2D *camera, Vector2 offset, float rotation, float zoom) {
        camera->target = { 0.0f, 0.0f };
        camera->offset = offset;
        camera->rotation = rotation;
        camera->zoom = zoom;
    }

    void CameraUpdate(Camera2D *camera, Vector2 target) {
        float dt = GetFrameTime();
        camera->target.x += (target.x - camera->target.x) * 4.0f * dt;
        camera->target.y += (target.y - camera->target.y) * 4.0f * dt;
    }


    void LoadStar(){
        SetConfigFlags(FLAG_FULLSCREEN_MODE);
        InitWindow(1, 1, "StarProject");
        int monitor = GetCurrentMonitor();
        const int screenWidth = GetMonitorWidth(monitor);
        const int screenHeight = GetMonitorHeight(monitor);
        InitAudioDevice();
        SetTargetFPS(60);
    }
    void LoadMiniStar(){
        const int screenWidth = 800;
        const int screenHeight = 450;

        InitWindow(screenWidth,screenHeight, "StarProject");
        InitAudioDevice();
        SetTargetFPS(60);
    }

    void CircleD(int centerX, int centerY, float radius, Color color) {
        DrawCircle(centerX, centerY, radius, color);
    }

    void RectD(Rectangle rec, Color color) {
        DrawRectangleRec(rec, color);
    }

    void TextureD(Texture2D texture, int posX, int posY, Color color) {
        DrawTexture(texture, posX, posY, color);
    }

    void TextD(const char *text, int posX, int posY, int fontSize, Color color) {
        DrawText(text, posX, posY, fontSize, color);
    }

    Font loadRusFont(const char *fileName, int fontSize) {
        std::vector<int> codepoints;
        for (int i = 32; i <= 126; i++) codepoints.push_back(i);      // Английский, цифры, знаки
        for (int i = 1024; i <= 1279; i++) codepoints.push_back(i);  // Весь русский алфавит (включая Ё/ё)

        return LoadFontEx(fileName, fontSize, codepoints.data(), codepoints.size());
    }
    static std::vector<Texture2D> loadedTextures;
    static std::vector<Font> loadedFonts;
    static std::vector<Sound> loadedSounds;

    // Обёртка для загрузки текстуры с авто-регистрацией
    Texture2D LoadTex(const char *fileName) {
        Texture2D tex = LoadTexture(fileName);
        loadedTextures.push_back(tex);
        return tex;
    }
    void drawRusText(Font font, const char *text, int posX, int posY, float fontSize, Color color) {
        // 1.0f — стандартный интервал между буквами
        DrawTextEx(font, text, {(float)posX, (float)posY}, fontSize, 1.0f, color);
    }
    void CloseStar() {
        for (auto& tex : loadedTextures) UnloadTexture(tex);
        loadedTextures.clear();

        for (auto& font : loadedFonts) UnloadFont(font);
        loadedFonts.clear();

        for (auto& snd : loadedSounds) UnloadSound(snd);
        loadedSounds.clear();

        CloseAudioDevice();
        CloseWindow();
    }

} // namespace eng
