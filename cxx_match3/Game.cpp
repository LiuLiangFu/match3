//
// Created by LiuLiangFu on 2021/4/20.
//

#include "Game.h"
#include <algorithm>
#include <utility>

Game::Game(unsigned short x, unsigned short y, unsigned short n_shapes, bool immovable_move,
           unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
           unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
           unsigned short number_of_step_add_immovable, unsigned short length, unsigned int random_seed,
           int rollout_len, short immovable_shape, short space_shape, bool immovable_interactive,
           std::string no_match_actions_do, bool only_match_action_legal, bool apply_non_match_action) :
        board_(Board(x, y, n_shapes, immovable_shape, space_shape, immovable_interactive)),
        filler_(Filler(immovable_move, n_of_match_counts_immovable, match_counts_add_immovable,
                       number_of_match_counts_add_immovable, step_add_immovable,
                       number_of_step_add_immovable, random_seed)),
        searcher_(Searcher(length)),
        step_(0),
        rollout_len_(rollout_len),
        no_match_actions_do_(std::move(no_match_actions_do)),
        only_match_action_legal_(only_match_action_legal),
        apply_non_match_action_(apply_non_match_action),
        reward_(0),
        total_reward_(0) {
}

Game::Game(unsigned short x, unsigned short y, unsigned short n_shapes, bool immovable_move,
           unsigned short n_of_match_counts_immovable, bool match_counts_add_immovable,
           unsigned short number_of_match_counts_add_immovable, bool step_add_immovable,
           unsigned short number_of_step_add_immovable, unsigned short length,
           int rollout_len, short immovable_shape, short space_shape, bool immovable_interactive,
           std::string no_match_actions_do, bool only_match_action_legal, bool apply_non_match_action) :
        board_(Board(x, y, n_shapes, immovable_shape, space_shape, immovable_interactive)),
        filler_(Filler(immovable_move, n_of_match_counts_immovable, match_counts_add_immovable,
                       number_of_match_counts_add_immovable, step_add_immovable,
                       number_of_step_add_immovable)),
        searcher_(Searcher(length)),
        step_(0),
        rollout_len_(rollout_len),
        no_match_actions_do_(std::move(no_match_actions_do)),
        only_match_action_legal_(only_match_action_legal),
        apply_non_match_action_(apply_non_match_action),
        reward_(0),
        total_reward_(0) {
}

void Game::start() {
    board_.clear();
    filler_.move_and_fill_spaces(board_, true, true);
    bool have_any_match = searcher_.have_any_match(board_);
    bool have_any_matches_action = searcher_.have_any_matches_action(board_);
    while (have_any_match || !have_any_matches_action) {
        if (have_any_match) {
            board_.delete_match(searcher_.scan_board_for_matches(board_));
        } else if (!have_any_matches_action) {
            board_.clear();
        }
        filler_.move_and_fill_spaces(board_, true, true);
        have_any_match = searcher_.have_any_match(board_);
        have_any_matches_action = searcher_.have_any_matches_action(board_);
    }
    filler_.reset();
}

std::string Game::to_string() const {
    return board_.to_string();
}

bool Game::apply_action(unsigned int action) {
    std::vector<unsigned int> legal_action = legal_actions();
    auto it = std::find(legal_action.begin(), legal_action.end(), action);
    if (it != legal_action.end() && board_.apply_action(action)) {
        step_++;
        filler_.step();
        reward_ = 0;

        std::vector<unsigned short> matches = searcher_.scan_board_for_matches(board_);
        if (matches.empty() && !apply_non_match_action_)
            board_.apply_action(action);
        else {
            while (!matches.empty()) {
                reward_ += matches.size();
                board_.delete_match(matches);
                filler_.move_and_fill_spaces(board_, true, false);
                matches = searcher_.scan_board_for_matches(board_);
            }
        }
        total_reward_ += reward_;

        if (no_match_actions_do_ != "terminal" && !searcher_.have_any_matches_action(board_)) {
            // TODO
        }
        return true;
    }
    return false;
}

std::vector<unsigned int> Game::legal_actions() {
    if (only_match_action_legal_)
        return searcher_.get_matches_action(board_);
    return searcher_.get_legal_action(board_);
}

bool Game::is_terminal() {
    if (rollout_len_ > 0 && step_ >= rollout_len_)
        return true;
    return no_match_actions_do_ == "terminal" && !searcher_.have_any_matches_action(board_);
}

int Game::current_player() const {
    return 0;
}

std::vector<float> Game::rewards() const {
    return {reward_};
}

std::vector<short> Game::observation_tensor() const {
    return board_.observation();
}

void Game::reset() {
    start();
    step_ = 0;
    reward_ = 0;
    total_reward_ = 0;
}

float Game::simulation_apply_action(unsigned int action) const {
    Board copy_board = board_;
    Filler copy_filler = filler_;
    float reward = 0;
    copy_board.apply_action(action);
    std::vector<unsigned short> matches = searcher_.scan_board_for_matches(copy_board);
    while (!matches.empty()) {
        reward += matches.size();
        copy_board.delete_match(matches);
        copy_filler.move_and_fill_spaces(copy_board, false, false);
        matches = searcher_.scan_board_for_matches(copy_board);
    }
    return reward;
}
