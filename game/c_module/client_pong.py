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
        self.i = 0
        self.p1x = 0
        self.p2x = 0
        self.bx = 0
        self.by = 0
        self.s1 = '0'
        self.s2 = '0'
            
    def recebepos(self):
        buff = cclient.pos()
        if buff[0]>0:
            print buff
            self.i = buff[0]
            self.p1x = buff[1]
            self.p2x = buff[2]
            self.bx = buff[3]
            self.by = buff[4]
            self.s1 = buff[5]
            self.s2 = buff[6]


    def draw(self):
        self.tela.fill((192,192,192))
        pygame.draw.rect(self.tela,(255,0,0),Rect((5,self.p1x),(10, self.HEIGHT/4)),0)
        pygame.draw.rect(self.tela,(0,0,255),Rect((self.WIDTH-15,self.p2x),(10, self.HEIGHT/4)),0)
        self.tela.blit(self.img,(self.bx-(self.bola_w/2.0),self.by-(self.bola_h/2.0)))


    def run(self):
        cclient.start()
        pygame.init()
        while(1):
            self.recebepos()
            self.recebepos()
            self.draw()
            pygame.display.flip()
            self.time.tick(90)

c = Client()
c.run()
