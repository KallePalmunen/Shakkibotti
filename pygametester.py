import pygame
import os

pygame.init()

# Set up the drawing window
x=600
y=600
screen = pygame.display.set_mode([x, y])

boardimg = pygame.image.load("Images/board.png").convert()

# Run until the user asks to quit
running = True
while running:

    # Did the user click the window close button?
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    # Fill the background with white
    screen.blit(boardimg,(0,0))

    # Draw a solid blue circle in the center
    #pygame.draw.circle(screen, (200, 100, 255), (250, 250), 75)

    # Flip the display
    pygame.display.flip()

# Done! Time to quit.
pygame.quit()