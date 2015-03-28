# Bryant To, Jessica To
# CS550 - AI A03

import AbstractStrategy

class Strategy(AbstractStrategy.Strategy):

	num_inf = float('inf')
	poss_action = []
	depth = 0
	initial_count = True

	def utility(self, board):
		"""Returns utility value based on number of pieces on board, player distance to
		kings row, and number of jumps"""
		value = self.eval_pieces(board)
		value += self.eval_distance(board)
		value += self.eval_jumps(board) 
		
		return value

	def eval_jumps(self,board):
		"""Calculates the number of jumps for max/min player by taking the difference
		between the total number of pieces on the previous board and current board"""
		if self.initial_count:
			self.rev_piececounter = self.curr_piececounter = self.num_playerpieces(board)
			print "Entered initial_count"
			self.initial_count = False
		self.prev_piececounter = self.curr_piececounter
		self.curr_piececounter = self.num_playerpieces(board)

		diff_maxplayer = self.prev_piececounter[0] - self.curr_piececounter[0]
		diff_minplayer = self.prev_piececounter[1] - self.curr_piececounter[1]

		# If maxplayer loses less pieces than the minplayer, then it's a good jump
		if (diff_maxplayer < diff_minplayer):
			# Return how many minplayer pieces were jumped
			return diff_minplayer
		else:
			# Returns how many maxplayer pieces are lost (should be negative value) 
			return diff_minplayer - diff_maxplayer
	
	def eval_pieces(self, board):
		"""Returns a value based on number of pawns (that have a value of 1),
		and kings (that have a value of 1.4). This number reflects the value respective
		to the current player."""
		curr_player = board.playeridx(self.maxplayer)
		other_player = board.playeridx(self.minplayer)
		pawn_value = board.get_pawnsN()[curr_player] - board.get_pawnsN()[other_player]
		king_value = board.get_kingsN()[curr_player] - board.get_kingsN()[other_player]
		return pawn_value + (1.4*king_value)

	def num_playerpieces(self,board):
		"""Returns the total number of pawns and kings on the given board. Used to calculate
		the number of jumps on the board."""
		curr_player = board.playeridx(self.maxplayer)
		other_player = board.playeridx(self.minplayer)
		num_pawns = board.get_pawnsN()
		num_kings = board.get_kingsN()
		return [num_pawns[curr_player] + num_kings[curr_player] , num_pawns[other_player] + num_kings[other_player]]
	
	def eval_distance(self,board):
		"""Looks at the pieces of the current player and sums the distances of each 
		piece to kings row. Allows us to evaluate the overall progress of pieces."""
		value = 0
		curr_player = board.playeridx(self.maxplayer)
		
		for row,col,piece in board:
			(player,isking) = board.identifypiece(piece)
			if not isking:
				if player is curr_player:
					value += board.disttoking(piece,row)
				else:
					value -= board.disttoking(piece,row)
			if self.on_edge:
				if player is self.maxplayer:
					value += board.edgesize
				else:
					value -= board.edgesize
		return value
	
	def on_edge(self,board,row,col):
		"Checks to see if the given row and column are on the edge of the board."
		if row is 0 or row is (board.edgesize - 1) or col is 0 or col is (board.edgesize - 1):
			return True
		return False

	def play(self, board):
		"""Returns (newboard, action) where newboard is the result of having 
		applied action to board and action is determined via a game tree search 
		(e.g. minimax with alpha-beta pruning)."""
		action = self.alphaBeta(board)
		
		# Reset the depth after each play so that alpha beta will run through the 
		# correct number of designated maxplies
		self.depth = 0
		return (board.move(action), action)

	def alphaBeta(self, board):
		"Apply alpha-beta search algorithm on the given board."
		best_value = self.maxValue(state=board, alpha=(-self.num_inf), beta=self.num_inf)
		
		# Takes the possible actions of this board and compares each utility of the board to
		# the value returned from maxValue to determine which is the best action that correlates
		# with the given best value.
		actions = board.get_actions(self.maxplayer)
		for action in actions:
			value = self.utility(board.move(action))
			if abs(best_value - value) < 1:
				return action

	def maxValue(self, state, alpha, beta):
		"""Finds max value and returns a value obtained from calling the utility function."""
		terminal = state.is_terminal()
		util_value = 0

		# Checks if the current state is a terminal node
		if terminal[0] is True:
			value = self.utility(state)
		
		# If not a terminal node, proceed to grab values from the children nodes of this state
		else:
			value = (-self.num_inf)
			actions = state.get_actions(self.maxplayer)
			
			# Return if this search has already reached the designated maxplies
			if self.depth > self.maxplies:
				return value
			self.depth += 2
			
			for action in actions:
				value = max(value, self.minValue(state=state.move(action), alpha=alpha, beta=beta))
				# Store the utility value of the state since value will constantly grab infinity after first run
				util_value = self.utility(state.move(action))

				if util_value >= beta:
					break
				else:
					alpha = max(alpha, util_value)
		return util_value

	def minValue(self, state, alpha, beta):
		"Finds the min value and returns a value obtained from calling the utility function."
		terminal = state.is_terminal()

		# Checks if the current state is a terminal node
		if terminal[0] is True:
			value = self.utility(state)
		
		# If not a terminal node, proceed to grab values from the children nodes of this state
		else:
			value = self.num_inf
			actions = state.get_actions(self.minplayer)
			for action in actions:
				
				# Return if this search has already reached the designated maxplies
				if self.depth > self.maxplies:
					return value

				value = min(value, self.maxValue(state=state.move(action), alpha=alpha, beta=beta))
				# Store the utility value of the state since value will constantly grab -infinity after first run
				util_value = self.utility(state.move(action))
				if util_value <= alpha:
					break
				else:
					beta = min(beta, util_value)
		return util_value
