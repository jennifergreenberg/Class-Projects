from boardtypes import TileBoard

print "TILEBOARD ORIGINAL"
x = TileBoard(8)
print x
print "TUPLE FOR TABLE:"
print x.state_tuple()
print "POSSIBLE ACTIONS: %s" % (x.get_actions())
print "TILEBOARD AFTER MOVE UP"
print x.move([0,1])
print "TILEBOARD AFTER MOVE DOWN"
print x.move([0,-1])
print "TILEBOARD AFTER MOVE LEFT"
print x.move([-1,0])
print "TILEBOARD AFTER MOVE RIGHT"
print x.move([1,0])
print "ORIGINAL TILEBOARD == NEW TILEBOARD: %s" % x.__eq__(x,x.move([1,0]))
print "PUZZLE SOLVED: %s" % x.solved()
