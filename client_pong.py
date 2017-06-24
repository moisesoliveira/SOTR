import cclient
import pygame
from pygame.locals import * 
import os


class Client:
    def __init__(self):
        self.WIDTH,self. HEIGHT = 800, 600
        self.tela = pygame.display.set_mode((self.WIDTH,self.HEIGHT))
        #self.font = pygame.font.SysFont("calibri",40)
        self.img = pygame.image.load('bola.png').convert_alpha()
        self.bola_w = self.img.get_width()
        self.bola_h = self.img.get_height()
        self.time = pygame.time.Clock()
#        self.buff = cclient.pos()
        self.i = 0
        self.p1x = 0
        self.p2x = 0
        self.bx = 0
        self.by = 0
        self.s1 = 0
        self.s2 = 0
            
    def recebepos(self):
        self.buff = cclient.pos()
#        print self.buff
        if self.buff[0]:
            self.i = self.buff[0]
            self.p1x = self.buff[1]
            self.p2x = self.buff[2]
            self.bx = self.buff[3]
            self.by = self.buff[4]
            self.s1 = self.buff[5]
            self.s2 = self.buff[6]


    def draw(self):
        self.tela.fill((192,192,192))
        pygame.draw.rect(self.tela,(255,0,0),Rect((5,self.p1x),(10, self.HEIGHT/4)),0)
        pygame.draw.rect(self.tela,(0,0,255),Rect((self.WIDTH-15,self.p2x),(10, self.HEIGHT/4)),0)
        self.tela.blit(self.img,(self.bx-(self.bola_w/2.0),self.by-(self.bola_h/2.0)))


    def run(self, porta):
        pygame.init()
        cclient.start(porta)
        while(1):
            self.recebepos()
            self.draw()
            pygame.display.flip()
            self.time.tick(60)
porta = 9001
#ip = "127.0.0.1"
c = Client()
c.run(porta)
