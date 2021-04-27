//
// Created by LiuLiangFu on 2021/4/22.
//

#ifndef CXX_MATCH3_SEARCHER_H
#define CXX_MATCH3_SEARCHER_H

#include "Board.h"

class Searcher {
public:
    explicit Searcher(unsigned short length);

    std::vector<unsigned int> get_matches_action(Board &board) const;

    bool have_any_matches_action(Board &board) const;

    std::vector<unsigned int> get_legal_action(Board &board) const;

    [[nodiscard]] std::vector<unsigned short> scan_board_for_matches(const Board &board) const;

    [[nodiscard]] bool have_any_match(const Board &board) const;

private:
    unsigned short length_;
};

#endif //CXX_MATCH3_SEARCHER_H
