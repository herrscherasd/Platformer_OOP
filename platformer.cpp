#include "raylib.h"

#include "globals.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "graphics.h"
#include "assets.h"
#include "utilities.h"

Player player;

void update_game() {
    game_frame++;

    switch (game_state) {
        case MENU_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                SetExitKey(0);
                game_state = GAME_STATE;
                load_level(0);
            }
            break;

        case GAME_STATE:
            if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
                player.moveHorizontally(PLAYER_MOVEMENT_SPEED);
            }

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
                player.moveHorizontally(-PLAYER_MOVEMENT_SPEED);
            }

            // Calculating collisions to decide whether the player is allowed to jump
            Player::is_on_ground = is_colliding({player.position.x, player.position.y + 0.1f}, WALL);
            if ((IsKeyDown(KEY_UP) || IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE)) && Player::is_on_ground) {
                player.setVelocity(-JUMP_STRENGTH);
            }

            player.update();
            update_enemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = PAUSED_STATE;
            }
            break;

        case PAUSED_STATE:
            if (IsKeyPressed(KEY_ESCAPE)) {
                game_state = GAME_STATE;
            }
            break;

        case DEATH_STATE:
            player.updateGravity();

            if (IsKeyPressed(KEY_ENTER)) {
                if (player.lives > 0) {
                    load_level(0);
                    game_state = GAME_STATE;
                }
                else {
                    game_state = GAME_OVER_STATE;
                    PlaySound(game_over_sound);
                }
            }
            break;

        case GAME_OVER_STATE:
            if (IsKeyPressed(KEY_ENTER)) {
                reset_level_index();
                player.resetStats();
                game_state = GAME_STATE;
                load_level(0);
            }
            break;

        case VICTORY_STATE:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_ESCAPE)) {
                reset_level_index();
                player.resetStats();
                game_state = MENU_STATE;
                SetExitKey(KEY_ESCAPE);
            }
            break;
    }
}

void draw_game() {
    switch(game_state) {
        case MENU_STATE:
            ClearBackground(BLACK);
            draw_menu();
            break;

        case GAME_STATE:
            ClearBackground(BLACK);
            draw_parallax_background();
            draw_level();
            draw_game_overlay();
            break;

        case DEATH_STATE:
            ClearBackground(BLACK);
            draw_death_screen();
            break;

        case GAME_OVER_STATE:
            ClearBackground(BLACK);
            draw_game_over_menu();
            break;

        case PAUSED_STATE:
            ClearBackground(BLACK);
            draw_pause_menu();
            break;

        case VICTORY_STATE:
            draw_victory_menu();
            break;
    }
}

int main() {
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(1024, 480, "Platformer");
    SetTargetFPS(60);
    HideCursor();

    load_fonts();
    load_images();
    load_sounds();
    load_level();

    while (!WindowShouldClose()) {
        BeginDrawing();

        update_game();
        draw_game();

        EndDrawing();
    }

    unload_level();
    unload_sounds();
    unload_images();
    unload_fonts();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
