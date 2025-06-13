# Easter Bunny King Competition  EXTENDED TASK

## 🐰 Task
Spring is here, and the bunnies are getting ready for the "Easter Bunny King" election. 
The "Chief Bunny" will organise a competition between the bunny boys to see
who can recite the most beautiful watering poem and collect the most red eggs for it from the bunny girls. 
For example: "Red eggs, white bunny, a kiss for watering!"

The bunnies must register for the competition - when you register, 
you must give the bunny's name (e.g. Bunny LongEar) and the poem you want to recite. 
There should be a counter as well, the number of collected eggs (at the time of entry is 0). 
The one who collects the most red eggs from the bunny girls during the watering will be the Easter Bunny King. Data stored in a file.

Come Easter Monday, the "Chief Bunny" sends all the bunny boys (child process) who have signed up to water the bunny girls.
The boys go to the chosen bunny family to water, when they arrive, the child process sends a signal to the "Chief Bunny", 
then they recite the poems in turn, they are written on a screen, and the bunny girls give the boys red eggs for watering. 
If they really like the poem, they give more, if not, less. A bunny boy receives between 1 and 20 eggs, 
depending on his liking index (random number). This is also written on the screen by the child process, 
who send the number of eggs they have received back to the Chief Bunny via a pipeline, and then they finish watering. 
Chief Bunny records the values received on the pipe in the data file. At the same time,
he announces the boy who collects the most eggs as the "Easter Bunny King" and then writes his name 
and the number of eggs received on a screen.

Create a C language program to solve this task, the solution should run either on opsys.inf.elte.hu or on a Linux system similar to it. The solution must be presented to the tutor during the week following the deadline.