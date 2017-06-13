import pong
import hello
jogo = pong.Game()
jogo.run()
while(True):
    hello.add(jogo.player1.y,jogo.player2.y, jogo.bola.x, jogo.bola.y, jogo.player1.score, jogo.player2.score, 1)
