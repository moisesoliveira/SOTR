import cclient
import pygame
from pygame.locals import * 
import os


class Client:
    def __init__(self):
        cclient.start()
        self.WIDTH,self. HEIGHT = 800, 600
        self.tela = pygame.display.set_mode((self.WIDTH,self.HEIGHT))
        self.font = pygame.font.SysFont("calibri",40)
        self.img = pygame.image.load('bola.png').convert_alpha()
        self.bola_w = self.img.get_width()
        self.bola_h = self.img.get_height()
        self.time = pygame.time.Clock()
    def recebepos(self):
        buff = cclient.pos()
        if len(buff)>0:
            i = int(buff[0])
            p1x = int(buff[1:4])
            p2x = int(buff[4:7])
            bx = int(buff[7:10])
            by = int(buff[10:13])
            s1 = int(buff[13:16])
            s2 = int(buff[16:19])

    #        os.system('clear')
    #        print i
    #        print p1x
    #        print p1x
    #        print bx
    #        print by
    #        print s1
    #        print s2
            return (i, p1x, p2x, bx, by, s1, s2)


    def draw(self, (i, p1x, p2x, bx, by, s1, s2)):
        self.tela.fill((192,192,192))
        pygame.draw.rect(self.tela,(255,0,0),Rect((5,self.p1x),(10, self.HEIGHT/4)),0)
        pygame.draw.rect(self.tela,(0,0,255),Rect((self.WIDTH-15,self.p2x),(10, self.HEIGHT/4)),0)
        self.tela.blit(self.img,(bx-(bola_w/2.0),by-(bola_h/2.0)))


    def run(self):
        pygame.init()
        while(1):
            draw(recebepos())
            pygame.display.flip()
            self.time.tick(60)
            
c = Client()
c.run()
