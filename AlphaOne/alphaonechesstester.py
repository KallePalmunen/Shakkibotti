from alphaonetest import *

args = {
    'C': 2,
    'num_searches': 600,
    'dirichlet_epsilon': 0.1,
    'dirichlet_alpha': 0.3,
    'search': True,
    'temperature': 0,
}

game = Chess()

board = game.get_initial_state()
player = 1
moves = game.get_valid_moves(board, player)
