import numpy as np
print(np.__version__)

import torch
print(torch.__version__)

import torch.nn as nn
import torch.nn.functional as F

#torch.manual_seed(0) #set the same seed for pytorch every time to ensure reproducibility

import random
import math
import time
from SigmaZero.Chess import *

def normalize(score):
    return 1.95 / (1 + np.exp(-0.5*score)) - 1

def print_policy_heatmap(policy):
    for i in range(0, 64, 8):
        slice_rounded = np.around(policy[i:i+8], decimals=2)
        print(" ".join(["{:7.2f}".format(item) for item in slice_rounded]))

class Chess:
    def __init__(self): #run when class is initiated
        self.row_count = 8
        self.column_count = 8
        self.action_size = self.row_count * self.column_count
        self.kingmoved = [0, 0]
        self.rookmoved = [[0,0],[0,0]]
        self.pieces = [[8,8],[2,2],[2,2],[2,2],[1,1],[1,1]]
        self.enpassant = -1
        self.max_search_depth = 2
        self.max_game_length = 100
        self.start_eval = 0.0
        #evaluation arrays
        self.pawn_position_eval = [[80.0,80.0,80.0,80.0,80.0,80.0,80.0,80.0],[0.5,0.5,0.5,0.5,0.5,0.5,0.5,0.5],[0.1,0.1,0.2,0.3,0.3,0.2,0.1,0.1],[0.05,0.05,0.1,0.25,0.25,0.1,0.05,0.05],[0.0,0.0,0.0,0.2,0.2,0.0,0.0,0.0],[0.05,-0.05,-0.1,0.0,0.0,-0.1,-0.05,0.05],[0.05,0.1,0.1,-0.2,-0.2,0.1,0.1,0.05],[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]]
        self.knight_position_eval = [[-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5],[-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4],[-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3],[-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3],[-0.3,0.05,0.15,0.2,0.2,0.15,0.05,-0.3],[-0.3,0.0,0.1,0.15,0.15,0.1,0.0,-0.3],[-0.4,-0.2,0.0,0.0,0.0,0.0,-0.2,-0.4],[-0.5,-0.4,-0.3,-0.3,-0.3,-0.3,-0.4,-0.5]]
        self.bishop_position_eval = [[-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2],[-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1],[-0.1,0.0,0.05,0.1,0.1,0.05,0.0,-0.1],[-0.1,0.05,0.05,0.1,0.1,0.05,0.05,-0.1],[-0.1,0.0,0.1,0.1,0.1,0.1,0.0,-0.1],[-0.1,0.1,0.01,0.1,0.1,0.1,0.1,-0.1],[-0.1,0.05,0.0,0.0,0.0,0.0,0.05,-0.1],[-0.2,-0.1,-0.1,-0.1,-0.1,-0.1,-0.1,-0.2]]
        self.rook_position_eval = [[0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0],[0.05,0.1,0.1,0.1,0.1,0.1,0.1,0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[-0.05,0.0,0.0,0.0,0.0,0.0,0.0,-0.05],[0.0,0.0,0.0,0.5,0.5,0.0,0.0,0.0]]
        self.queen_position_eval = [[-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2],[-0.1,0.0,0.0,0.0,0.0,0.0,0.0,-0.1],[-0.1,0.0,0.05,0.05,0.05,0.05,0.0,-0.1],[-0.05,0.0,0.05,0.05,0.05,0.05,0.0,-0.05],[0.0,0.0,0.05,0.05,0.05,0.05,0.0,-0.05],[-0.1,0.05,0.05,0.05,0.05,0.05,0.0,-0.1],[-0.1,0.0,0.05,0.0,0.0,0.0,0.0,-0.1],[-0.2,-0.1,-0.1,-0.05,-0.05,-0.1,-0.1,-0.2]]
        self.king_position_eval = [[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.3,-0.4,-0.4,-0.5,-0.5,-0.4,-0.4,-0.3],[-0.2,-0.3,-0.3,-0.4,-0.4,-0.3,-0.3,-0.2],[-0.1,-0.2,-0.2,-0.2,-0.2,-0.2,-0.2,-0.1],[0.2,0.2,0.0,0.0,0.0,0.0,0.2,0.2],[0.2,0.3,0.1,0.0,0.0,0.1,0.3,0.2]]
        self.can_move_positions = [np.array([[1,0],[2,0],[1,1],[1,-1]]), np.array([[1,2],[2,1],[2,-1],[1,-2],[-1,-2],[-2,-1],[-2,1],[-1,2]]), np.array([[1,1],[2,2],[3,3],[4,4],[5,5],[6,6],[7,7],[1,-1],[2,-2],[3,-3],[4,-4],[5,-5],[6,-6],[7,-7],[-1,1],[-2,2],[-3,3],[-4,4],[-5,5],[-6,6],[-7,7],[-1,-1],[-2,-2],[-3,-3],[-4,-4],[-5,-5],[-6,-6],[-7,-7]]), np.array([[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7],[0,-1],[0,-2],[0,-3],[0,-4],[0,-5],[0,-6],[0,-7],[1,0],[2,0],[3,0],[4,0],[5,0],[6,0],[7,0],[-1,0],[-2,0],[-3,0],[-4,0],[-5,0],[-6,0],[-7,0]]),np.array([[1,1],[2,2],[3,3],[4,4],[5,5],[6,6],[7,7],[1,-1],[2,-2],[3,-3],[4,-4],[5,-5],[6,-6],[7,-7],[-1,1],[-2,2],[-3,3],[-4,4],[-5,5],[-6,6],[-7,7],[-1,-1],[-2,-2],[-3,-3],[-4,-4],[-5,-5],[-6,-6],[-7,-7],[0,1],[0,2],[0,3],[0,4],[0,5],[0,6],[0,7],[0,-1],[0,-2],[0,-3],[0,-4],[0,-5],[0,-6],[0,-7],[1,0],[2,0],[3,0],[4,0],[5,0],[6,0],[7,0],[-1,0],[-2,0],[-3,0],[-4,0],[-5,0],[-6,0],[-7,0]]), np.array([[0,1],[1,1],[1,0],[1,-1],[0,-1],[-1,-1],[-1,0],[-1,1]])]

    def __repr__(self): #string representation of this class
        return "Chess"
    
    def get_initial_state(self):
        return np.array([[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]])
    
    def get_next_state(self, state, startsquare_action, endsquare_action, player):
        #get y0, x0, y1, x1 by inversing convert_to_action
        y0 = startsquare_action // self.column_count
        x0 = startsquare_action % self.column_count
        y1 = endsquare_action // self.column_count
        x1 = endsquare_action % self.column_count
        
        moved_piece = state[y0, x0]
        if(abs(moved_piece) == 50):
            if abs(x1 - x0) > 1:
                whichrook = int(math.copysign(30 + (x1 > 4), moved_piece))
                whichrook_y = np.where(state[y1] == whichrook)[0]
                state[y1, whichrook_y] = 0
                state[y1, x1 + int(math.copysign(1, 4-x1))] = whichrook
            self.kingmoved[(moved_piece > 0)] = 1
        state[y0,x0] = 0
        if(abs(moved_piece) == 30 or abs(moved_piece) == 31):
            self.rookmoved[(moved_piece >0)][abs(moved_piece)-30] = 1
        if promote(moved_piece, y1):
            promoteto = 4
            state[y1,x1] = int(math.copysign(1, moved_piece))*(promoteto*10+self.pieces[promoteto][(moved_piece < 0)])
            #self.pieces[promoteto][(moved_piece < 0)] += 1
        else:
            state[y1,x1] = moved_piece
        if self.enpassant >= 0 and x1*8+y1 == self.enpassant:
            state[y1-int(math.copysign(1, y1 - y0)),y1] = 0
        if abs(moved_piece) < 10 and abs(y1 - y0) > 1:
            enpassant = x1*8+y0+int(math.copysign(1, y1 - y0))
        else:
            enpassant = -1
        return state
    
    def get_possible_ensquares(self, piece_type, y0, x0):
        can_move_positions = self.can_move_positions[piece_type]
        possible_endsquares = can_move_positions + np.array([y0,x0])
        return possible_endsquares[(possible_endsquares[:,0] >= 0) & (possible_endsquares[:,1] >= 0) & (possible_endsquares[:,0] < 8) & (possible_endsquares[:,1] < 8)]
    
    def get_valid_endsquares(self, state, y0, x0):
        valid_moves = np.zeros(self.action_size)
        piece = state[y0,x0]
        possible_endsquares = self.get_possible_ensquares(piece//10, y0, x0)
        number_of_possible_ensquares = len(possible_endsquares)
        for i in range(number_of_possible_ensquares):
            y1 = possible_endsquares[i,0]
            x1 = possible_endsquares[i,1]
            if canmove(state, piece, y0, x0, y1, x1, self.kingmoved, self.rookmoved, self.pieces, self.enpassant):
                valid_moves[y1*self.column_count+x1] = 1

        return (valid_moves.reshape(-1)).astype(np.uint8)
    
    def bot_get_valid_endsquares(self, state, y0, x0):
        valid_moves = np.zeros(self.action_size)
        piece = state[y0,x0]
        possible_endsquares = self.get_possible_ensquares(piece//10, y0, x0)
        number_of_possible_ensquares = len(possible_endsquares)
        for i in range(number_of_possible_ensquares):
            y1 = possible_endsquares[i,0]
            x1 = possible_endsquares[i,1]
            if botpiecemove(state, piece, y0, x0, y1, x1, self.kingmoved, self.rookmoved, self.pieces, self.enpassant):
                valid_moves[y1*self.column_count+x1] = 1

        return (valid_moves.reshape(-1)).astype(np.uint8)
    
    def valid_moves_exist(self, state, y0, x0):
        if state[y0,x0] <= 0:
            return False

        for y1 in range(self.row_count):
            for x1 in range(self.column_count):
                if canmove(state, state[y0,x0], y0, x0, y1, x1, self.kingmoved, self.rookmoved, self.pieces, self.enpassant):
                    return True
        
        return False
    
    def bot_valid_moves_exist(self, state, y0, x0):
        piece = state[y0,x0]
        if piece <= 0 or piece > 50:
            return False
        
        possible_endsquares = self.get_possible_ensquares(piece//10, y0, x0)
        number_of_possible_ensquares = len(possible_endsquares)
        for i in range(number_of_possible_ensquares):
            y1 = possible_endsquares[i,0]
            x1 = possible_endsquares[i,1]
            if botpiecemove(state, piece, y0, x0, y1, x1, self.kingmoved, self.rookmoved, self.pieces, self.enpassant):
                return True

        return False
    
    def get_valid_startsquares(self, state):
        valid_moves = np.zeros(self.action_size)
        for y0 in range(self.row_count):
            for x0 in range(self.column_count):
                if(self.valid_moves_exist(state, y0, x0)):
                    valid_moves[y0*self.column_count+x0] = 1
        return valid_moves
    
    def bot_get_valid_startsquares(self, state):
        valid_moves = np.zeros(self.action_size)
        for y0 in range(self.row_count):
            for x0 in range(self.column_count):
                if(self.bot_valid_moves_exist(state, y0, x0)):
                    valid_moves[y0*self.column_count+x0] = 1
        return valid_moves
        
    def check_win(self, state, startsquare_action, endsquare_action, player):
        if startsquare_action == None or endsquare_action == None:
            return False

        if checkmate(state, -player*50, self.kingmoved, self.rookmoved, self.pieces, self.enpassant):
            return True
        return False

    def evaluation(self, state):
        evaluation = 0
        for y in range(8):
            for x in range(8):
                n = state[y, x]
                #pawns
                if(n != 0 and abs(n) < 9):
                    evaluation += np.sign(n)*(1+0.1*self.pawn_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)])
                
                #knights and bishops
                elif(abs(n) > 9 and abs(n) < 20):
                    evaluation += np.sign(n)*(3+0.1*self.knight_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)])
                
                #bishops
                elif(abs(n) > 19 and abs(n) < 30):
                    evaluation += np.sign(n)*(3+0.1*self.bishop_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)])
                
                #rooks
                elif(abs(n) > 29 and abs(n) < 40):
                    evaluation += np.sign(n)*(5+0.1*self.rook_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)])
                
                #queens
                elif(abs(n) > 39 and abs(n) < 50):
                    evaluation += np.sign(n)*(9+0.1*self.queen_position_eval[(n<0)*y+(n>0)*(7-y)][(n<0)*x+(n>0)*(7-x)])
        return -(evaluation-self.start_eval)
    
    def valid_endsquares_exist(self, state, valid_startsquares):
        for startsquare, is_valid in enumerate(valid_startsquares):
            if is_valid == 1:
                y0 = startsquare // self.column_count
                x0 = startsquare % self.column_count
                if np.sum(self.get_valid_endsquares(state, y0, x0)) != 0:
                    return True
        return False
    
    def get_value_and_terminated(self, state, startsquare_action, endquare_action, depth, player, move_count = 0):
        if self.check_win(state, startsquare_action, endquare_action, player):
            return 1, True
        #to do: should we also check for valid endsquares?
        valid_startsquares = self.get_valid_startsquares(state)
        if np.sum(valid_startsquares) == 0:
            return 0, True
        if not self.valid_endsquares_exist(state, valid_startsquares):
            return 0, True
        if depth >= self.max_search_depth:
            return normalize(self.evaluation(state)), True
        if move_count >= self.max_game_length:
            return normalize(self.evaluation(state)), True
        return 0, False
    
    def bot_get_value_and_terminated(self, state, startsquare_action, endquare_action, depth, player):
        if self.check_win(state, startsquare_action, endquare_action, player):
            return 1, True
        #to do: should we also check for valid endsquares?
        valid_startsquares = self.bot_get_valid_startsquares(state)
        if np.sum(valid_startsquares) == 0:
            return 0, True
        if depth >= self.max_search_depth:
            return normalize(self.evaluation(state)), True
        return 0, False
    
    def get_opponent(self, player):
        return -player
    
    def get_opponent_value(self, value):
        return -value
    
    def change_perspective(self, state, player):
        return np.flipud(state * player) #flips the board and changes the signs
        #this is slow
    
    def get_encoded_state(self, state):
        #encoded_state is a matrix where each row represents whether each element in the state array is a white or black pawn, knight, bishop,
        #rook, queen, king, or empty
        encoded_state = np.stack(
            (np.logical_and(state > 0, state < 10), np.logical_and(state >= 10, state < 20), np.logical_and(state >= 20, state < 30)
             , np.logical_and(state >= 30, state < 40), np.logical_and(state >= 40, state < 50), state == 50, state == 0
             , np.logical_and(state < 0, state > -10), np.logical_and(state <= -10, state > -20), np.logical_and(state <= -20, state > -30)
             , np.logical_and(state <= -30, state > -40), np.logical_and(state <= -40, state > -50), state == -50)
        ).astype(np.float32)
        
        if len(state.shape) == 3:
            #swaps vertical and horizontal axes
            encoded_state = np.swapaxes(encoded_state, 0, 1)
        
        return encoded_state
    
    def convert_to_action(self, y0, x0, y1, x1):
        return (self.column_count*y0 + x0, + y1*self.column_count + x1)
    
