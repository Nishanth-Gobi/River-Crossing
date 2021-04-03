# River-Crossing
A Synchronisation problem

### Program specifications:

  -> Multi-threading is done using the pthreah librabry (Threads are synchronised)

### Scenario description:
  
  -> Two types of people 'Hackers' and 'Employees' want to cross a river
  
  -> People board the boat, such that situations with 3 employees and 1 hacker, or vice versa, do not occur
  
  -> Only one person can get in the boat at a time
  
  -> People inside the boat will wait for the boat to be filled
  
  -> When the boat is full, one of the people rows the boat
 
  -> Hackers and employees outside the boat will wait until the boat makes a trip to the opposite shore

Source: <a href="https://people.eecs.berkeley.edu/~kubitron/courses/cs162-F06/hand-outs/synch-solutions.html"> Berkley University</a>
  
### Program description:

  INPUT: Number of hackers, number of employees
  OUPUT: A simulation of the hackers and employees crossing the river
  
  -> The program uses multi-threading (one thread - one person)
  
  -> The program validates the user input, checking if it's possible to cross the river for the given input while satisfying all the constraints
  
  -> Each thread then accesses the 'person_arrives' function to simulate the crossing
