(Q)What are 2 things that you learned from this assignment?
Ans:
(1) Each and every step explained in the book as well as in the examples mentioned in the assignment is very importantand can not be skipped. e.g: in Message passing synchronization, sending OK message to the parent process is very important. or the last step mentioned in the polling exampe in assignment i.i consumer-producer problem, (/* Polling to see if both producer and consumer are done */
while (consume < 3);) is also very important as my code for polling was getting executed successfully but was entering the deadlock state and resulting in terminal hang.
  
(2)Sometimes, creating global variables is more feasible than creating multiple loca variables multiple times. In this assignment, I have useda number of global variables as all functions, processes can access them easily rather than passing each other via arguments.



(Q)What are 2 mistakes that you made in this assignment, and how did you discover and fix these mistakes?
Ans:
(1) Initially, I implemented the code for polling. In this code, I created a loop for totalCount number of times where totalCount=p*r-1 which ultimately created totalCountnumber of processes instead of creating p number of processes. Just adjusting and sending value of process to print it did not avoid creating such long number of processes unnecessarily. I discovered this issue while implementing synchronization via message passing as I was unable to adjust the value of number of processes and rings.

(2) When I was trying to execute the polling, the variable sent to the process using call by reference, I was trying to print it as it is without copying it in a variable and thus it was printing a different value rather than printing the desired value. On trying multiple times, the only option left was copying the value from parameter in a variable as all other options, I had already tried and did not really work.




(Q)If you were restarting this assignment, what would you change about how you went about completing this assignment?
Ans:
If I were restarting this assignment, I would definitely have changed the approach of doing this assignment. I first implemented polling and synchronization in one single file with hardcoded values of number of processes and number of rounds instead of asking user to provide these values. also, the different functions were written in one and the same file. Not as mentioned in the assignment i.e. definitions in include folder, ring processes in app folder, etc. I would have definitely implemented these small things first like writing differnet blocks of code in different fle-folders as mentioned in the question as it would have helped me in saving a lot of time.
