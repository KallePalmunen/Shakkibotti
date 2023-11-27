from alphaonetest import *

args = {
    'C': 2,
    'num_searches': 600,
    'dirichlet_epsilon': 0.1,
    'dirichlet_alpha': 0.3,
    'search': True,
    'temperature': 0,
}

game = TicTacToe()

play(args, game)