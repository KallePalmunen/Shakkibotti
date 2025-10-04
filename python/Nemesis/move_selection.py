import nemesis as nms
from SigmaZero.rules import *
from notation_to_move_vector import *
import numpy as np

def select_move(move_vector_n, state, move, color, kingmoved, rookmoved, pieces, enpassant):
    moves = []
    move_vector_results = {}
    unnormalized_parameter=random() #fetch these parameters from file created after training process (they are defined there)
    normalized_parameter=random()
    for y0 in range(8):
        for x0 in range(8):
            piece = state[y0,x0]
            if piece == 0 or np.sign(piece) != color:
                continue
            for y1 in range(8):
                for x1 in range(8):
                    if canmove(state, piece, y0, x0, y1, x1, kingmoved, rookmoved, pieces, enpassant):
                        move = [piece, y0, x0, y1, x1]
                        moves.append(move)
    for move in moves:
            move_vector = create_move_vector(state, move, color, kingmoved, rookmoved, pieces, enpassant)
            move_vector_normalized = move_vector/np.linalg.norm(move_vector)
            move_vector_results[move]=unnormalized_parameter*(np.dot(move_vector_n, move_vector))+normalized_parameter*(np.dot(move_vector_n, move_vector_normalized))
    selected_move = max(move_vector_results, key=move_vector_results.get)
    return selected_move