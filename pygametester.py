import pygame
import os
from pygame.locals import *
import sys
import time

pygame.init()
pygame.display.set_caption('LGG Chessbot')

firstmenu = True

def piecemove(n, x0, y0, x1, y1):
    if not (x0 == x1 and y0 == y1):
        return True
    return False

def movepieceto(n, x0, y0, x1, y1):
    board[x0][y0] = n
    board[x1][y1] = 0

menufont = pygame.font.SysFont("Arial", 60)

# Set up the drawing window
x=600
y=600
click = 0
pselectx = -1
pselecty = -1
board = [[30,10,20,50,40,21,11,31], [1,2,3,4,5,6,7,8],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],[0,0,0,0,0,0,0,0],
         [0,0,0,0,0,0,0,0],[-1,-2,-3,-4,-5,-6,-7,-8],[-30,-10,-20,-50,-40,-21,-11,-31]]
screen = pygame.display.set_mode([x, y])

boardimg = pygame.image.load("Images/board.png").convert()
bpawnimg = pygame.image.load("Images/blackpawn.png").convert()
wpawnimg = pygame.image.load("Images/whitepawn.png").convert()
wknightimg = pygame.image.load("Images/whiteknight.png").convert()
wbishopimg = pygame.image.load("Images/whitebishop.png").convert()
wrookimg = pygame.image.load("Images/whiterook.png").convert()
wqueenimg = pygame.image.load("Images/whitequeen.png").convert()
wkingimg = pygame.image.load("Images/whiteking.png").convert()
bknightimg = pygame.image.load("Images/blackknight.png").convert()
bbishopimg = pygame.image.load("Images/blackbishop.png").convert()
brookimg = pygame.image.load("Images/blackrook.png").convert()
bqueenimg = pygame.image.load("Images/blackqueen.png").convert()
bkingimg = pygame.image.load("Images/blackking.png").convert()

bpawnimg.set_colorkey((123, 0, 0))
wpawnimg.set_colorkey((123, 0, 0))
wknightimg.set_colorkey((123, 0, 0))
wbishopimg.set_colorkey((123, 0, 0))
wrookimg.set_colorkey((123, 0, 0))
wqueenimg.set_colorkey((123, 0, 0))
wkingimg.set_colorkey((123, 0, 0))
bknightimg.set_colorkey((123, 0, 0))
bbishopimg.set_colorkey((123, 0, 0))
brookimg.set_colorkey((123, 0, 0))
bqueenimg.set_colorkey((123, 0, 0))
bkingimg.set_colorkey((123, 0, 0))

# Draws pieces to their coordinates

def drawpieces():
    for i in range(8):
        for ii in range(8):
            if board[i][ii] > 0 and board[i][ii] < 10:
                screen.blit(wpawnimg, (ii*75, i*75))
            elif board[i][ii] < 0 and board[i][ii] > -10:
                screen.blit(bpawnimg, (ii*75, i*75))
            elif board[i][ii] > 9 and board[i][ii] < 20:
                screen.blit(wknightimg, (ii*75, i*75))
            elif board[i][ii] > 19 and board[i][ii] < 30:
                screen.blit(wbishopimg, (ii*75, i*75))
            elif board[i][ii] > 29 and board[i][ii] < 40:
                screen.blit(wrookimg, (ii*75, i*75))
            elif board[i][ii] > 39 and board[i][ii] < 50:
                screen.blit(wqueenimg, (ii*75, i*75))
            elif board[i][ii] > 49:
                screen.blit(wkingimg, (ii*75, i*75))
            elif board[i][ii] < -9 and board[i][ii] > -20:
                screen.blit(bknightimg, (ii*75, i*75))
            elif board[i][ii] < -19 and board[i][ii] > -30:
                screen.blit(bbishopimg, (ii*75, i*75))
            elif board[i][ii] < -29 and board[i][ii] > -40:
                screen.blit(brookimg, (ii*75, i*75))
            elif board[i][ii] < -39 and board[i][ii] > -50:
                screen.blit(bqueenimg, (ii*75, i*75))
            elif board[i][ii] < -49:
                screen.blit(bkingimg, (ii*75, i*75))

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
            if click == 1:
                if piecemove(board[pselectx][pselecty], int(pygame.mouse.get_pos()[1]/75), int(pygame.mouse.get_pos()[0]/75), pselectx, pselecty):
                    movepieceto(board[pselectx][pselecty], int(pygame.mouse.get_pos()[1]/75), int(pygame.mouse.get_pos()[0]/75), pselectx, pselecty)
            if click == 0:
                pselectx = int(pygame.mouse.get_pos()[1]/75)
                pselecty = int(pygame.mouse.get_pos()[0]/75)
                if board[pselectx][pselecty] != 0:
                    click = 1
            else:
                click = 0
    # Drawing the board
    screen.blit(boardimg,(0,0))
    drawpieces()

    # Flip the display
    pygame.display.flip()

# Done! Time to quit.
pygame.quit()