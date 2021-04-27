//
// Created by LiuLiangFu on 2021/4/20.
//

#ifndef CXX_MATCH3_BOARD_H
#define CXX_MATCH3_BOARD_H

#include <vector>
#include <string>

std::vector<unsigned short> get_indexes(const std::vector<bool> &vec);

class Board {
public:
    Board(unsigned short x, unsigned short y, unsigned short n_shapes, short immovable_shape = -1,
          short space_shape = -2, bool immovable_interactive = false);

    Board(const Board& board) = default;

    bool apply_action(unsigned int action);

    bool put(const std::vector<unsigned short> &indexes, const std::vector<short> &shapes);

    void delete_match(const std::vector<unsigned short> &indexes);

    void clear();

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::vector<unsigned short> shape() const;

    [[nodiscard]] unsigned short size() const;

    [[nodiscard]] std::vector<short> observation() const;

    [[nodiscard]] short immovable_shape() const;

    [[nodiscard]] short space_shape() const;

    [[nodiscard]] unsigned short n_shapes() const;

    [[nodiscard]] unsigned int action_space() const;

    std::vector<bool> operator==(short shape);

    std::vector<bool> operator!=(short shape);

private:
    unsigned short x_;
    unsigned short y_;
    unsigned short size_;
    unsigned short n_shapes_;
    short immovable_shape_;
    short space_shape_;
    bool immovable_interactive_;
    std::vector<short> data_;
};

#endif //CXX_MATCH3_BOARD_H
