#include <pthread.h> // Threading utilities
#include <stdio.h> // Standard IO utilities
#include <stdlib.h> // All things standard

/*
    Explanation: this program will generate some
    number of threads, compute the square of their
    index (which is taken from a shared variable) and
    add it to a global tally. Finally, the global
    tally will be printed on the screen.

    As usual, because this is a trivial toy example, 
    there will be no input parsing. It would not be
    hard to implement by any means, but it would
    detract slightly from the key take home points. 
*/
// Macro definitions start here
#define NTHREADS 15
// We can pass a parameter into a macro
#define square(a) a*a

unsigned int tally; 	// Total tally
unsigned int next_ID;
pthread_mutex_t mtally, mID;	// Global mutex for the tally

/*
  Function to compute the square of the input and add it 
  to the tally. We will use a local "thread ID" defined as
  the iterator+1 during the generation process (we will 
  standardize to initializing iteration at 0 to comply
  with common practices). Assume the ID is provided in a
  perfected format, only requiring casting.
*/
void* work_on_tally(){
  // Safely obtain thread ID
  pthread_mutex_lock(&mID);
  int ID = next_ID;
  next_ID++;  // While we are at it, update the ID for the next
			  // thread to claim.
  pthread_mutex_unlock(&mID); // We are done, free the access to ID
  
  int squared = square(ID); // This will expand our macro
  printf("%2d^2 = %2d\n", ID, square(ID));
  // CAUTION: accessing global variable here! this is shared!
  // Note that it could also be a passed-in argument pointing to
  // some potentially shared memory location
  pthread_mutex_lock(&mtally);
  // We will only get here once we are given temporary ownership
  // of the mutex: we are in the clear to perform changes now
  tally += squared; // Update the running tally with our squared value
  // We are done. Now we have to free the mutex for some other thread
  // to use. If we don't do this, we WILL create a deadlock situation!
  pthread_mutex_unlock(&mtally);
  
  // Now that we are done, we can return fully
  return NULL;
}

int main(){
  /*
    Let's detail a game plan, in steps, before we start going crazy with code.
    In order:
    1. Initialize our mutexes.
    2. Allocate any buffers we may need. We will do so statically this time
    3. Create NTHREADS with appropriate IDs (using a for loop).
    4. Wait for threads to be done.
    5. Free mutexes.
    6. Now we can present the result (which we will have saved to a local variable).
  */
  next_ID = 1; // Initialize IDs to 1
  // Step 1: mutex
  pthread_mutex_init(&mtally, NULL); // Notice we have to pass the *reference* to the mutex
  pthread_mutex_init(&mID, NULL); // Notice we have to pass the *reference* to the mutex
  
  // Step 2: allocate memory
  // We will need at least enough memory to hold all (actual) thread IDs, so let's
  // begin with that
  pthread_t threads[NTHREADS]; // This would demand a malloc if we don't know the size ahead of time
  
  // We also need a long integer buffer to store thread exit codes in. We don't care for them, but it is
  // needed, so we have to allocate it. This could be a larger memory allocation if we needed to store all results
  long results[1]; // Since we are ignoring the results, allocate a single long worth of memory
  
  // Step 3: create threads. This is a simple for loop, though we need to do some pointer
  // manipulation.
  for(int i=0; i<NTHREADS; i++){
     // Now we can create the new thread. Let's break it down.
     pthread_create(  // We need for arguments: thread buffer, thread properties, task (callable function), and task arguments
     	&threads[i], // We don't want to overwrite thread information, so we need to grab the pointer to each index
     	NULL,        // Thankfully, we don't need special properties this time around
     	&work_on_tally, // We need to pass in the *pointer* to the function which implements our task
     	NULL	     // Pointer to the arguments (in this case, nothing)
     );
     // Pay close attention to how every argument is a pointer. This is important: threads share memory, and
     // so can be coordinated using that shared RAM. This is also way we need access-control mechanisms
     // to prevent race conditions (e.g., incosistent states when performing operations in parallel).
  }
  
  // Now we have a track of all threads and can wait for all of them. Time for another for loop
  // Step 4: Waiting
  for(int i=0; i<NTHREADS; i++){
    // Normally, we would define our results buffer right around here, or just outside this loop,
    // then free it as soon as we are done.
    pthread_join(threads[i],	 // We will wait for each thread in order; if they finish early, it's no big deal 
    		(void**)&results);// We need to pass a pointer to a pointer, untyped. that's void**
  }
  
  // At this point, we can destroy any memory allocations we might have made using malloc
  // We avoided it this time, so we are perfectly safe
  
  // Step 5: free the mutexes fully
  pthread_mutex_destroy(&mtally);
  pthread_mutex_destroy(&mID);
  
  // Step 6: show results
  printf("Total tally: %d\n", tally);
  return 0;
}