class ResNet(nn.Module):
    def __init__(self, game, num_resBlocks, num_hidden, device, number_of_input_channels):
        super().__init__() #initiates the parent class
        
        self.device = device
        self.startBlock = nn.Sequential(
            nn.Conv2d(number_of_input_channels, num_hidden, kernel_size=3, padding=1), #convolutional layer(input channels, output channels, size of layer, match the input size with output size)
            nn.BatchNorm2d(num_hidden), #normalize output
            nn.ReLU()
        )
        
        #create a backbone for neural network by creating multiple Resblocks
        self.backBone = nn.ModuleList(
            [ResBlock(num_hidden) for i in range(num_resBlocks)]
        )
        
        self.policyHead_startsquare = nn.Sequential(
            nn.Conv2d(num_hidden, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Flatten(), #takes each element from a tensor to an array
            nn.Linear(32 * game.row_count * game.column_count, game.action_size) #linear transformation(input size, output size)
        )

        self.policyHead_endsquare = nn.Sequential(
            nn.Conv2d(num_hidden, 32, kernel_size=3, padding=1),
            nn.BatchNorm2d(32),
            nn.ReLU(),
            nn.Flatten(), #takes each element from a tensor to an array
            nn.Linear(32 * game.row_count * game.column_count, game.action_size) #linear transformation(input size, output size)
        )
        
        self.valueHead = nn.Sequential(
            nn.Conv2d(num_hidden, 3, kernel_size=3, padding=1),
            nn.BatchNorm2d(3),
            nn.ReLU(),
            nn.Flatten(),
            nn.Linear(3 * game.row_count * game.column_count, 1), #tranform to single value
            nn.Tanh() #activation function
        )
        
        self.to(device) #move to the device where you want to run the operations
    
    #iterate through the neural network
    def forward(self, x):
        x = self.startBlock(x)
        for resBlock in self.backBone:
            x = resBlock(x)
        startsquare_policy = self.policyHead_startsquare(x)
        endsquare_policy = self.policyHead_endsquare(x)
        value = self.valueHead(x)
        return startsquare_policy, endsquare_policy, value
        
        
class ResBlock(nn.Module):
    def __init__(self, num_hidden):
        super().__init__()
        self.conv1 = nn.Conv2d(num_hidden, num_hidden, kernel_size=3, padding=1)
        self.bn1 = nn.BatchNorm2d(num_hidden)
        self.conv2 = nn.Conv2d(num_hidden, num_hidden, kernel_size=3, padding=1)
        self.bn2 = nn.BatchNorm2d(num_hidden)
        
    def forward(self, x):
        residual = x
        x = F.relu(self.bn1(self.conv1(x)))
        x = self.bn2(self.conv2(x))
        x += residual
        x = F.relu(x)
        return x

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

class Node:
    def __init__(self, game, args, state, parent=None, startsquare_action_taken=None, endsquare_action_taken=None, probability=0, visit_count=0, depth = 0):
        self.game = game
        self.args = args
        self.state = state
        self.parent = parent
        self.startsquare_action_taken = startsquare_action_taken
        self.endsquare_action_taken = endsquare_action_taken
        self.probability = probability
        
        self.children = []
        
        self.visit_count = visit_count
        self.value_sum = 0
        self.depth = depth
        
    def is_fully_expanded(self):
        return len(self.children) > 0
    
    def select(self):
        best_child = None
        best_ucb = -np.inf
        
        for child in self.children:
            ucb = self.get_ucb(child)
            if ucb > best_ucb:
                best_child = child
                best_ucb = ucb
                
        return best_child
    
    def get_ucb(self, child):
        if child.visit_count == 0:
            q_value = 0
        else:
            q_value = 1 - ((child.value_sum / child.visit_count) + 1) / 2
        return q_value + self.args['C'] * (math.sqrt(self.visit_count) / (child.visit_count + 1)) * child.probability
    
    def expand(self, state, startsquare_policy, endsquare_policy):
        for startsquare_action, startsquare_probability in enumerate(startsquare_policy):
            if startsquare_probability > 0:
                y0 = startsquare_action // self.game.column_count
                x0 = startsquare_action % self.game.column_count
                valid_endsquares = self.game.bot_get_valid_endsquares(state, y0, x0)

                endsquare_policy_for_startsquare = np.copy(endsquare_policy) #create a copy to not affect the original
                endsquare_policy_for_startsquare *= valid_endsquares
                if np.sum(endsquare_policy_for_startsquare) > 0:
                    endsquare_policy_for_startsquare /= np.sum(endsquare_policy_for_startsquare)
                    for endquare_action, endsquare_probability in enumerate(endsquare_policy_for_startsquare):
                        if endsquare_probability > 0:
                            child_state = self.state.copy()
                            child_state = self.game.get_next_state(child_state, startsquare_action, endquare_action, 1)
                            child_state = self.game.change_perspective(child_state, player=-1)
                            
                            probability = startsquare_probability*endsquare_probability

                            child = Node(self.game, self.args, child_state, self, startsquare_action, endquare_action, probability, depth = self.depth + 1)
                            self.children.append(child)
        return child

            
    def backpropagate(self, value):
        self.value_sum += value
        self.visit_count += 1
        
        value = self.game.get_opponent_value(value)
        if self.parent is not None:
            self.parent.backpropagate(value)  


class MCTS:
    def __init__(self, game, args, model):
        self.game = game
        self.args = args
        self.model = model
        
    @torch.no_grad() #don't track gradients for backpropagation
    def search(self, state):
        root = Node(self.game, self.args, state, visit_count=1, depth = 0)
        
        self.game.start_eval = 0.0
        self.game.start_eval = -self.game.evaluation(state)
        startsquare_policy, endsquare_policy, _ = self.model(
            torch.tensor(self.game.get_encoded_state(state), device=self.model.device).unsqueeze(0)
        )
        startsquare_policy = torch.softmax(startsquare_policy, axis=1).squeeze(0).cpu().numpy()
        endsquare_policy = torch.softmax(endsquare_policy, axis=1).squeeze(0).cpu().numpy()
        #add randomness
        startsquare_policy = (1 - self.args['dirichlet_epsilon']) * startsquare_policy + self.args['dirichlet_epsilon'] \
            * np.random.dirichlet([self.args['dirichlet_alpha']] * self.game.action_size)
        endsquare_policy = (1 - self.args['dirichlet_epsilon']) * endsquare_policy + self.args['dirichlet_epsilon'] \
            * np.random.dirichlet([self.args['dirichlet_alpha']] * self.game.action_size)
        
        valid_startsquares = self.game.get_valid_startsquares(state)
        startsquare_policy *= valid_startsquares
        startsquare_policy /= np.sum(startsquare_policy)

        root.expand(state, startsquare_policy, endsquare_policy)
        
        for search in range(self.args['num_searches']):
            node = root
            
            while node.is_fully_expanded():
                node = node.select()
            
            value, is_terminal = self.game.bot_get_value_and_terminated(node.state, node.startsquare_action_taken
                , node.endsquare_action_taken, node.depth, -1)
            value = self.game.get_opponent_value(value)
            
            if not is_terminal:
                startsquare_policy, endsquare_policy, value = self.model(
                    torch.tensor(self.game.get_encoded_state(node.state), device=self.model.device).unsqueeze(0)
                )
                startsquare_policy = torch.softmax(startsquare_policy, axis=1).squeeze(0).cpu().numpy()
                endsquare_policy = torch.softmax(endsquare_policy, axis=1).squeeze(0).cpu().numpy()
                valid_startsquares = self.game.bot_get_valid_startsquares(node.state)
                #get two policies: one for startsquare and one for endsquare
                startsquare_policy *= valid_startsquares
                startsquare_policy /= np.sum(startsquare_policy)
                
                value = value.item()
                node.expand(node.state, startsquare_policy, endsquare_policy)
                
            node.backpropagate(value)    
            
        startsquare_action_probs = np.zeros(self.game.action_size)
        endsquare_action_probs = np.zeros(self.game.action_size)
        for child in root.children:
            startsquare_action_probs[child.startsquare_action_taken] += child.visit_count
            endsquare_action_probs[child.endsquare_action_taken] += child.visit_count
        startsquare_action_probs /= np.sum(startsquare_action_probs)
        endsquare_action_probs /= np.sum(endsquare_action_probs)
        return startsquare_action_probs, endsquare_action_probs
    
class SigmaZero:
    def __init__(self, model, optimizer, game, args):
        self.model = model
        self.optimizer = optimizer
        self.game = game
        self.args = args
        self.mcts = MCTS(game, args, model)
        
    def selfPlay(self):
        memory = []
        player = 1
        state = self.game.get_initial_state()
        move_count = 0
        
        while True:
            if player == -1:
                neutral_state = self.game.change_perspective(state, player)
            else:
                neutral_state = state
            startsquare_action_probs, endsquare_action_probs = self.mcts.search(neutral_state)
            
            memory.append((neutral_state, startsquare_action_probs, endsquare_action_probs, player))
            
            temperature_startsquare_action_probs = startsquare_action_probs ** (1 / self.args['temperature']) # Divide temperature_action_probs with its sum in case of an error
            temperature_endsquare_action_probs = endsquare_action_probs ** (1 / self.args['temperature'])
            temperature_startsquare_action_probs /= np.sum(temperature_startsquare_action_probs)
            temperature_endsquare_action_probs /= np.sum(temperature_endsquare_action_probs)
            startsquare_action = np.random.choice(self.game.action_size, p=temperature_startsquare_action_probs)
            endsquare_action = np.random.choice(self.game.action_size, p=temperature_endsquare_action_probs)
            
            if player == -1:
                state = self.game.change_perspective(state,-1)
            state = self.game.get_next_state(state, startsquare_action, endsquare_action, player)
            if player == -1:
                state = self.game.change_perspective(state, -1)
            
            #check if opponent has valid moves

            move_count += 1
            
            value, is_terminal = self.game.get_value_and_terminated(state, startsquare_action, endsquare_action, 0, player, move_count)
            
            if is_terminal:
                returnMemory = []
                for hist_neutral_state, hist_startsquare_action_probs, hist_endsquare_action_probs, hist_player in memory:
                    hist_outcome = value if hist_player == player else self.game.get_opponent_value(value)
                    returnMemory.append((
                        self.game.get_encoded_state(hist_neutral_state),
                        hist_startsquare_action_probs,
                        hist_endsquare_action_probs,
                        hist_outcome
                    ))
                return returnMemory
            
            player = self.game.get_opponent(player)
                
    def train(self, memory):
        random.shuffle(memory)
        for batchIdx in range(0, len(memory), self.args['batch_size']):
            sample = memory[batchIdx:min(len(memory) - 1, batchIdx + self.args['batch_size'])] # Change to memory[batchIdx:batchIdx+self.args['batch_size']] in case of an error
            state, startsquare_policy_targets, endsquare_policy_targets, value_targets = zip(*sample)
            
            state, startsquare_policy_targets, endsquare_policy_targets, value_targets = np.array(state), np.array(startsquare_policy_targets)\
            , np.array(endsquare_policy_targets), np.array(value_targets).reshape(-1, 1)
            
            state = torch.tensor(state, dtype=torch.float32, device=self.model.device)
            startsquare_policy_targets = torch.tensor(startsquare_policy_targets, dtype=torch.float32, device=self.model.device)
            endsquare_policy_targets = torch.tensor(endsquare_policy_targets, dtype=torch.float32, device=self.model.device)
            value_targets = torch.tensor(value_targets, dtype=torch.float32, device=self.model.device)
            
            out_startsquare_policy, out_endsquare_policy, out_value = self.model(state)
            
            startsquare_policy_loss = F.cross_entropy(out_startsquare_policy, startsquare_policy_targets)
            endsquare_policy_loss = F.cross_entropy(out_endsquare_policy, endsquare_policy_targets)
            policy_loss = (startsquare_policy_loss+endsquare_policy_loss)/2 #average loss
            value_loss = F.mse_loss(out_value, value_targets)
            loss = policy_loss + value_loss
            
            self.optimizer.zero_grad()
            loss.backward()
            self.optimizer.step()
    
    def learn(self):
        start = time.time()
        for iteration in range(self.args['num_iterations']):
            memory = []
            
            self.model.eval()
            for selfPlay_iteration in range(self.args['num_selfPlay_iterations']):
                memory += self.selfPlay()
                print(f"{iteration+1}/{self.args['num_iterations']}: {100*(selfPlay_iteration+1)/self.args['num_selfPlay_iterations']}%, estimated time left: {round((time.time()-start)*(self.args['num_iterations']*self.args['num_selfPlay_iterations']-iteration*self.args['num_selfPlay_iterations']-selfPlay_iteration-1+0.01)/(iteration*self.args['num_selfPlay_iterations']+selfPlay_iteration+1+0.01),2)}s")
                
            self.model.train()
            for epoch in range(self.args['num_epochs']):
                self.train(memory)
                print(f"{100*(epoch+1)/self.args['num_epochs']}%")
            
            torch.save(self.model.state_dict(), f"./python/SigmaZero/models/model_{iteration}_{self.game}.pt")
            torch.save(self.optimizer.state_dict(), f"./python/SigmaZero/models/optimizer_{iteration}_{self.game}.pt")

def learn(args, game):
    model = ResNet(game, 4, 512, device=device, number_of_input_channels=13)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    model.train() #training mode
    sigmaZero = SigmaZero(model, optimizer, game, args)
    start_time = time.time()
    sigmaZero.learn()
    print(f"learning time: {time.time()-start_time}s")

def play(args, game, model_dict):
    model = ResNet(game, 4, 512, device=device, number_of_input_channels=13)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    #load previously learned values
    model.load_state_dict(torch.load(model_dict, map_location=device))
    model.eval() #playing mode

    print("Game starts")
    state = game.get_initial_state()
    print(state)

    while True:
        y0 = -1
        x0 = -1
        y1 = -1
        x1 = -1

        action = 0
        while(y0 <= 0 or y0 > game.row_count or x0 <= 0 or x0 > game.column_count):
            y0 = int(input("select row: "))
            x0 = int(input("select column: "))
        
        while(y1 <= 0 or y1 > game.row_count or x1 <= 0 or x1 > game.column_count):
            y1 = int(input("select row: "))
            x1 = int(input("select column: "))

        startsquare_action, endsquare_action = game.convert_to_action(y0-1, x0-1, y1-1, x1-1)

        state = game.get_next_state(state, startsquare_action, endsquare_action, -1)

        print(state)

        state = game.change_perspective(state, -1)
        if game.get_value_and_terminated(state, startsquare_action, endsquare_action, 0, -1)[1]:
            if game.get_value_and_terminated(state, startsquare_action, endsquare_action, 0, -1)[0] == 1:
                print("You win!")
            else:
                print("tie")
            break

        start_time = time.time()

        startsquare_policy, endsquare_policy = MCTS(game, args, model).search(state)
        valid_startsquares = game.get_valid_startsquares(state)

        startsquare_policy *= valid_startsquares
        startsquare_policy /= np.sum(startsquare_policy)
        startsquare_action = int(np.argmax(startsquare_policy))
        y0 = startsquare_action // game.column_count
        x0 = startsquare_action % game.column_count
        
        valid_endsquares = game.get_valid_endsquares(state, y0, x0)
        endsquare_policy *= valid_endsquares
        endsquare_policy /= np.sum(endsquare_policy)
        endsquare_action = int(np.argmax(endsquare_policy))

        state = game.get_next_state(state, startsquare_action, endsquare_action, 1)

        state = game.change_perspective(state, -1)
        print(state)
        
        if game.get_value_and_terminated(state, startsquare_action, endsquare_action, 0, -1)[1]:
            if game.get_value_and_terminated(state, startsquare_action, endsquare_action, 0, -1)[0] == 1:
                print("You lose")
            else:
                print("tie")
            break

        print(f"{round(time.time()-start_time,2)}s")#print calculation time

def make_move(state, botcolor):
    args = {
        'C': 2,
        'num_searches': 5000,
        'dirichlet_epsilon': 0.1,
        'dirichlet_alpha': 0.3,
        'search': True,
        'temperature': 0,
    }
    game = Chess()
    model_dict = "./python/SigmaZero/models/model_3_Chess.pt"
    model = ResNet(game, 4, 512, device=device, number_of_input_channels=13)
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001)
    #load previously learned values
    model.load_state_dict(torch.load(model_dict, map_location=device))
    model.eval() #playing mode

    if botcolor == 1:
        state = game.change_perspective(state, -1)

    start_time = time.time()

    startsquare_policy, endsquare_policy = MCTS(game, args, model).search(state)
    valid_startsquares = game.get_valid_startsquares(state)

    startsquare_policy *= valid_startsquares
    startsquare_policy /= np.sum(startsquare_policy)
    startsquare_action = int(np.argmax(startsquare_policy))
    y0 = startsquare_action // game.column_count
    x0 = startsquare_action % game.column_count
    
    valid_endsquares = game.get_valid_endsquares(state, y0, x0)
    endsquare_policy *= valid_endsquares
    endsquare_policy /= np.sum(endsquare_policy)
    endsquare_action = int(np.argmax(endsquare_policy))

    state = game.get_next_state(state, startsquare_action, endsquare_action, 1)

    if botcolor == 1:
        state = game.change_perspective(state, -1)

    print(f"{round(time.time()-start_time,2)}s")#print calculation time

    return state