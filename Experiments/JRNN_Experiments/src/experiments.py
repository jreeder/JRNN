# Define the experiments to run here.

                #BP experiments This setup definitely works
experiments = [\
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 1'},\
        #Test 1 with No Validation
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 7'},\
        #Not enough hidden layer same number of training pts
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 4'},\
        #Starved Training Data 20 pts
        {'dsfile':'band-task1.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        {'dsfile':'band-task2.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        {'dsfile':'band-task3.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        {'dsfile':'band-task4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        {'dsfile':'band-task1-4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        {'dsfile':'band-task3-6.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 2'},\
        #Same as test 2 but no validation
        {'dsfile':'band-task1.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        {'dsfile':'band-task2.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        {'dsfile':'band-task3.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        {'dsfile':'band-task4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        {'dsfile':'band-task1-4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        {'dsfile':'band-task3-6.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 8'},\
        #Starved Training Data 10 pts
        {'dsfile':'band-task1.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        {'dsfile':'band-task2.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        {'dsfile':'band-task3.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        {'dsfile':'band-task4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        {'dsfile':'band-task1-4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        {'dsfile':'band-task3-6.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 3'},\
        #Same as Test 3 but no validation
        {'dsfile':'band-task1.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        {'dsfile':'band-task2.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        {'dsfile':'band-task3.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        {'dsfile':'band-task4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':4,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        {'dsfile':'band-task1-4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        {'dsfile':'band-task3-6.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':16,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 9'},\
        #Double the hidden layer with Validation
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':32,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':32,'numOut':4,'type':'BP','useVal':'T'\
        ,'numRuns':60,'expFold':'BP Test 5'},\
        #Double the hidden layer with No Validation
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':8,'numOut':1,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':32,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':32,'numOut':4,'type':'BP','useVal':'F'\
        ,'numRuns':60,'expFold':'BP Test 6'},\
        #CC Standard Run
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 1'},\
        #CC Starved Data 20 pts
        {'dsfile':'band-task1.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        {'dsfile':'band-task2.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        {'dsfile':'band-task3.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        {'dsfile':'band-task4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        {'dsfile':'band-task1-4.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        {'dsfile':'band-task3-6.txt','numTrain':20,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 2'},\
        #CC Starved Data 10 pts
        {'dsfile':'band-task1.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        {'dsfile':'band-task2.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        {'dsfile':'band-task3.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        {'dsfile':'band-task4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        {'dsfile':'band-task1-4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        {'dsfile':'band-task3-6.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'T'\
        ,'numRuns':60,'expFold':'CC Test 3'},\
        #CC 50 train no validation
        {'dsfile':'band-task1.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        {'dsfile':'band-task2.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        {'dsfile':'band-task3.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        {'dsfile':'band-task4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        {'dsfile':'band-task1-4.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        {'dsfile':'band-task3-6.txt','numTrain':50,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 4'},\
        #CC 25 train no validation
        {'dsfile':'band-task1.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        {'dsfile':'band-task2.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        {'dsfile':'band-task3.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        {'dsfile':'band-task4.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        {'dsfile':'band-task1-4.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        {'dsfile':'band-task3-6.txt','numTrain':25,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 5'},\
        #CC 10 train no validation
        {'dsfile':'band-task1.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'},\
        {'dsfile':'band-task2.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'},\
        {'dsfile':'band-task3.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'},\
        {'dsfile':'band-task4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':1,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'},\
        {'dsfile':'band-task1-4.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'},\
        {'dsfile':'band-task3-6.txt','numTrain':10,'numVal':100\
        ,'numTest':500,'numIn':2,'numHid':1,'numOut':4,'type':'CC','useVal':'F'\
        ,'numRuns':60,'expFold':'CC Test 6'}]