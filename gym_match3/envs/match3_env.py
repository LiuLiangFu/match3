import gym
from gym import error, spaces, utils
from gym.utils import seeding

from gym_match3.envs.game import Game, Point
from gym_match3.envs.game import OutOfBoardError, ImmovableShapeError
from gym_match3.envs.levels import LEVELS, Match3Levels
from gym_match3.envs.renderer import Renderer

from itertools import product
import warnings

BOARD_NDIM = 2


class Match3Env(gym.Env):
    metadata = {'render.modes': None}

    def __init__(self, immovable_move, n_of_match_counts_immov,
                 match_counts_add_immovable, number_of_match_counts_add_immovable,
                 step_add_immovable, number_of_step_add_immovable,
                 no_legal_actions_do, immovable_shape=-1,
                 rollout_len=-1, all_moves=False, levels=None, random_state=None):
        self.no_legal_actions_do = no_legal_actions_do
        self.rollout_len = rollout_len
        self.random_state = random_state
        self.all_moves = all_moves
        self.levels = levels or Match3Levels(LEVELS)
        self.h = self.levels.h
        self.w = self.levels.w
        self.n_shapes = self.levels.n_shapes
        self.__episode_counter = 0

        self.__game = Game(rows=self.h, columns=self.w, n_shapes=self.n_shapes, length=3,
                           immovable_move=immovable_move,
                           n_of_match_counts_immov=n_of_match_counts_immov,
                           match_counts_add_immovable=match_counts_add_immovable,
                           number_of_match_counts_add_immovable=number_of_match_counts_add_immovable,
                           step_add_immovable=step_add_immovable,
                           number_of_step_add_immovable=number_of_step_add_immovable,
                           no_legal_actions_do=no_legal_actions_do,
                           all_moves=all_moves,
                           immovable_shape=immovable_shape,
                           random_state=self.random_state)
        self.reset()
        self.renderer = Renderer(self.n_shapes)

        # setting observation space
        self.observation_space = spaces.Box(
            low=0,
            high=self.n_shapes,
            shape=self.__game.board.board_size,
            dtype=int)

        # setting actions space
        self.__match3_actions = self.get_available_actions()
        self.action_space = spaces.Discrete(
            len(self.__match3_actions))

    @staticmethod
    def __get_directions(board_ndim):
        """ get available directions for any number of dimensions """
        directions = [
            [[0 for _ in range(board_ndim)] for _ in range(2)]
            for _ in range(board_ndim)
        ]
        for ind in range(board_ndim):
            directions[ind][0][ind] = 1
            directions[ind][1][ind] = -1
        return directions

    def __points_generator(self):
        """ iterates over points on the board """
        rows, cols = self.__game.board.board_size
        points = [Point(i, j) for i, j in product(range(rows), range(cols))]
        for point in points:
            yield point

    def get_available_actions(self):
        """ calculate available actions for current board sizes """
        actions = []
        directions = [[1, 0], [0, 1]]
        for dir_ in directions:
            for point in self.__points_generator():
                dir_p = Point(*dir_)
                new_point = point + dir_p
                try:
                    _ = self.__game.board[new_point]
                    actions.append((point, new_point))
                except OutOfBoardError:
                    continue
        return actions
    
    def get_validate_actions(self):
        possible = self.__game.get_all_possible_moves()
        validate_actions = []
        for point, direction in possible:
            newpoint =  point +  Point(*direction)
            validate_actions.append((newpoint, point))
        return validate_actions
    
    def __get_action(self, ind):
        return self.__match3_actions[ind]

    def step(self, action):
        # make action
        m3_action = self.__get_action(action)
        reward = self.__swap(*m3_action)
        ob = self.__get_board()

        # change counter even action wasn't successful
        self.__episode_counter += 1
        
        if self.rollout_len > 0 and self.__episode_counter >= self.rollout_len:
            episode_over = True
        elif self.no_legal_actions_do == "terminal" and len(self.__game.get_all_possible_moves(False)) == 0:
            episode_over = True
        else:
            episode_over = False
            
        return ob, reward, episode_over, {}
    
    def simulation_step(self, action):
        m3_action = self.__get_action(action)
        reward = self.__game.simulation_swap(*m3_action)
        return reward

    def reset(self, *args, **kwargs):
        self.__episode_counter = 0
        board = self.levels.sample()
        self.__game.start(board)
        return self.__get_board()

    def __swap(self, point1, point2):
        try:
            reward = self.__game.swap(point1, point2)
        except ImmovableShapeError:
            reward = 0
        return reward

    def __get_board(self):
        return self.__game.board.board.copy()

    def render(self, mode='human', close=False):
        if close:
            warnings.warn("close=True isn't supported yet")
        self.renderer.render_board(self.__game.board)
      
        
def Getlevels(WidthAndHeight, shapes):
    import numpy as np
    from gym_match3.envs.levels import Level
    new_level = [Level(WidthAndHeight, WidthAndHeight, shapes, np.zeros((WidthAndHeight, WidthAndHeight)).tolist())]
    return Match3Levels(new_level)
       
if __name__ == "__main__":
    import faulthandler
    from random import choice
    faulthandler.enable()
    test = Match3Env(True, 1, False, 0, False, 0, "terminal", levels=Getlevels(5, 5), rollout_len=-1)
    for _ in range(1):
        available_actions = {v : k for k, v in dict(enumerate(test.get_available_actions())).items()}
        print("available:", available_actions)
        
        obs = test.reset()
        print(obs)
        done = False
        while not done:
            validate_move = test.get_validate_actions()
            print("validate:", validate_move)
            validate_list = []       
            for i in validate_move:
                if i in available_actions:
                    validate_list.append(available_actions.get(i))
            print(validate_list)
            action = choice(validate_list)
            print("action:", action)
            observation, reward, done, info = test.step(action)
            print(observation)
            print("reward:", reward)
            
