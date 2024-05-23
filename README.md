# Loop unrolling
A comparison between loop unrolling and standard copying.

## Duff's device
This C++ program is meant to create Duff's device and compare how fast it is, with the standard copy using the std::transform function, and the memcpy function in C++. A few words first. 

Duff's device is a way of manually implementing loop unrolling by interleaving the do-while loop and a switch statement. Its discovery is credited to Tom Duff, when Duff was working for Lucasfilm and used it to speed up a real-time animation program. 

Loop unrolling attempts to reduce the overhead of conditional branching needed to check whether a loop is done, by executing a batch of loop bodies per iteration. To handle cases where the number of iterations is not divisible by the unrolled-loop increments, he used a common technique among assembly language programmers. This technique is to jump directly into the middle of the unrolled loop body to handle the remainder. Duff implemented this technique in C by using C's case label fall-through feature to jump into the unrolled body. The initial problem was to copy 16-bit unsigned integers from an array into a memory-mapped output register. 

The basic idea of loop unrolling is that the number of instructions executed in a loop can be reduced by reducing the number of loop tests, sometimes reducing the amount of time spent in the loop. For example, in the case of a loop with only a single instruction in the block code, the loop test will typically be performed for every iteration of the loop, that is every time the instruction is executed. If, eight copies of the same instruction are placed in the loop, then the test will be performed only every eight iterations, and this may gain time by avoiding seven tests. However, this only handles a multiple of eight iterations, requiring something else to handle any remainder of iterations.

Duff's device provides a solution for this. First, it performs the remainder of iterations, followed by iterating as many times as necessary the multiple of eight similar instructions. To determine the number of remainder iterations, the code first calculates the total number of iterations modulo eight. According to this remainder, the program execution will then jump to a case statement followed by exactly the number of iterations needed. Once this is done, the code continues by doing iterations of groups of eight instructions.

Duff's device provides a compact loop unrolling by using the case keyword both inside and outside the loop. This is unusual because the contents of a case statement are traditionally thought of as a block of code nested inside the case statement, and a reader would typically expect it to end before the next case statement. According to the specifications of C language though,  case statements can appear anywhere inside the switch code block, and at any depth; the program execution will simply jump to the next statement, wherever it may be.

The comparison between Duff's device, std::copy, and memcpy is done in the program provided. The time of the execution is measured and saved to extract the time needed for the copy operation to be completed. The copy is done 100 times and the program will display the time it took at the end per technique, i.e. for Duff's device and the copy using the std::transform.

The memcpy_copy function, which first copies the data from the source to a temporary buffer using memcpy, then adds 1 to each element in the temporary buffer, and finally copies the modified elements to the destination array. This approach ensures that memcpy_copy performs a similar operation to the other two functions for a fair comparison.

## Comparison Factors:
### Compiler Optimizations:
**Duff's Device:** Modern compilers may struggle to optimize Duff's device as effectively as higher-level abstractions. While it's a clever technique for unrolling loops, compilers might not apply as many optimizations due to its unconventional structure.<br />  
**std::transform:** Modern compilers are excellent at optimizing standard library algorithms like std::transform. These functions are highly optimized and can take advantage of various compiler optimizations, including loop unrolling, vectorization, and inlining.<br />  
**memcpy:** memcpy is highly optimized and widely used in low-level memory operations. Compilers optimize it aggressively, especially when combined with simple arithmetic operations.<br />  

### Readability and Maintainability:
**Duff's Device:** Duff's device is less readable and more complex. It requires a deep understanding of the switch-case unrolling technique, which can be error-prone and harder to maintain.<br />  
**std::transform:** std::transform is much more readable and maintainable. Its purpose is clear, making the code easier to understand and maintain. Using standard algorithms reduces the risk of errors.<br />  
**memcpy:** While memcpy itself is straightforward, combining it with additional operations (like adding 1 to each element) reduces readability. It is still more readable than Duff's device but less so than std::transform.<br />  

### Portability:
**Duff's Device:** Duff's device is portable, but its behavior can vary slightly depending on the compiler's handling of loop unrolling and fallthrough warnings.<br />  
**std::transform:** std::transform is highly portable, guaranteeing consistent behavior across different platforms and compilers.<br />  
**memcpy:** memcpy is portable and standardized, ensuring consistent behavior. However, combining it with custom arithmetic operations might require additional code, potentially reducing portability.<br />  

### Performance:
**Duff's Device:** While Duff's device was a clever optimization for older hardware, modern CPUs and compilers handle such operations efficiently with higher-level abstractions. Its performance may not significantly surpass optimized standard algorithms.<br />  
**std::transform:** std::transform benefits from modern compiler optimizations and often performs well due to its high-level abstraction and optimization potential.<br />  
**memcpy:** memcpy is highly optimized for raw memory copying. When combined with simple arithmetic operations, it can be very efficient. Its performance will depend on the overhead of the additional operations.<br />  

### Conclusion:
**Duff's Device:** While historically interesting, Duff's device is less readable, harder to maintain, and may not offer significant performance benefits on modern systems.<br />  
**std::transform:** Offers the best balance of readability, maintainability, portability, and performance due to compiler optimizations.<br />  
**memcpy:** Provides excellent raw performance for memory operations but can be less readable and maintainable when combined with additional operations.<br />  

### Results:
```
Duff's device time: 0.104453s
std::transform time: 0.346826s
memcpy time: 0.358945s
```

## Note
The times displayed above can vary based on the hardware, compiler, and specific optimizations applied.<br />  
You might also notice in the code the following two segments of code:
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