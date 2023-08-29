import numpy as np
import math
import torch

#input could be 2*6*64==768 neurons for each type of piece
#this helps recognize the chessboard as if it was an image
#using neural network stucture suitable for image recognition
#output could maybe be 2*64==128 neurons where the first 64 neurons 
# represent the starting square and the last 64 neurons represent the square where we want to move
#could also include a value head