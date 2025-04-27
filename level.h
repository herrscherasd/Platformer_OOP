#ifndef LEVEL_H
#define LEVEL_H

#include "globals.h"
#include "player.h"

class Level {
private:
    size_t m_rows = 0;
    size_t m_columns = 0;
    std::vector<char> m_data;

public:
    Level() = default;
    Level(size_t rows, size_t columns, const std::vector<char>& data);

    [[nodiscard]] size_t rows() const;
    [[nodiscard]] size_t columns() const;
    [[nodiscard]] const std::vector<char>& data() const { return m_data; }

    [[nodiscard]] bool is_inside(int row, int column) const;

    [[nodiscard]] char get_cell(size_t row, size_t column) const;
    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char chr);
};

std::vector<char> decode_rll_line(const std::string& line);
bool load_levels_from_file(const std::string& filename);
void load_levels();
void init_levels();
void cleanup_levels();

bool is_inside_level(int row, int column);
bool is_colliding(Vector2 pos, char look_for = '#');
char& get_collider(Vector2 pos, char look_for);
void reset_level_index();
void load_level(int offset = 0);
void unload_level();

extern Level* current_level;
extern int level_index;

#endif //LEVEL_H
