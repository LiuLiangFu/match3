#include <iostream>
#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    clock_t a, b;
    float total = 0;
    unsigned int total_step = 0;
    a = clock();
    srand(time(nullptr));
    for (unsigned int random_seed = 0; random_seed < 10000; random_seed++) {
        Game test(6, 6, 4, true, 1, false,
                  0, true, 5, 3,
                  random_seed, -1, -1, -2, false, "terminal", true);
        test.start();
        // std::cout << test.to_string();
        float total_reward = 0;
        unsigned int step = 0;
        while (!test.is_terminal()) {
            std::vector<unsigned int> legal_actions = test.legal_actions();
            //for (auto action:legal_actions) {
            //    std::cout << action << " ";
            //}
            //std::cout << std::endl;

            unsigned int action = legal_actions[0];
            float max_reward = 0;
            for (unsigned int candidate_action : legal_actions) {
                float reward = test.simulation_apply_action(candidate_action);
                if (reward > max_reward) {
                    action = candidate_action;
                    max_reward = reward;
                }
                //std::cout << "action: " << action << " Reward: " << reward << std::endl;
            }
            //break;

            //unsigned int action = legal_actions[rand() % legal_actions.size()];
            //std::cout << "action:" << action << std::endl;

            test.apply_action(action);
            total_reward += test.rewards()[0];
            step++;
            //std::cout << "reward:" << test.rewards()[0] << std::endl;
            //std::cout << test.to_string();
        }
        std::cout << total_reward << " " << step << std::endl;
        total += total_reward;
        total_step += step;
    }
    b = clock();
    std::cout << "total:" << total << std::endl;
    std::cout << "step:" << total_step << std::endl;
    std::cout << "time " << a << " " << b << " " << CLOCKS_PER_SEC << std::endl;
    std::cout << "Use time:" << (double) (b - a) / CLOCKS_PER_SEC << std::endl;
    return 0;
}