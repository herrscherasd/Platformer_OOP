#include "enemy.h"

std::vector<Enemy> enemies;

Enemy::Enemy(Vector2 position, bool lookingRight)
    : pos(position), is_looking_right(lookingRight) {}

void Enemy::update() {
    float next_x = pos.x + (is_looking_right ? ENEMY_MOVEMENT_SPEED : -ENEMY_MOVEMENT_SPEED);
    if (is_colliding({next_x, pos.y}, WALL)) {
        is_looking_right = !is_looking_right;
    } else {
        pos.x = next_x;
    }
}

void Enemy::draw(float cellSize, Texture2D& sprite) {
    Rectangle source = {
        0,
        0,
        (float)sprite.width * (is_looking_right ? 1 : -1),
        (float)sprite.height
    };

    Vector2 screenPos = {
        pos.x * cellSize + horizontal_shift,
        pos.y * cellSize
    };

    DrawTexturePro(sprite, source, {
        screenPos.x, screenPos.y,
        cellSize, cellSize
    }, { 0, 0 }, 0.0f, WHITE);
}

bool Enemy::checkCollision(Vector2 otherPos) const {
    Rectangle enemyHitbox = { pos.x, pos.y, 1.0f, 1.0f };
    Rectangle otherHitbox = { otherPos.x, otherPos.y, 1.0f, 1.0f };
    return CheckCollisionRecs(enemyHitbox, otherHitbox);
}

Vector2 Enemy::getPosition() const {
    return pos;
}


void spawn_enemies() {
    enemies.clear();

    for (size_t row = 0; row < current_level.rows; ++row) {
        for (size_t column = 0; column < current_level.columns; ++column) {
            char cell = get_level_cell(row, column);
            if (cell == ENEMY) {
                enemies.emplace_back(Vector2{(float)column, (float)row});
                set_level_cell(row, column, AIR);
            }
        }
    }
}

void update_enemies() {
    for (auto& enemy : enemies) {
        enemy.update();
    }
}

void draw_enemies(float cellSize, Texture2D& sprite) {
    for (auto& enemy : enemies) {
        enemy.draw(cellSize, sprite);
    }
}

bool is_colliding_with_enemies(Vector2 pos) {
    for (const auto& enemy : enemies) {
        if (enemy.checkCollision(pos)) {
            return true;
        }
    }
    return false;
}

void remove_colliding_enemy(Vector2 pos) {
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->checkCollision(pos)) {
            enemies.erase(it);
            remove_colliding_enemy(pos);
            return;
        }
    }
}
