from alphaonetest import *

args = {
    'C': 2,
    'num_searches': 100,
    'dirichlet_epsilon': 0.1,
    'dirichlet_alpha': 0.3,
    'search': True,
    'temperature': 0,
}

game = Chess()
model_dict = "model_2_Chess.pt"

play(args, game, model_dict)