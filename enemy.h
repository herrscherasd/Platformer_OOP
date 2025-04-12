#ifndef ENEMY_H
#define ENEMY_H

#include "globals.h"

class Enemy {
private:
    Vector2 pos;
    bool is_looking_right;

public:
    Enemy(Vector2 position, bool lookingRight = true);

    void update();
    void draw(float cellSize, Texture2D& sprite);

    bool checkCollision(Vector2 otherPos) const;
    Vector2 getPosition() const;
};

extern std::vector<Enemy> enemies;

void spawn_enemies();
void update_enemies();
void draw_enemies(float cellSize, Texture2D& sprite);
bool is_colliding_with_enemies(Vector2 pos);
void remove_colliding_enemy(Vector2 pos);

#endif // ENEMY_H
