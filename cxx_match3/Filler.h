//
// Created by LiuLiangFu on 2021/4/20.
//

#ifndef CXX_MATCH3_FILLER_H
#define CXX_MATCH3_FILLER_H

#include <random>

#include "Board.h"

class Filler {
public:
    Filler(bool immovable_move, unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
           unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
           unsigned short number_of_step_add_immovable);

    Filler(bool immovable_move, unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
           unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
           unsigned short number_of_step_add_immovable, unsigned int random_seed);

    void step();

    void reset();

    void move_and_fill_spaces(Board &board, bool fill = true, bool init = false);

private:
    void move_spaces(Board &board);

    void fill_spaces(Board &board, bool init = false);

private:
    std::mt19937 random_engine;

    bool immovable_move_;
    unsigned short num_of_immovable_;
    unsigned short n_of_match_counts_immovable_;

    bool match_counts_add_immovable_;
    unsigned short number_of_match_counts_add_immovable_;
    unsigned short match_count_;

    bool step_add_immovable_;
    unsigned short number_of_step_add_immovable_;
    unsigned short step_;
};

#endif //CXX_MATCH3_FILLER_H
