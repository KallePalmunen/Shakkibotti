
import sys
import os

# Get the directory of the current script
script_dir = os.path.dirname(os.path.abspath(__file__))
# Add the parent folder (one level up) to sys.path
parent_dir = os.path.dirname(script_dir)
sys.path.append(parent_dir)

from SigmaZero.rules import *
pawn_position_eval = [[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],[0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5]
,[0.1,0.1,0.2,0.3,0.3,0.2,0.1,0.1],[0.05,0.05,0.1,0.25,0.25,0.1,0.05,0.05],[0.0,0.0,0.0,0.2,0.2,0.0,0.0,0.0]
,[0.05,-0.05,-0.1,0.0,0.0,-0.1,-0.05,0.05],[0.05,0.1,0.1,-0.2,-0.2,0.1,0.1,0.05],[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]]
knight_position_eval = [[-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5],[-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4]
,[-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3],[-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3],[-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3]
,[-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3],[-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4],[-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5]]
bishop_position_eval = [[-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2],[-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1]
,[-0.1,0.0,0.05,0.1,0.1,0.05,0.0,-0.1],[-0.1,0.05,0.05,0.1,0.1,0.05,0.05,-0.1],[-0.1,0.0,0.1,0.1,0.1,0.1,0.0,-0.1]
,[-0.1,0.1,0.01,0.1,0.1,0.1,0.1,-0.1],[-0.1,0.05,0.0,0.0,0.0,0.0,0.05,-0.1],[-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2]]
rook_position_eval = [[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],[0.05,0.1,0.1,0.1,0.1,0.1,0.1,0.05]
,[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05]
,[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[0.0,0.0,0.0,0.5,0.5,0.0,0.0,0.0]]
queen_position_eval = [[-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2],[-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1]
,[-0.1,0.0,0.05,0.05,0.05,0.05,0.0,-0.1],[-0.05,0.0,0.05,0.05,0.05,0.05,0.0,-0.05],[0.0,0.0,0.05,0.05,0.05,0.05,0.0,-0.05]
,[-0.1,0.05,0.05,0.05,0.05,0.05,0.0,-0.1],[-0.1,0.0,0.05,0.0,0.0,0.0,0.0,-0.1],[-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2]]
king_position_eval = [[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3]
,[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.2,-0.3,-0.3,-0.4,-0.4,-0.3,-0.3,-0.2]
,[-0.1,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.1],[0.2,0.2,0.0,0.0,0.0,0.0,0.2,0.2],[0.2,0.3,0.1,0.0,0.0,0.1,0.3,0.2]]

def piece_value(piece):
    if piece == 0:
        return 0
    if abs(piece) < 10:
        return math.copysign(1, piece)
    if abs(piece) < 30:
        return math.copysign(3, piece)
    if abs(piece) < 40:
        return math.copysign(5, piece)
    if abs(piece) < 50:
        return math.copysign(9, piece)
    if abs(piece) == 50:
        return math.copysign(1000000, piece)
    return 0

def material_gain(state, piece, y0, x0, y1, x1, color):
    if abs(piece)<10 and (y1==2 and color == -1 ) and state[y1,x1] == 0:
        if 0<state[y1+1, x1]<10:
            taken_piece = state[y1+1,x1]
        
    elif abs(piece)<10 and (y1 == 5 and color == 1) and state[y1,x1] == 0:
        if state[y1-1, x1]<0:
            taken_piece = state[y1-1,x1]
    else: 
        taken_piece = state[y1,x1]
    ### TODO: add enpassant material gain
    
    gain = -color*piece_value(taken_piece)
    if promote(piece, y1):
        gain += 8.0
    return gain

def piece_position_eval(piece, color, y, x):
    transformed_y = (piece<0)*y+(piece>0)*(7-y)
    transformed_x = (piece<0)*x+(piece>0)*(7-x)
    if piece == 0:
        return 0
    if abs(piece) < 10:
        return pawn_position_eval[transformed_y][transformed_x]
    if abs(piece) < 20:
        return knight_position_eval[transformed_y][transformed_x]
    if abs(piece) < 30:
        return bishop_position_eval[transformed_y][transformed_x]
    if abs(piece) < 40:
        return rook_position_eval[transformed_y][transformed_x]
    if abs(piece) < 50:
        return queen_position_eval[transformed_y][transformed_x]
    if abs(piece) == 50:
        return king_position_eval[transformed_y][transformed_x]
    return 0

def create_move_vector(state, move, color):
    move_vector = []
    piece, y0, x0, y1, x1 = move

    move_vector += [material_gain(state, piece, y0, x0, y1, x1, color)]
    move_vector += [piece_position_eval(piece, color, y1, x1)-piece_position_eval(piece, color, y0, x0)]

    return move_vector

#testing
state = np.array([[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,0,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,7,-8],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,0],[-30,-10,-20,-50,-40,-21,-11,-31]])

print(state[4,6])
print(state[5,7])
move = [7,4,6,5,7]
color = 1
move_vector = create_move_vector(state, move, color)
print(move_vector)