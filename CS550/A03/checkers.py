# Bryant To, Jessica To
# CS550 - AI A03

import AI
import checkerboard
import Human

def Game(red=AI.Strategy, black=Human.Strategy, init=None, maxplies=8, verbose=False):
	game_board = checkerboard.CheckerBoard()
	redplayer = red('r', game_board, maxplies)
	blackplayer = black('b', game_board, maxplies)
	turn_tracker = 1
	players = [redplayer, blackplayer]
	
	while True:
		turn_tracker = turn_tracker % 2 - 1
		
		game_board = players[turn_tracker].play(game_board)[0]
		print "RED MOVED\n", game_board
		
		turn_tracker = turn_tracker % 2 - 1
		
		game_board = players[turn_tracker].play(game_board)[0]
		print "BLACK MOVED\n", game_board
		 
		if game_board.is_terminal()[0]:
			print "GAME OVER\n"
			break
