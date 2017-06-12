import pygame
import random
import os
from pygame.locals import *

c = pygame.time.Clock()
pygame.init()
WIDTH = 800 # LARGURA
HEIGHT = 600 # ALTURA
tela = pygame.display.set_mode((WIDTH,HEIGHT))
font = pygame.font.SysFont("calibri",40)
class Player:
    def __init__(self, p):
        self.score = 0.0
        self.w = HEIGHT/3
        self.h = 10
        self.y = (HEIGHT - self.w)/2
        self.v = 0.0 #velocidade
        self.p = p
    def move(self, direction):
        if (direction == "up"):
            self.v = -5.0
        elif (direction == "down"):
            self.v = 5.0
        elif (direction == "stop"):
            self.v = 0
    def desenha(self, tela):
#        x = self.x - self.w/2.0
#        y = self.y - self.h/2.0
        if self.p == 1:
            pygame.draw.rect(tela,(255,0,0),Rect((5,self.y),(self.h, self.w)),0)
        else:
            pygame.draw.rect(tela,(0,0,255),Rect((WIDTH-self.h - 5,self.y),(self.h, self.w)),0)
        #rect(Surface, color, Rect, width=0) -> Rect = 
    def update(self, dt=1):
            self.y = self.y + self.v*dt
            self.y = self.y
            if self.y + self.w > HEIGHT:
                self.y = HEIGHT - self.w
            if self.y < 0:
                self.y = 0

class Bola:
    def __init__(self, arq, vx, vy):
        self.img = pygame.image.load(arq).convert_alpha()
        self.w = self.img.get_width()
        self.h = self.img.get_height()
        self.x = WIDTH/2
        self.y = HEIGHT/2
        self.vx = vx
        self.vy = vy
        self.ax = 0.0
        self.ay = 0.0
    def update(self,dt=1.0):
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

def controls(player1, player2):
        if event.type == KEYDOWN:
            if event.key == K_w:
                player1.move("up")
            elif event.key == K_s:
                player1.move("down")
        elif event.type == KEYUP:
            if event.key == K_w:
                player1.move("stop")
            elif event.key == K_s:
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

def field(tela, player1, player2):
    score1 = font.render(str(int(player1.score)), True,(0,0,0))
    score2 = font.render(str(int(player2.score)), True,(0,0,0))
    pos1 = font.render(str(int(player1.y)), True,(0,0,0))
    pos2 = font.render(str(int(player2.y)), True,(0,0,0))
    pygame.draw.line(tela, (0,0,0), (WIDTH/2, 0), (WIDTH/2,WIDTH),2)
    tela.blit(score1,(300,50))
    tela.blit(score2,(500,50))
    tela.blit(pos1,(0,550))
    tela.blit(pos2,(700,550))
def rules(player1, player2, bola):
#    print player1.y
#    pygame.draw.line(tela, (0,0,255), (bola.x, bola.y), (WIDTH-5 ,player2.y),2)
#    pygame.draw.line(tela, (0,0,255), (bola.x, bola.y), (WIDTH-5 ,player2.y + player2.w),2)
    if bola.x < 50:
        bola.vx = -bola.vx
        if not ((bola.y > player1.y) and (bola.y < player1.y + player1.w)):
            player2.score = player2.score + 1
            bola.x = WIDTH/2
            print "PONTOOO!!"

    if bola.x > WIDTH - 50:
        bola.vx = -bola.vx
        if not ((bola.y > player2.y) and (bola.y < player2.y + player2.w)):
            player1.score = player1.score + 1
            bola.x = WIDTH/2
            print "PONTOOO!!"


vmax = 10
vmin = 3
rand = random.randint(vmin,vmax)
bola = Bola('bola.png',rand,vmax-rand)

player1 = Player(1)
player2 = Player(2)

sair = False
while not sair:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sair = True
    controls(player1, player2)
    tela.fill((192,192,192))
    bola.update()
    bola.desenha(tela)
    player1.desenha(tela)
    player2.desenha(tela)
    field(tela, player1, player2)
    player1.update()
    player2.update()
    rules(player1, player2, bola)
    pygame.display.flip()
    c.tick(60)
