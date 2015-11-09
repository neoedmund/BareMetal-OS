(Neoe) Play With the Memory 
is a alloc/free/realloc implement  written by Neoe.


It can be used in layout in memory or disk or filesystem.


Because Neoe believes Java's develop speed (including coding, testing, debugging) are 10x times faster than C.
So, Neoe write and test things in Java, then, translate them into C with the help of J2C(another Neoe project).
Then, port C code into BMOS. Neoe found it work like magic and efficent.


Lv0 format's the memory into the layout, and read/write functions.
Lv1 is on the top of Lv0, do alloc, free, realloc(enlarge in this case).
Lv0Neos implements the "abstract" methods in Lv0, doing real read/write/copyRange functions.


in the tmp dir, TestLv0 is a implement running on hosts, easy for testing and debugging.
