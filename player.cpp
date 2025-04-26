#include "player.h"

#include "level.h"


float Player::y_velocity = 0;
Vector2 Player::position = {0, 0};

bool Player::is_on_ground = false;
bool Player::is_looking_forward = true;
bool Player::is_moving = false;

int Player::level_scores[LEVEL_COUNT] = {};
int Player::lives = Player::MAX_PLAYER_LIVES;

Player::Player() {
    y_velocity = 0;
    position = {0, 0};
    is_on_ground = false;
    is_looking_forward = true;
    is_moving = false;
    lives = MAX_PLAYER_LIVES;
    std::fill(level_scores, level_scores + LEVEL_COUNT, 0);
}


void Player::resetStats() {
    lives = MAX_PLAYER_LIVES;
    std::fill(level_scores, level_scores + LEVEL_COUNT, 0);
}

void Player::incrementScore() {
    PlaySound(coin_sound);
    level_scores[level_index]++;
}

int Player::getTotalScore() const {
    int sum = 0;
    for (int i = 0; i < LEVEL_COUNT; ++i) {
        sum += level_scores[i];
    }
    return sum;
}

void Player::spawn() {
    size_t level_rows = current_level->rows();
    size_t level_columns = current_level->columns();
    y_velocity = 0;
    for (size_t row = 0; row < level_rows; ++row) {
        for (size_t col = 0; col < level_columns; ++col) {
            char cell = current_level->get_cell(row, col);
            if (cell == PLAYER) {
                position.x = col;
                position.y = row;
                current_level->set_cell(row, col, AIR);
                return;
            }
        }
    }
}

void Player::kill() {
    PlaySound(player_death_sound);
    game_state = DEATH_STATE;
    lives--;
    level_scores[level_index] = 0;
}

void Player::moveHorizontally(float delta) {
    float next_x = position.x + delta;
    if (!is_colliding({next_x, position.y}, WALL)) {
        position.x = next_x;
    } else {
        position.x = roundf(position.x);
    }

    is_looking_forward = delta > 0;
    is_moving = delta != 0;
}

void Player::updateGravity() {
    if (is_colliding({position.x, position.y - 0.1f}, WALL) && y_velocity < 0) {
        y_velocity = CEILING_BOUNCE_OFF;
    }

    position.y += y_velocity;
    y_velocity += GRAVITY_FORCE;

    is_on_ground = is_colliding({position.x, position.y + 0.1f}, WALL);
    if (is_on_ground) {
        y_velocity = 0;
        position.y = roundf(position.y);
    }
}

bool Player::isCollidingWithEnemies(Vector2 pos) {
    return is_colliding_with_enemies(pos);
}

void Player::update() {
    updateGravity();
    size_t level_rows = current_level->rows();

    if (is_colliding(position, COIN)) {
        get_collider(position, COIN) = AIR;
        incrementScore();
    }

    if (is_colliding(position, EXIT)) {
        if (timer > 0) {
            timer -= 25;
            time_to_coin_counter += 5;

            if (time_to_coin_counter / 60 > 1) {
                incrementScore();
                time_to_coin_counter = 0;
            }
        } else {
            load_level(1);
            PlaySound(exit_sound);
        }
    } else {
        if (timer >= 0) timer--;
    }

    if (is_colliding(position, SPIKE) || position.y > level_rows) {
        kill();
    }

    if (is_colliding_with_enemies(position)) {
        if (y_velocity > 0) {
            remove_colliding_enemy(position);
            PlaySound(kill_enemy_sound);
            incrementScore();
            y_velocity = -BOUNCE_OFF_ENEMY;
        } else {
            kill();
        }
    }
}


Vector2 Player::getPosition() const { return position; }
void Player::setPosition(Vector2 newPos) { position = newPos; }

float Player::getVelocity() const { return y_velocity; }
void Player::setVelocity(float v) { y_velocity = v; }

int Player::getLives() const { return lives; }
bool Player::isOnGround() const { return is_on_ground; }
