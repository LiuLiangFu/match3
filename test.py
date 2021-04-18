# -*- coding: utf-8 -*-
import concurrent.futures
from random import choice
import numpy as np
import time
from gym_match3.envs import Match3Env
from gym_match3.envs.levels import LEVELS, Match3Levels, Level
import argparse
import yaml
from pathlib import Path
import multiprocessing
import copy

def Getlevels(WidthAndHeight, shapes):
    new_level = [Level(WidthAndHeight, WidthAndHeight, shapes, np.zeros((WidthAndHeight, WidthAndHeight)).tolist())]
    return Match3Levels(new_level)

def play_game(config, verbose=False):
    levels = Getlevels(config["board_width_and_hight"], config["board_number_of_different_color"])
    env = Match3Env(immovable_move=config["immovable_move"],
                    n_of_match_counts_immov=config["number_of_immovable_add"],
                    match_counts_add_immovable=config["match_counts_add_immovable"],
                    number_of_match_counts_add_immovable=config["number_of_match_counts_add_immovable"],
                    step_add_immovable=config["step_add_immovable"],
                    number_of_step_add_immovable=config["number_of_step_add_immovable"],
                    no_legal_actions_do=config["no_legal_actions_do"],
                    rollout_len=config["rollout_len"],
                    levels = levels)
    
    available_actions = {v : k for k, v in dict(enumerate(env.get_available_actions())).items()}
    observation = env.reset()
    if verbose:
        print("origin observation:\n", observation)
    done = False
    
    total_reward = []
    total_steps = 0
    total_legals_actions = []
    start = time.time()
    
    while not done:
        validate_move = env.get_validate_actions()
        validate_list = []       
        for i in validate_move:
            if i in available_actions:
                validate_list.append(available_actions.get(i))
        
        if config["mode"] == "random":
            action = choice(validate_list)
            
        elif config["mode"] == "greedy_a":
            temp_reward_dict = {}
            for validate_action in validate_list:
                reward = env.simulation_step(validate_action)
                temp_reward_dict[validate_action] = reward  
            action = max(temp_reward_dict, key=temp_reward_dict.get)
            
            if verbose:
                print(temp_reward_dict)
        
        observation, reward, done, info = env.step(action)
        
        total_reward.append(reward)
        total_steps += 1
        total_legals_actions.append(len(validate_list))
        
        if verbose:
            print("validate actions:", validate_move)
            print("validate lise:", validate_list)
            print("action:", action)
            print("reward:", reward, "\t total reward:", np.sum(total_reward))
            print("-" * 20)
            print("observation:\n", observation)
        
    end = time.time()
    return np.sum(total_reward), total_steps, np.mean(total_legals_actions), end-start
    

if __name__ == "__main__":
    import faulthandler
    faulthandler.enable()
    
    get_info = lambda x: dict(mean=np.mean(x), min=np.min(x), max=np.max(x), std=np.std(x))
    
    parser = argparse.ArgumentParser()
    parser.add_argument('-f', "--config", type=str, default="config.yaml")
    parser.add_argument('-c', '--cpu-workers', type=int, default=multiprocessing.cpu_count())
    parser.add_argument('-v', '--verbose', action='store_true')
    args = parser.parse_args()
    config = yaml.safe_load(Path(args.config).read_text())
    
    print("Number of cpu can use:", args.cpu_workers)
    print("Use config file:", Path(args.config))
    
    with concurrent.futures.ThreadPoolExecutor(max_workers=args.cpu_workers) as executor:
        try:
            for case_config in config["case"]:
                rewards = []
                steps = []
                legal_actions = []
                use_times = []
                futures = [executor.submit(play_game, case_config, args.verbose) for i in range(config["number_of_game_each_case_plays"])]
                try:
                    for future in concurrent.futures.as_completed(futures):
                        reward, step, legal_action, use_time = future.result()
                        rewards.append(reward)
                        steps.append(step)
                        legal_actions.append(legal_action)
                        use_times.append(use_time)
                        
                    print("case:", case_config, end="\n\n")
                    print("rewards:", get_info(rewards), end="\n\n")
                    print("steps:", get_info(steps), end="\n\n")
                    print("legal_actions:", get_info(legal_actions), end="\n\n")
                    print("use_times:", get_info(use_times), end="\n\n")
                except KeyboardInterrupt:
                    break
        except KeyboardInterrupt:
            executor.shutdown(wait=False, cancel_futures=True)

