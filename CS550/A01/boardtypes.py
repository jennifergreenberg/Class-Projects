from basicsearch_lib.board import Board
import math
import random
import copy

class TileBoard(Board):

    def __init__(self, size):
        """Initializes a blank board according to the user's input size and
        populates the board with the appropriate values"""

        # Check for valid input size
        if (math.sqrt(size+1)%1) == 0:
            self.column = int(math.sqrt(size+1))
        else:
            raise ValueError("%s is an invalid board size" % size)
        self.row = self.column
        Board.__init__(self, self.column, self.row)

        # Generate the list of lists representation of the board
        self.mult_tiles = []
        count = 1
        for i in range(self.column):
            tile = []
            for j in range(self.row):
                if count > size:
                    tile.append(None)
                else:
                    tile.append(count)
                count += 1
                random.shuffle(tile)
            self.mult_tiles.append(tile)
            random.shuffle(self.mult_tiles)

        # Populate the board
        for i in range(self.column):
            for j in range(self.row):
                Board.place(self, i, j, self.mult_tiles[i][j])

        # Grab position of blank tile
        for i in range(self.column):
            for j in range(self.row):
                if self.get(i,j) == None:
                    self.blank_tile = [i,j]

    def __eq__(self, tileboard1, tileboard2):
        """Overload == operator to check if two tile boards are in the same state.
        Searches through each tile of each board until a mismatched value is found"""
        flag = True
        for i in range(self.column):
            for j in range(self.row):
                x = tileboard1.get(i,j)
                y = tileboard2.get(i,j)
                if x != y:
                    flag = False
        return flag

    def state_tuple(self):
        "Flatten the list representation of the board and cast to tuple"
        tiles_list = []
        for self.columns in self.mult_tiles:
            for value in self.columns:
                tiles_list.append(value)
        return tuple(tiles_list)

    def get_actions(self):
        "Returns list of possible blank space moves"
        actions = []
        # Blank not on top so it can move up
        if self.blank_tile[0] != 0:
            actions.extend([[0,1]])
        # Blank not on bottom so it can move down
        if self.blank_tile[0] != (self.column-1):
            actions.extend([[0,-1]])
        # Blank not on left so it can move left
        if self.blank_tile[1] != 0:
            actions.extend([[-1,0]])
        # Blank not on right so it can move right
        if self.blank_tile[1] != (self.column-1):
            actions.extend([[1,0]])
        return (actions)

    def move(self, offset):
        "Return a new TileBoard that represents state after the move is made"
        # Check for valid offset move
        new_blank_coord = [0,0]
        if offset in self.get_actions():
            new_blank_coord[0] = self.blank_tile[0] - offset[1]
            new_blank_coord[1] = self.blank_tile[1] + offset[0]
        else:
            raise ValueError('%s is an invalid move' % offset)

        new_table = copy.deepcopy(self)
        tmp_column = int(self.blank_tile[0])
        tmp_row = int(self.blank_tile[1])
        value = int(self.mult_tiles[new_blank_coord[0]][new_blank_coord[1]])

        # Swap the blank tile with the value at the offset tile
        new_table.place(tmp_column, tmp_row, value)
        new_table.place(new_blank_coord[0], new_blank_coord[1], None)
        return (new_table)

    def solved(self):
        "Return true if puzzle is in its goal state with the blank in the center"
        solution = self.state_tuple()
        length = len(solution)
        tile_value = 1
        for i in range(length):
            if solution[i] == tile_value:
                tile_value += 1
            elif i is (length/2) and solution[i] == None:
                continue
            else:
                return False
        return True        
