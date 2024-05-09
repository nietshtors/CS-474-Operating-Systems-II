# Project 3: Producers and Consumers

## Building

Command line:

* `make` to build. An executable called `pc` will be produced.

VS Code:

* The default build task runs `make`.

## Files

* `pc.c`: The main code
* `eventbuf.h`: Header and helpful info about the event buffer
* `eventbuf.c`: Contains code for the event buffer

## Functions

* `main()`
  * `sem_open_temp()`: Helper function to open a temporary semaphore
  * `producer_run()`: Manages a producer thread
  * `consumer_run()`: Manages a consumer thread