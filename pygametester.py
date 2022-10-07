import pygame
import os
from pygame.locals import *
import sys
import time

pygame.init()
pygame.display.set_caption('LGG Chessbot')

firstmenu = True

menufont = pygame.font.SysFont("Arial", 60)

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

    #Menu 
    while firstmenu:
        screen.fill((0,255,255))
        playwhitebtn = menufont.render("White", 1, (100,100,100), (0,255,255))
        whiterect = playwhitebtn.get_rect()
        whiterect.center = (x/2, y/4)

        playblackbtn = menufont.render("Black", 1, (100,100,100), (0,255,255))
        blackrect = playblackbtn.get_rect()
        blackrect.center = (x/2, y/2)

        h2hbtn = menufont.render("Player vs Player", 1, (100,100,100), (0,255,255))
        h2hrect = h2hbtn.get_rect()
        h2hrect.center = (x/2, y/1.33)

        screen.blit(playwhitebtn, whiterect)
        screen.blit(playblackbtn, blackrect)
        screen.blit(h2hbtn, h2hrect)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                firstmenu = False
                running = False
                break
            if event.type == pygame.MOUSEBUTTONDOWN and whiterect.collidepoint(pygame.mouse.get_pos()):
                firstmenu = False
                break
            if event.type == pygame.MOUSEBUTTONDOWN and blackrect.collidepoint(pygame.mouse.get_pos()):
                firstmenu = False
                break
            if event.type == pygame.MOUSEBUTTONDOWN and h2hrect.collidepoint(pygame.mouse.get_pos()):
                firstmenu = False
                break

        pygame.display.flip()

    #Event handlers
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            pawnx = int(pygame.mouse.get_pos()[0]/75)*75
            pawny = int(pygame.mouse.get_pos()[1]/75)*75
    # Drawing the board
    screen.blit(boardimg,(0,0))
    screen.blit(blackpawnimg,(pawnx,pawny))

    # Flip the display
    pygame.display.flip()

# Done! Time to quit.
pygame.quit()