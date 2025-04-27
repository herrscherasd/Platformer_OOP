#include "level.h"
#include "globals.h"
#include <algorithm>
#include <fstream>

static Level* LEVELS[LEVEL_COUNT] = {nullptr};

Level* current_level = nullptr;

int level_index = 0;

std::vector<char> decode_rll_line(const std::string& line) {
    std::vector<char> result;
    size_t i = 0;

    while (i < line.size()) {
        if (isdigit(line[i])) {
            size_t count = 0;
            while (i < line.size() && isdigit(line[i])) {
                count = count * 10 + (line[i] - '0');
                i++;
            }

            if (i < line.size()) {
                char c = line[i];
                for (size_t j = 0; j < count; j++) {
                    result.push_back(c);
                }
                i++;
            }
        } else {
            result.push_back(line[i]);
            i++;
        }
    }

    return result;
}

bool load_levels_from_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    for (int i = 0; i < LEVEL_COUNT; i++) {
        delete LEVELS[i];
        LEVELS[i] = nullptr;
    }

    std::string line;
    int level_idx = 0;

    while (std::getline(file, line) && level_idx < LEVEL_COUNT) {
        if (line.empty() || line[0] == ';') {
            continue;
        }

        std::vector<std::string> rows;
        std::string current_row;
        size_t i = 0;

        while (i < line.size()) {
            if (line[i] == '|') {
                rows.push_back(current_row);
                current_row.clear();
            } else {
                current_row += line[i];
            }
            i++;
        }

        if (!current_row.empty()) {
            rows.push_back(current_row);
        }

        std::vector<std::vector<char>> decoded_rows;
        for (const auto& row : rows) {
            decoded_rows.push_back(decode_rll_line(row));
        }

        size_t columns = decoded_rows[0].size();
        for (const auto& row : decoded_rows) {
            if (row.size() != columns) {
                return false;
            }
        }

        std::vector<char> level_data;
        for (const auto& row : decoded_rows) {
            level_data.insert(level_data.end(), row.begin(), row.end());
        }

        LEVELS[level_idx] = new Level(decoded_rows.size(), columns, level_data);
        level_idx++;
    }

    return level_idx > 0;
}

void load_levels() {
    bool loaded = load_levels_from_file("data/levels.rll");
}

void init_levels() {
    load_levels();
    load_level();
}

void cleanup_levels() {
    for (int i = 0; i < LEVEL_COUNT; i++) {
        delete LEVELS[i];
        LEVELS[i] = nullptr;
    }
}

Level::Level(size_t rows, size_t cols, const std::vector<char>& data)
    : m_rows(rows), m_columns(cols), m_data(data) {}

char Level::get_cell(size_t row, size_t col) const {
        return m_data[row * m_columns + col];
}

char& Level::get_cell(size_t row, size_t col) {
        return m_data[row * m_columns + col];
}

void Level::set_cell(size_t row, size_t col, char value) {
        m_data[row * m_columns + col] = value;
}

bool Level::is_inside(int row, int col) const {
        return row >= 0 && row < m_rows && col >= 0 && col < m_columns;
}


bool is_inside_level(int row, int column) {
        return current_level && current_level->is_inside(row, column);
}

bool is_colliding(Vector2 pos, char look_for) {
        Rectangle entity_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        // Scan the adjacent area in the level to look for a match in collision
        for (int row = pos.y - 1; row < pos.y + 1; ++row) {
                for (int column = pos.x - 1; column < pos.x + 1; ++column) {
                        // Check if the cell is out-of-bounds
                        if (!is_inside_level(row, column)) continue;
                        if (current_level->get_cell(row, column) == look_for) {
                                Rectangle block_hitbox = {(float) column, (float) row, 1.0f, 1.0f};
                                if (CheckCollisionRecs(entity_hitbox, block_hitbox)) {
                                        return true;
                                }
                        }
                }
        }
        return false;
}

char& get_collider(Vector2 pos, char look_for) {
        Rectangle player_hitbox = {pos.x, pos.y, 1.0f, 1.0f};

        for (int row = pos.y - 1; row < pos.y + 1; ++row) {
                for (int column = pos.x - 1; column < pos.x + 1; ++column) {
                        if (is_inside_level(row, column) &&
                            current_level->get_cell(row, column) == look_for) {
                                return const_cast<char&>(current_level->get_cell(row, column));
                            }
                }
        }
        return const_cast<char&>(current_level->get_cell(pos.y, pos.x));
}

void load_level(int offset) {
        if (current_level) {
                delete current_level;
                current_level = nullptr;
        }

        level_index += offset;

        if (level_index >= LEVEL_COUNT) {
                game_state = VICTORY_STATE;
                create_victory_menu_background();
                level_index = 0;
                return;
        }

        current_level = new Level(*LEVELS[level_index]);

        player.spawn();
        spawn_enemies();
        derive_graphics_metrics_from_loaded_level();
        timer = MAX_LEVEL_TIME;
}

void unload_level() {
        if (current_level) {
                delete current_level;
                current_level = nullptr;
        }
    cleanup_levels();
}

void reset_level_index() {
    level_index = 0;
}

size_t Level::rows() const { return m_rows; }
size_t Level::columns() const { return m_columns; }