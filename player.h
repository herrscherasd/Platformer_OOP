#ifndef PLAYER_H
#define PLAYER_H

#include "globals.h"

class Player {
public:
    static float y_velocity;
    static Vector2 position;

    static bool is_on_ground;
    static bool is_looking_forward;
    static bool is_moving;

    static int level_scores[LEVEL_COUNT];
    static const int MAX_PLAYER_LIVES = 3;
    static int lives;

    Player();

    void resetStats();
    void incrementScore();
    int getTotalScore() const;
    void spawn();
    void kill();
    void moveHorizontally(float delta);
    void updateGravity();
    bool isCollidingWithEnemies(Vector2 pos);
    void update();

    Vector2 getPosition() const;
    void setPosition(Vector2 newPos);
    float getVelocity() const;
    void setVelocity(float v);
    int getLives() const;
    bool isOnGround() const;
};

extern Player player;

#endif // PLAYER_H
