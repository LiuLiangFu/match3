//
// Created by LiuLiangFu on 2021/4/20.
//

#include "Filler.h"

#include <chrono>
#include <stack>
#include <algorithm>
#include <iostream>

Filler::Filler(bool immovable_move, unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
               unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
               unsigned short number_of_step_add_immovable) :
        immovable_move_(immovable_move),
        n_of_match_counts_immovable_(n_of_match_counts_immovable),
        match_counts_add_immovable_(match_counts_add_immovable),
        number_of_match_counts_add_immovable_(number_of_match_counts_add_immovable),
        step_add_immovable_(step_add_immovable),
        number_of_step_add_immovable_(number_of_step_add_immovable),
        match_count_(0),
        step_(0),
        num_of_immovable_(0) {
    random_engine = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
}

Filler::Filler(bool immovable_move, unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
               unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
               unsigned short number_of_step_add_immovable, unsigned int random_seed) :
        immovable_move_(immovable_move),
        n_of_match_counts_immovable_(n_of_match_counts_immovable),
        match_counts_add_immovable_(match_counts_add_immovable),
        number_of_match_counts_add_immovable_(number_of_match_counts_add_immovable),
        step_add_immovable_(step_add_immovable),
        number_of_step_add_immovable_(number_of_step_add_immovable),
        match_count_(0),
        step_(0),
        num_of_immovable_(0) {
    random_engine = std::mt19937((unsigned) random_seed);
}

void Filler::step() {
    step_ += 1;
}

void Filler::reset() {
    num_of_immovable_ = 0;
    match_count_ = 0;
    step_ = 0;
}

void Filler::move_and_fill_spaces(Board &board, bool fill, bool init) {
    move_spaces(board);
    if (fill) {
        fill_spaces(board, init);
    }
}

void Filler::move_spaces(Board &board) {
    const std::vector<short> obs = board.observation();
    const std::vector<unsigned short> shape = board.shape();
    for (unsigned short x = 0; x < shape[1]; x++) {
        std::vector<short> new_line(shape[0], board.space_shape());
        std::vector<unsigned short> indexes(shape[0], 0);
        std::stack<short> can_move;
        for (unsigned short y = 0; y < shape[0]; y++) {
            unsigned short index = y * shape[1] + x;
            indexes[y] = index;

            if (obs[index] == board.immovable_shape()) {
                if (immovable_move_) {
                    can_move.push(obs[index]);
                } else {
                    new_line[y] = board.immovable_shape();
                }
            } else if (obs[index] != board.space_shape()) {
                can_move.push(obs[index]);
            }
        }
        for (unsigned short y = shape[0] - 1; y >= 0; y--) {
            if (can_move.empty())
                break;
            if (new_line[y] == board.space_shape()) {
                new_line[y] = can_move.top();
                can_move.pop();
            }
        }
        board.put(indexes, new_line);
    }
}

void Filler::fill_spaces(Board &board, bool init) {
    const std::vector<unsigned short> spaces = get_indexes(board == board.space_shape());
    const unsigned short spaces_count = spaces.size();
    std::vector<short> new_shapes(spaces_count, board.space_shape());
    if (init) {
        std::uniform_int_distribution<int> distribution(0, board.n_shapes() - 1);
        for (unsigned short i = 0; i < spaces_count; i++) {
            new_shapes[i] = distribution(random_engine);
        }
    } else {
        if (match_counts_add_immovable_) {
            match_count_ += spaces_count;
            num_of_immovable_ += n_of_match_counts_immovable_ * (match_count_ / number_of_match_counts_add_immovable_);
            match_count_ %= number_of_match_counts_add_immovable_;
        }

        if (step_add_immovable_) {
            num_of_immovable_ += n_of_match_counts_immovable_ * (step_ / number_of_step_add_immovable_);
            step_ %= number_of_step_add_immovable_;
        }
        // std::cout << "Filler:" << step_ << " " << number_of_step_add_immovable_ << std::endl;

        if (num_of_immovable_ >= spaces_count) {
            num_of_immovable_ -= spaces_count;
            for (unsigned short i = 0; i < spaces_count; i++) {
                new_shapes[i] = board.immovable_shape();
            }
        } else {
            std::uniform_int_distribution<int> distribution(0, board.n_shapes() - 1);
            for (unsigned short i = 0; i < num_of_immovable_; i++) {
                new_shapes[i] = board.immovable_shape();
            }
            for (unsigned short i = num_of_immovable_; i < spaces_count; i++) {
                new_shapes[i] = distribution(random_engine);
            }
            num_of_immovable_ = 0;
            std::shuffle(new_shapes.begin(), new_shapes.end(), random_engine);
        }
    }
    board.put(spaces, new_shapes);
}
