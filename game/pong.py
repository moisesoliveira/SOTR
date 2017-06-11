import pygame
import random
import os
from pygame.locals import *

c = pygame.time.Clock()
pygame.init()
WIDTH = 800 # LARGURA
HEIGHT = 600 # ALTURA
tela = pygame.display.set_mode((WIDTH,HEIGHT))
class Player1:
    def __init__(self):
        self.w = 200
        self.h = 10
        self.y = (HEIGHT - self.w)/2
        self.v = 0.0 #velocidade
    def move(self, direction):
        print direction
        if (direction == "up"):
            self.v = -5.0
        elif (direction == "down"):
            self.v = 5.0
        elif (direction == "stop"):
            self.v = 0
    def desenha(self, tela):
#        x = self.x - self.w/2.0
#        y = self.y - self.h/2.0
        pygame.draw.rect(tela,(255,0,0),Rect((5,self.y),(self.h, self.w)),0)
        print self.y
        #rect(Surface, color, Rect, width=0) -> Rect = 
    def update(self, dt=1):
            self.y = self.y + self.v*dt
            self.y = self.y
            if self.y + self.w > HEIGHT:
                self.y = HEIGHT - self.w
            if self.y < 0:
                self.y = 0

class Player2:
    def __init__(self):#p = player, 1 ou 2
        self.w = 200
        self.h = 10
        self.y = (HEIGHT - self.w)/2
        self.v = 0.0 #velocidade
    def move(self, direction):
        print direction
        if (direction == "up"):
            self.v = -5.0
        elif (direction == "down"):
            self.v = 5.0
        elif (direction == "stop"):
            self.v = 0
    def desenha(self, tela):
#        x = self.x - self.w/2.0
#        y = self.y - self.h/2.0
        pygame.draw.rect(tela,(0,0,255),Rect((WIDTH-self.h - 5,self.y),(self.h, self.w)),0)
        print self.y
        #rect(Surface, color, Rect, width=0) -> Rect = 
    def update(self, dt=1):
            self.y = self.y + self.v*dt
            self.y = self.y
            if self.y + self.w > HEIGHT:
                self.y = HEIGHT - self.w
            if self.y < 0:
                self.y = 0

class Bola:
    def __init__(self, arq, x, y):
        self.img = pygame.image.load(arq).convert_alpha()
        self.w = self.img.get_width()
        self.h = self.img.get_height()
        self.x = x
        self.y = y
        self.vx = 5.0
        self.vy = 3.0
        self.ax = 0.0
        self.ay = 0.0
    def update(self,dt=1.0):
        self.vx = self.vx + self.ax*dt
        self.vy = self.vy + self.ay*dt
        self.x = self.x + self.vx*dt
        self.y = self.y + self.vy*dt
        if self.y + self.h/2.0 >= HEIGHT or self.y - self.h/2<= 0:
#            self.y = HEIGHT - self.h/2.0
            self.vy = -self.vy
        if self.x + self.w/2.0 >= WIDTH or self.x - self.h/2<= 0:
#            self.x = WIDTH - self.w/2.0
            self.vx = -self.vx
#        print self.x, self.y
    def desenha(self, tela):
        x = self.x - self.w/2.0
        y = self.y - self.h/2.0
        tela.blit(self.img,(x,y))

bola = Bola('bola.png',random.randint(0,800),random.randint(0,600))
player1 = Player1()
player2 = Player2()


sair = False
while not sair:
    os.system('clear')
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sair = True

        if event.type == KEYDOWN:
            if event.key == K_UP:
                player1.move("up")
            elif event.key == K_DOWN:
                player1.move("down")
        elif event.type == KEYUP:
            if event.key == K_UP:
                player1.move("stop")
            elif event.key == K_DOWN:
                player1.move("stop")
                
        if event.type == KEYDOWN:
            if event.key == K_UP:
                player2.move("up")
            elif event.key == K_DOWN:
                player2.move("down")
        elif event.type == KEYUP:
            if event.key == K_UP:
                player2.move("stop")
            elif event.key == K_DOWN:
                player2.move("stop")
                
    tela.fill((192,192,192))
    bola.update()
    bola.desenha(tela)
    player1.desenha(tela)
    player2.desenha(tela)
    player1.update()
    player2.update()
    pygame.display.flip()
    c.tick(30)

