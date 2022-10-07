import pygame
import os
from pygame.locals import *
import sys

pygame.init()

# Set up the drawing window
x=600
y=600
pawnx = 0
pawny = 0
screen = pygame.display.set_mode([x, y])

boardimg = pygame.image.load("Images/board.png").convert()
blackpawnimg = pygame.image.load("Images/blackpawn.png").convert()
blackpawnimg.set_colorkey((123, 0, 0))

# Run until the user asks to quit
running = True
while running:

    # Did the user click the window close button?
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            pawnx = int(pygame.mouse.get_pos()[0]/75)*75
            pawny = int(pygame.mouse.get_pos()[1]/75)*75
    # Fill the background with white
    screen.blit(boardimg,(0,0))
    screen.blit(blackpawnimg,(pawnx,pawny))

    # Draw a solid blue circle in the center
    #pygame.draw.circle(screen, (200, 100, 255), (250, 250), 75)

    # Flip the display
    pygame.display.flip()

# Done! Time to quit.
pygame.quit()