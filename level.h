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

    size_t rows() const;
    size_t columns() const;
    const std::vector<char>& data() const { return m_data; }

    bool is_inside(int row, int column) const;

    char get_cell(size_t row, size_t column) const;
    char& get_cell(size_t row, size_t column);
    void set_cell(size_t row, size_t column, char chr);

    void load_from_data(size_t rows, size_t columns, const char* data);

};


bool is_inside_level(int row, int column);
bool is_colliding(Vector2 pos, char look_for = '#');
char& get_collider(Vector2 pos, char look_for);
void reset_level_index();
void load_level(int offset = 0);
void unload_level();

extern Level* current_level;
extern int level_index;

#endif //LEVEL_H
