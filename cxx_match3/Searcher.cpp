//
// Created by LiuLiangFu on 2021/4/22.
//

#include "Searcher.h"

Searcher::Searcher(unsigned short length) :
        length_(length) {

}

std::vector<unsigned short> Searcher::scan_board_for_matches(const Board &board) const {
    std::vector<bool> ret(board.size(), false);
    std::vector<unsigned short> shape = board.shape();
    const std::vector<short> &obs = board.observation();
    short space_shape = board.space_shape();
    short immovable_shape = board.immovable_shape();
    for (unsigned short y = 0; y < shape[0]; y++) {
        std::vector<unsigned short> match(shape[1], 1);
        for (unsigned short x = 1; x < shape[1]; x++) {
            unsigned short index = y * shape[1] + x;
            if (obs[index] != space_shape && obs[index] != immovable_shape) {
                if (obs[index] == obs[index - 1]) {
                    match[x] = match[x - 1] + 1;
                    if (match[x] == length_) {
                        for (unsigned short dx = 0; dx < length_; dx++) {
                            ret[index - dx] = true;
                        }
                    } else if (match[x] > length_) {
                        ret[index] = true;
                    }
                }
            }
        }
    }
    for (unsigned short x = 0; x < shape[1]; x++) {
        std::vector<unsigned short> match(shape[0], 1);
        for (unsigned short y = 1; y < shape[0]; y++) {
            unsigned short index = y * shape[1] + x;
            if (obs[index] != space_shape && obs[index] != immovable_shape) {
                if (obs[index] == obs[index - shape[1]]) {
                    match[y] = match[y - 1] + 1;
                    if (match[y] == length_) {
                        for (unsigned short dy = 0; dy < length_; dy++) {
                            ret[index - dy * shape[1]] = true;
                        }
                    } else if (match[y] > length_) {
                        ret[index] = true;
                    }
                }
            }
        }
    }
    return get_indexes(ret);
}

bool Searcher::have_any_match(const Board &board) const {
    std::vector<unsigned short> shape = board.shape();
    const std::vector<short> &obs = board.observation();
    short space_shape = board.space_shape();
    short immovable_shape = board.immovable_shape();
    for (unsigned short y = 0; y < shape[0]; y++) {
        std::vector<unsigned short> match(shape[1], 1);
        for (unsigned short x = 1; x < shape[1]; x++) {
            unsigned short index = y * shape[1] + x;
            if (obs[index] != space_shape && obs[index] != immovable_shape) {
                if (obs[index] == obs[index - 1]) {
                    match[x] = match[x - 1] + 1;
                    if (match[x] >= length_) {
                        return true;
                    }
                }
            }
        }
    }
    for (unsigned short x = 0; x < shape[1]; x++) {
        std::vector<unsigned short> match(shape[0], 1);
        for (unsigned short y = 1; y < shape[0]; y++) {
            unsigned short index = y * shape[1] + x;
            if (obs[index] != space_shape && obs[index] != immovable_shape) {
                if (obs[index] == obs[index - shape[1]]) {
                    match[y] = match[y - 1] + 1;
                    if (match[y] >= length_) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

std::vector<unsigned int> Searcher::get_matches_action(Board &board) const {
    std::vector<unsigned int> ret;
    for (unsigned int action = 0; action < board.action_space(); action++) {
        if (board.apply_action(action)) {
            if (have_any_match(board)) {
                ret.push_back(action);
            }
            board.apply_action(action);
        }
    }
    return ret;
}

bool Searcher::have_any_matches_action(Board &board) const {
    for (unsigned int action = 0; action < board.action_space(); action++) {
        if (board.apply_action(action)) {
            if (have_any_match(board)) {
                board.apply_action(action);
                return true;
            }
            board.apply_action(action);
        }
    }
    return false;
}

std::vector<unsigned int> Searcher::get_legal_action(Board &board) const {
    std::vector<unsigned int> ret;
    for (unsigned int action = 0; action < board.action_space(); action++) {
        if (board.apply_action(action)) {
            ret.push_back(action);
            board.apply_action(action);
        }
    }
    return ret;
}
