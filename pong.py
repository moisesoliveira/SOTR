import pygame
import random
import os
import sys
from pygame.locals import *
import cserver
    
class Game:
    def __init__(self, WIDTH = 800, HEIGHT = 600):
        self.time = pygame.time.Clock()
        pygame.init()
        self.WIDTH = WIDTH # LARGURA
        self.HEIGHT = HEIGHT # ALTURA
        self.tela = pygame.display.set_mode((WIDTH,HEIGHT))
        pygame.display.set_caption('server', 'Spine Runtime')
        self.font = pygame.font.SysFont("calibri",40)
        self.player1 = Player(1, self.WIDTH, self.HEIGHT)
        self.player2 = Player(2, self.WIDTH, self.HEIGHT)
        self.bola = Bola('bola.png',5.0,3.0, self.HEIGHT, self.HEIGHT)

    def field(self):
        #desenha o placar
        score1 = self.font.render(str(int(self.player1.score)), True,(0,0,0))
        score2 = self.font.render(str(int(self.player2.score)), True,(0,0,0))
        pos1 = self.font.render(str(int(self.player1.y)), True,(0,0,0))
        pos2 = self.font.render(str(int(self.player2.y)), True,(0,0,0))
        #desena a linha no meio da tela
        pygame.draw.line(self.tela, (0,0,0), (self.WIDTH/2, 0), (self.WIDTH/2,self.HEIGHT),2)
        self.tela.blit(score1,(300,50))
        self.tela.blit(score2,(500,50))
        self.tela.blit(pos1,(0,550))
        self.tela.blit(pos2,(700,550))

    def rules(self):
        if self.bola.x < 50:
            self.bola.vx = -self.bola.vx*1.2

            if not ((self.bola.y > self.player1.y) and (self.bola.y < self.player1.y + self.player1.w)):
                self.player2.score = self.player2.score + 1
                self.bola.x = self.WIDTH/2
                self.bola.vx = (self.bola.vx/abs(self.bola.vx))*5.0


        if self.bola.x > self.WIDTH - 50:
            self.bola.vx = -self.bola.vx*1.2
            if not ((self.bola.y > self.player2.y) and (self.bola.y < self.player2.y + self.player2.w)):
                self.player1.score = self.player1.score + 1
                self.bola.x = self.WIDTH/2
                self.bola.vx = (self.bola.vx/abs(self.bola.vx))*5.0


    def controls(self):
        stop = 3
        up = 1
        down = 2
        if cserver.control(1) == up:
            self.player1.move("up")
        if cserver.control(1) == down:
            self.player1.move("down")
        if cserver.control(1) == stop:
            self.player1.move("stop")

        if cserver.control(2) == up:
            self.player2.move("up")
        if cserver.control(2) == down:
            self.player2.move("down")
        if cserver.control(2) == stop:
            self.player2.move("stop")
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit()


    def run(self,porta = 9000):
        cserver.start(porta)
        while True:
            self.controls()
            self.tela.fill((192,192,192))
            self.field()
            self.bola.desenha(self.tela)
            self.rules()
            self.player1.desenha(self.tela)
            self.player2.desenha(self.tela)
            self.bola.update()
            self.player1.update()
            self.player2.update()
            cserver.add(int(self.player1.p),int(self.player1.y),int(self.player2.y), \
            int(self.bola.x), int(self.bola.y), \
            int(self.player1.score), int(self.player2.score), 1)
            #cserver.add(0,0,0,0,0,0,0, 1)

            pygame.display.flip()
            self.time.tick(60)



class Player:
    def __init__(self, p, WIDTH, HEIGHT):
        self.WIDTH = WIDTH
        self.HEIGHT = HEIGHT
        self.score = 0.0
        self.w = HEIGHT/4
        self.h = 10
        self.y = (HEIGHT - self.w)/2
        self.v = 0.0 #velocidade
        self.p = p
    def move(self, direction):
        if (direction == "up"):
            self.v = -10.0
        elif (direction == "down"):
            self.v = 10.0
        elif (direction == "stop"):
            self.v = 0
    def desenha(self, tela):
#        x = self.x - self.w/2.0
#        y = self.y - self.h/2.0
        if self.p == 1:
            pygame.draw.rect(tela,(255,0,0),Rect((5,self.y),(self.h, self.w)),0)
        else:
            pygame.draw.rect(tela,(0,0,255),Rect((self.WIDTH-self.h - 5,self.y),(self.h, self.w)),0)
        #rect(Surface, color, Rect, width=0) -> Rect = 
    def update(self, dt=1):
            self.y = self.y + self.v*dt
            self.y = self.y
            if self.y + self.w > self.HEIGHT:
                self.y = self.HEIGHT - self.w
            if self.y < 0:
                self.y = 0

    

class Bola:
    def __init__(self, arq, vx, vy, WIDTH, HEIGHT):
        self.WIDTH = WIDTH
        self.HEIGHT = HEIGHT
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
        if self.vx > 70.0:
            self.vx = 70.0

        if self.y + self.h/2.0 >= self.HEIGHT or self.y - self.h/2<= 0: #quando bate em cima ou em baixo
            self.vy = -self.vy

    def desenha(self, tela):
        x = self.x - self.w/2.0
        y = self.y - self.h/2.0
#        self.img = pygame.transform.rotate(self.img, 2)
        tela.blit(self.img,(x,y))


if __name__ == '__main__':
    jogo = Game()
    jogo.run(int(sys.argv[1]))


