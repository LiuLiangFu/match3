//
// Created by LiuLiangFu on 2021/4/20.
//

#ifndef CXX_MATCH3_GAME_H
#define CXX_MATCH3_GAME_H

#include "Board.h"
#include "Filler.h"
#include "Searcher.h"

class Game {
public:
    Game(unsigned short x, unsigned short y, unsigned short n_shapes, bool immovable_move,
         unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
         unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
         unsigned short number_of_step_add_immovable, unsigned short length, unsigned int random_seed,
         int rollout_len = -1, short immovable_shape = -1, short space_shape = -2, bool immovable_interactive = false,
         std::string no_match_actions_do = "terminal", bool only_match_action_legal = true);

    Game(unsigned short x, unsigned short y, unsigned short n_shapes, bool immovable_move,
         unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
         unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
         unsigned short number_of_step_add_immovable, unsigned short length,
         int rollout_len = -1, short immovable_shape = -1, short space_shape = -2, bool immovable_interactive = false,
         std::string no_match_actions_do = "terminal", bool only_match_action_legal = true);

    void start();

    void reset();

    bool apply_action(unsigned int action);

    [[nodiscard]] float simulation_apply_action(unsigned int action) const;

    std::vector<unsigned int> legal_actions();

    bool is_terminal();

    [[nodiscard]] int current_player() const;

    [[nodiscard]] std::vector<float> rewards() const;

    [[nodiscard]] std::vector<short> observation_tensor() const;

    [[nodiscard]] std::string to_string() const;

private:
    Board board_;
    Filler filler_;
    Searcher searcher_;

    int step_;
    int rollout_len_;
    std::string no_match_actions_do_;
    bool only_match_action_legal_;

    float reward_;
    float total_reward_;

    float simulation_apply_action() const;
};

#endif //CXX_MATCH3_GAME_H
