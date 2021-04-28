#include <iostream>
#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <algorithm>
#include <cmath>

template<typename string_name, typename vector_vec>
void analysis_vector(const string_name &name, const vector_vec &vec) {
    auto minmax = std::minmax_element(vec.begin(), vec.end());
    double sum = std::accumulate(vec.begin(), vec.end(), 0.0);
    double mean = sum / vec.size();
    double variance = 0.0;
    for (auto element:vec) {
        variance += std::pow((double) element - mean, 2);
    }
    variance /= vec.size();

    std::cout << name
              << ":\t[Max: " << *minmax.second
              << "\tMean: " << mean
              << "\tStd: " << std::sqrt(variance)
              << "\tMin: " << *minmax.first << "]" << std::endl;
}

int main() {
    clock_t a, b;
    const unsigned int number_of_game = 10000;
    std::string mode = "random";

    std::vector<double> total_rewards, total_times;
    total_rewards.reserve(number_of_game);
    total_times.reserve(number_of_game);

    std::vector<unsigned int> total_steps;
    total_steps.reserve(number_of_game);

    std::vector<unsigned int> total_state_legal_actions;
    std::vector<double> total_game_legal_actions;
    total_game_legal_actions.reserve(number_of_game);

    srand(time(nullptr));

    for (unsigned int random_seed = 0; random_seed < number_of_game; random_seed++) {
        a = clock();
        Game test(6, 6, 4, true, 1, false,
                  0, true, 5, 3,
                  random_seed, -1, -1, -2, false,
                  "terminal", true, false);
        test.start();
        // std::cout << test.to_string();
        double rewards = 0;
        unsigned int steps = 0;
        unsigned int game_legal_actions = 0.0;
        while (!test.is_terminal()) {
            std::vector<unsigned int> legal_actions = test.legal_actions();
            //for (auto action:legal_actions) {
            //    std::cout << action << " ";
            //}
            //std::cout << std::endl;

            unsigned int action = legal_actions[0];
            if (mode == "random") {
                action = legal_actions[rand() % legal_actions.size()];
            } else if (mode == "greedy_a") {
                float max_reward = 0;
                for (unsigned int candidate_action : legal_actions) {
                    float reward = test.simulation_apply_action(candidate_action);
                    if (reward > max_reward) {
                        action = candidate_action;
                        max_reward = reward;
                    }
                    //std::cout << "action: " << action << " Reward: " << reward << std::endl;
                }
            }

            //std::cout << "action:" << action << std::endl;
            test.apply_action(action);

            rewards += test.rewards()[0];
            steps++;
            game_legal_actions += legal_actions.size();
            total_state_legal_actions.push_back(legal_actions.size());
            //std::cout << "reward:" << test.rewards()[0] << std::endl;
            //std::cout << test.to_string();
        }
        b = clock();
        std::cout << "rewards: " << rewards << "\tsteps: " << steps << "\ttime: " << (double) (b - a) / CLOCKS_PER_SEC
                  << std::endl;

        total_rewards.push_back(rewards);
        total_steps.push_back(steps);
        total_times.push_back((double) (b - a) / CLOCKS_PER_SEC);
        total_game_legal_actions.push_back((double) game_legal_actions / steps);
    }

    analysis_vector(std::string("Rewards"), total_rewards);
    analysis_vector(std::string("Steps  "), total_steps);
    std::cout << "Legal Actions:" << std::endl;
    analysis_vector(std::string("Games  "), total_game_legal_actions);
    analysis_vector(std::string("States "), total_state_legal_actions);
    analysis_vector(std::string("Times  "), total_times);
    return 0;
}