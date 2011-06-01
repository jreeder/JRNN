Round 1
Standard Experiment 2
Round 2
Attempt to add more parts to do some of the experiment 1. 
Round 3 
Corrected issue that overwrote some of the outputs files and filled in missing stl run. 
Round 3 didn't adjust the impoverished amount for the tasks. It is rubbish. 

Round 4 uses a higher impoverished percent for circinsq and linear (0.9) Only contains those two (last round should be fine for band. 

Round 5 Uses values found in the grid search for band linear and circ datasets 
	Band hN: 4 imp:20 Full:60
	cirinsq hN: 6 imp:20 Full:60
	linear hN: 4 imp:10 Full:60
Swapped cirinsq tasks 1 and 2 to 7 and 8. Moved all others up to fill the gap. 

Round 6 same values as Round 5 but uses full of 100. 
	Also turned on max weights for rprop, and changed scale and offset for rProp to .5 and .25 and cc to 1 and .5
	Also using new impoverish function. 
	This test was done using jrnn_exprun.  