from sigmazero import *

args = {
    'C': 2,
    'num_searches': 100,
    'num_iterations': 3,
    'num_selfPlay_iterations': 15,
    'num_parallel_games': 100,
    'num_epochs': 4,
    'batch_size': 128,
    'temperature': 1.25,
    'dirichlet_epsilon': 0.25,
    'dirichlet_alpha': 0.3
}

game = Chess()

learn(args, game)