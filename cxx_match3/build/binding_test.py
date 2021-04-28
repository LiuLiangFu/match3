import module_name

game = module_name.Game(6, 6, 4, True, 1, False, 0, True, 5, 3, 0, -1, -1, -2, False, "terminal", True)
game.start()
game.apply_action(0)
game.current_player
#print(game.simulation_apply_action())
print(game.is_terminal())
print(game.legal_actions())

