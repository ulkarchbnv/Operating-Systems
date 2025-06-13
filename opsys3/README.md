## Task
Spring is here, we've done the watering and it's time to get on with the actual gardening tasks. 
Turns out last year's little chainsaw has given up the fight, it's out of the bin, we need to buy a new one.

1. Buyer calls customer service (sends a signal) and sends a question via pipeline to the parent,
which saw is on sale, so price/value what is the best choice? 
The parent screens the question, then both process terminal, 
parent waits for the customer to complete the action.

2. Enter a current discount chainsaw type and price as a command line parameter. 
E.g: .\a.out "Stihl MSA70C" 140000 . The customer receives this parameters via message queu from customer service, 
writes it on the screen and terminals. Then parent also terminal.

3. The customer considers the offer, has a 50% chance of liking it, 
so he writes in a shared memory "Please send me the Stihl MSA70C chainsaw for HUF 140,000"
or "Unfortunately, I couldn't get that much for a saw!". 
Then customer terminal. Customer service writes the buyer's decision on the screen, then he also terminal.

4. Protect the use of shared memory with semaphores.

Create a C language program to do this, thus helping you. 
The solution should run either on the server opsys.inf.elte.hu or on a similar system.