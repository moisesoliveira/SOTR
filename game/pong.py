import pygame
import random

c = pygame.time.Clock()
pygame.init()
WIDTH = 800
HEIGHT = 600
tela = pygame.display.set_mode((WIDTH,HEIGHT))

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
        print self.x, self.y
    def desenha(self, tela):
        x = self.x - self.w/2.0
        y = self.y - self.h/2.0
        tela.blit(self.img,(x,y))

bola = Bola('bola.png',random.randint(0,800),random.randint(0,600))
#for i in range(50):
#bolas.append(Bola('bola.png',random.randint(0,800),random.randint(0,600)))

sair = False
while not sair:
    for evento in pygame.event.get():
        if evento.type == pygame.QUIT:
            sair = True
        elif evento.type == pygame.KEYUP:
            if evento.key == pygame.K_UP:
                pass
            elif evento.key == pygame.K_DOWN:
                pass
    tela.fill((255,255,255))
    bola.update()
    bola.desenha(tela)
    pygame.display.flip()
    c.tick(30)

