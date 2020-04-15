# RTS (Real-time systems)

Code for the RTS course.

## Priority inversion

Run `make` to show the problem of priority inversion. Note that in the example below, the lower priority thread 1 interrupts the execution of thread 2, because thread 2 is waiting for a resource. Hence the priorities are inverted.

```txt
  0: 0 
  1: 0R
  2: 0R
  3:            2 
  4:            2 
  5:            2 
  6:    1 
  7:                    3 
  8:                    3 
  9:    1 
 10: 0R
 11:            2R
 12:            2 
```

A possible solution is to inherit the priority of a higher priority thread, once it requests the resource. You can run this example with `make inherit`.

```txt
  0: 0 
  1: 0R
  2: 0R
  3:            2 
  4:            2 
  5:            2 
  6: 0R
  7:                    3 
  8:                    3 
  9:            2R
 10:            2 
 11:    1 
 12:    1 
```

Another solution is to give threads with critical resources the highest priority to allow them to finish their work as fast as possible. You can run this example with `make ceiling`.

```txt
  0: 0 
  1: 0R
  2: 0R
  3: 0R
  4:            2 
  5:            2 
  6:            2 
  7:                    3 
  8:                    3 
  9:            2R
 10:            2 
 11:    1 
 12:    1 
```

## License

This project is licensed under the terms of the [MIT license](./LICENSE.md).