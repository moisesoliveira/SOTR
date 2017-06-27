import cclient
import pygame
from pygame.locals import * 
import os


class Client:
    #iniializa as variaveis utilizadas no jogo
    def __init__(self):
        pygame.init()
        pygame.font.init()
        self.WIDTH,self. HEIGHT = 800, 600
        self.tela = pygame.display.set_mode((self.WIDTH,self.HEIGHT))
        pygame.display.set_caption('client', 'Spine Runtime')
        self.font = pygame.font.SysFont("calibri",40)
        self.img = pygame.image.load('bola.png').convert_alpha()
        self.bola_w = self.img.get_width()
        self.bola_h = self.img.get_height()
        self.time = pygame.time.Clock()
        self.i = 0
        self.p1x = 0
        self.p2x = 0
        self.bx = 0
        self.by = 0
        self.s1 = 0
        self.s2 = 0

    #recebe as posicoes do servidor
    #e armazena nas variaveis utilizadas pelo sistema
    def recebepos(self):
        self.buff = cclient.pos()
        if self.buff[0]:
            self.i = self.buff[0]
            self.p1x = self.buff[1]
            self.p2x = self.buff[2]
            self.bx = self.buff[3]
            self.by = self.buff[4]
            self.s1 = self.buff[5]
            self.s2 = self.buff[6]


    def draw(self):
        #define a cor de fundo no campo
        self.tela.fill((192,192,192))
        #desenha o player 1
        pygame.draw.rect(self.tela,(255,0,0),Rect((5,self.p1x),(10, self.HEIGHT/4)),0)
        #desenha o player 2
        pygame.draw.rect(self.tela,(0,0,255),Rect((self.WIDTH-15,self.p2x),(10, self.HEIGHT/4)),0)
        #desenha a bola
        self.tela.blit(self.img,(self.bx-(self.bola_w/2.0),self.by-(self.bola_h/2.0)))
        #desenha a linha que divide o campo
        pygame.draw.line(self.tela, (0,0,0), (self.WIDTH/2, 0), (self.WIDTH/2,self.HEIGHT),2)
        #desenha na tela o placar do jogo
        score1 = self.font.render(str(self.s1), True,(0,0,0))
        score2 = self.font.render(str(self.s2), True,(0,0,0))
        self.tela.blit(score1,(self.WIDTH/4,50))
        self.tela.blit(score2,((self.WIDTH/4)*3,50))


    #envia os comandos do teclado para o servidor
    def controls(self):
        stop = 3
        up = 1
        down = 2
        for event in pygame.event.get():
            if event.type == KEYDOWN:
                if event.key == K_UP:
                    cclient.move(up) #up
                elif event.key == K_DOWN:
                    cclient.move(down) #down
            if event.type == KEYUP:
                if event.key == K_UP:
                    cclient.move(stop) #stop
                elif event.key == K_DOWN:
                    cclient.move(stop)
            if event.type == pygame.QUIT:
                #return True
                exit()


    def run(self, porta):
        cclient.start(porta)
        while(1):
            self.controls()
            self.recebepos()
            self.draw()
            pygame.display.flip()
            self.time.tick(60)


porta = 9000
#ip = "127.0.0.1"
c = Client()
c.run(porta)
