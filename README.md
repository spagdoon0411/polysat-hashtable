# polysat-hashtable

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
