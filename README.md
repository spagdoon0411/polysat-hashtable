# polysat-hashtable

## Goals and Notes
- As encapsulated/modular as possible 
    - Separate functions for: 
        - Rehashing
        - Hash code generation (arguments would *include* the key and table size)
        - Insertion 
- Wrapper struct: 
    - Number of entries
    - Hash table size
    - Load factor (derived from the above two, likely found with a function for consistency/uniformity)
- For growth factors, [1.5 seems to be more optimal than 2](https://stackoverflow.com/questions/2369467/why-are-hash-table-expansions-usually-done-by-doubling-the-size). It would be a good idea to understand and be able to present on why this is the case. 

## Questions/To Consider
- A comment on this thread mentions a method for [lazy rehashing](https://stackoverflow.com/questions/9858751/how-to-implement-a-dynamic-size-hash-table)
- [About fragmentation](https://www.design-reuse.com/articles/25090/dynamic-memory-allocation-fragmentation-c.html)
- [Virtual memory](https://stackoverflow.com/questions/31773466/when-a-large-block-of-memory-is-requested-on-the-heap-if-contiguous-space-is-no): using disk storage as emergency RAM

## Task Description
This question would be good for those who are confident in their systems
programming.

Implement a hash table in the C programming language. Design criteria and things to consider:
1. Be able to take a key and a value. The key should be a string and the value should be
either a number or a string
2. The size of the hash-table should change dynamically. If there are only 5 items in the
table the size of the table should not be 1000. If the size of the table is only 10 and we
need to put 500 items in the table, it should be able to dynamically grow.
3. Collisions. Your hash table should use a hash that limits the amount of collisions of keys
hashing to the same area in memory. Collisions are bound to happen so make sure to use
a system to manage these collisions and still allow for values to be retrieved.
4. Don't allow for the same key to map to different values.
5. Bonus - create a command line interface to add and retrieve values based on key


Conceptual questions you should be able to talk about in your design:
- Talk about how to work with size changes
- Talk about how to work with clashes in hashes