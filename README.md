# Loop unrolling
A comparison between loop unrolling and standard copying.

## Duff's device
This C++ program is meant to create Duff's device and compare how fast it is, with the standard copy using the std::transform function in C++. A few words first. 

Duff's device is a way of manually implementing loop unrolling by interleaving the do-while loop and a switch statement. Its discovery is credited to Tom Duff, when Duff was working for Lucasfilm and used it to speed up a real-time animation program. 

Loop unrolling attempts to reduce the overhead of conditional branching needed to check whether a loop is done, by executing a batch of loop bodies per iteration. To handle cases where the number of iterations is not divisible by the unrolled-loop increments, he used a common technique among assembly language programmers. This technique is to jump directly into the middle of the unrolled loop body to handle the remainder. Duff implemented this technique in C by using C's case label fall-through feature to jump into the unrolled body. The initial problem was to copy 16-bit unsigned integers from an array into a memory-mapped output register. 

The basic idea of loop unrolling is that the number of instructions executed in a loop can be reduced by reducing the number of loop tests, sometimes reducing the amount of time spent in the loop. For example, in the case of a loop with only a single instruction in the block code, the loop test will typically be performed for every iteration of the loop, that is every time the instruction is executed. If, eight copies of the same instruction are placed in the loop, then the test will be performed only every eight iterations, and this may gain time by avoiding seven tests. However, this only handles a multiple of eight iterations, requiring something else to handle any remainder of iterations.

Duff's device provides a solution for this. First, it performs the remainder of iterations, followed by iterating as many times as necessary the multiple of eight similar instructions. To determine the number of remainder iterations, the code first calculates the total number of iterations modulo eight. According to this remainder, the program execution will then jump to a case statement followed by exactly the number of iterations needed. Once this is done, the code continues by doing iterations of groups of eight instructions.

Duff's device provides a compact loop unrolling by using the case keyword both inside and outside the loop. This is unusual because the contents of a case statement are traditionally thought of as a block of code nested inside the case statement, and a reader would typically expect it to end before the next case statement. According to the specifications of C language though,  case statements can appear anywhere inside the switch code block, and at any depth; the program execution will simply jump to the next statement, wherever it may be.

The comparison between Duff's device and copy is done in the program provided. The time of the execution is measured and saved to extract the time needed for the copy operation to be completed. The copy is done 100 times and the program will display the time it took at the end per technique, i.e. for Duff's device and the copy using the std::transform.

## Comparison Factors:
#### Compiler Optimizations: 
Modern compilers are very good at optimizing loops. When you use std::copy, the compiler can apply a wide range of optimizations that might not be possible with manually written code.

#### Readability and Maintainability:
std::copy is much more readable and maintainable. It's clear what the function is doing, and it's less error-prone.

#### Portability:
std::copy is portable and guarantees the same behavior across different platforms and compilers.

#### Performance:
While Duff's device is a clever optimization for older hardware, modern CPUs and compilers are designed to handle such operations efficiently.

## Results
Once you run the problem, you will see that Duff's device is about three times faster than the standard copy. Keep in mind that the code was written in C++ and not in C as the initial problem and technique appeared.
```
Duff's device time: 0.10484s
std::transform time: 0.341205s
```

## Note
You might notice in the code the following two segments of code:
```
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wimplicit-fallthrough"
#endif
```
and 
```
*to++ = *from++ + 1;
//[[fallthrough]];
case 7:      *to++ = *from++ + 1;
//[[fallthrough]];
case 6:      *to++ = *from++ + 1;
//[[fallthrough]];
case 5:      *to++ = *from++ + 1;
//[[fallthrough]];
case 4:      *to++ = *from++ + 1;
//[[fallthrough]];
case 3:      *to++ = *from++ + 1;
//[[fallthrough]];
case 2:      *to++ = *from++ + 1;
//[[fallthrough]];
```
The two segments have been placed here because once the program is built, it gives out a warning. The warning states **"unannotated fallthrough between switch labels"** and this is because the compiler thinks that you forgot the break clause between the cases, thus a fallthrough will occur. To tell the compiler that you know what you are doing and that this was intentional, you can use the *[[fallthrough]]* attribute to indicate intentional fallthrough (this works for C++17 and above). If your compiler does not support this, you can try the pragma directives as shown above. You will need either one or the other, not both at the same time. So comment out or remove the one you don't need. In case your compiler does not support either, then you can suppress the warning, ignore it, or update your compiler! It's up to you :)

## License
This project is licensed under the MIT license.