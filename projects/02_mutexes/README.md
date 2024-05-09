# Project 2: Mutexes

## Building

Command line:

* `make` to build. An executable called `reservations` will be produced.

VS Code:

* The default build task runs `make`.

## Files

* `reservations.c`: The main code

## Functions

* `main()`
  * `seat_broker()`: function that runs each broker thread
    * `reserve_seat()`: function that attempts to reserve a seat
      * `is_free()`: function returns if a seat is available
    * `free_seat()`: function that attempts to unreserve a seat
      * `is_free()`: function returns if a seat is available
    * `verify_seat_count()`: function that verifies is the number of reserved seats is accurate