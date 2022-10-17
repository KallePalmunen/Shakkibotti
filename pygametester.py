import pygame
from pygame.locals import *
import time
from copy import copy, deepcopy
import Chessbot1
import basicbot
import stockfishfanboy
import math

pygame.init()
pygame.display.set_caption('LGG Chessbot')

firstmenu = True
start = True


menufont = pygame.font.SysFont("Arial", 60)

# Set up the drawing window
x=600
y=600
click = 0
pselectx = -1
pselecty = -1
#botlevel 0 == randommover, 1 == basicbot, 2 == stockfishfanboy
botlevel = 1
promoteto = 0

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
            if Chessbot1.board[i][ii] > 0 and Chessbot1.board[i][ii] < 10:
                screen.blit(wpawnimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < 0 and Chessbot1.board[i][ii] > -10:
                screen.blit(bpawnimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] > 9 and Chessbot1.board[i][ii] < 20:
                screen.blit(wknightimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] > 19 and Chessbot1.board[i][ii] < 30:
                screen.blit(wbishopimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] > 29 and Chessbot1.board[i][ii] < 40:
                screen.blit(wrookimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] > 39 and Chessbot1.board[i][ii] < 50:
                screen.blit(wqueenimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] > 49:
                screen.blit(wkingimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < -9 and Chessbot1.board[i][ii] > -20:
                screen.blit(bknightimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < -19 and Chessbot1.board[i][ii] > -30:
                screen.blit(bbishopimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < -29 and Chessbot1.board[i][ii] > -40:
                screen.blit(brookimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < -39 and Chessbot1.board[i][ii] > -50:
                screen.blit(bqueenimg, (ii*75, i*75))
            elif Chessbot1.board[i][ii] < -49:
                screen.blit(bkingimg, (ii*75, i*75))

def promotegui():
    global promoteto
    while True:
        screen.fill((0,255,255))
        queenbtn = menufont.render("Queen", 1, (100,100,100), (0,255,255))
        queenrect = queenbtn.get_rect()
        queenrect.center = (x/2, y/5)

        knightbtn = menufont.render("Knight", 1, (100,100,100), (0,255,255))
        knightrect = knightbtn.get_rect()
        knightrect.center = (x/2, 2*y/5)

        rookbtn = menufont.render("Rook", 1, (100,100,100), (0,255,255))
        rookrect = rookbtn.get_rect()
        rookrect.center = (x/2, 3*y/5)

        bishopbtn = menufont.render("Bishop", 1, (100,100,100), (0,255,255))
        bishoprect = bishopbtn.get_rect()
        bishoprect.center = (x/2, 4*y/5)

        screen.blit(queenbtn, queenrect)
        screen.blit(knightbtn, knightrect)
        screen.blit(rookbtn, rookrect)
        screen.blit(bishopbtn, bishoprect)

        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                Chessbot1.promotemenu = False
                return
            if event.type == pygame.MOUSEBUTTONDOWN and queenrect.collidepoint(pygame.mouse.get_pos()):
                promoteto=4
                Chessbot1.promotemenu = False
                return
            if event.type == pygame.MOUSEBUTTONDOWN and knightrect.collidepoint(pygame.mouse.get_pos()):
                promoteto=1
                Chessbot1.promotemenu = False
                return
            if event.type == pygame.MOUSEBUTTONDOWN and rookrect.collidepoint(pygame.mouse.get_pos()):
                promoteto=3
                Chessbot1.promotemenu = False
                return
            if event.type == pygame.MOUSEBUTTONDOWN and bishoprect.collidepoint(pygame.mouse.get_pos()):
                promoteto=2
                Chessbot1.promotemenu = False
                return

        pygame.display.flip()


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
                Chessbot1.bot=1
                firstmenu = False
                break
            if event.type == pygame.MOUSEBUTTONDOWN and blackrect.collidepoint(pygame.mouse.get_pos()):
                Chessbot1.bot=0
                firstmenu = False
                break
            if event.type == pygame.MOUSEBUTTONDOWN and h2hrect.collidepoint(pygame.mouse.get_pos()):
                Chessbot1.bot=2
                firstmenu = False
                break

        pygame.display.flip()
    
    if start:
        screen.blit(boardimg,(0,0))
        drawpieces()
        pygame.display.flip()
        time.sleep(1)
        start = False

    if Chessbot1.turn >= 0:
        if Chessbot1.turn == Chessbot1.bot:
            if botlevel == 1 and Chessbot1.bot == 0:
                basicbot.basicbot()
            elif botlevel == 2 and Chessbot1.bot == 0:
                stockfishfanboy.move()
            else:
                Chessbot1.randommove()
        Chessbot1.gameend()
            

    #Event handlers
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.MOUSEBUTTONDOWN:
            if click == 1 and Chessbot1.turn != Chessbot1.bot:
                x1=int(pygame.mouse.get_pos()[1]/75)
                y1=int(pygame.mouse.get_pos()[0]/75)
                n=Chessbot1.board[pselectx][pselecty]
                if (Chessbot1.board[pselectx][pselecty] > 0 and Chessbot1.turn == 0) or (Chessbot1.board[pselectx][pselecty] < 0 and Chessbot1.turn == 1):
                    if Chessbot1.piecemove(Chessbot1.board[pselectx][pselecty], pselectx, pselecty, x1,
                        y1) and not Chessbot1.pin(Chessbot1.board[pselectx][pselecty], pselectx, pselecty,
                        x1, y1):
                        Chessbot1.movepieceto(n, pselectx, pselecty, x1, y1)
                        if Chessbot1.promotemenu:
                            promotegui()
                            Chessbot1.board[x1][y1] = int(math.copysign(1, n))\
                                *(promoteto*10+Chessbot1.pieces[promoteto][(n < 0)])
                            Chessbot1.pieces[promoteto][(n < 0)] += 1
                        if Chessbot1.turn == 0:
                            Chessbot1.turn = 1
                        else:
                            Chessbot1.turn = 0
                        Chessbot1.gameend()
            if click == 0:
                pselectx = int(pygame.mouse.get_pos()[1]/75)
                pselecty = int(pygame.mouse.get_pos()[0]/75)
                if (Chessbot1.board[pselectx][pselecty] > 0 and Chessbot1.turn == 0) or (Chessbot1.board[pselectx][pselecty] < 0 and Chessbot1.turn == 1):
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