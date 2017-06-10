STRID
=====

Generate random id 10 char long, by default. Uses base 36 encoding and include a checksum. It can be useful for application that need to send id through HTTP or any character-based protocol.

Usage
-----

Initialize the random number generator (use drand48()) by passing any string to strid_init(). Initialization with a static string will produce predictible results. You don't need to initialize, just you should.

    int main(void) {
        Strid myId;

        strid_init("No Random String");
        strid_generate(myId);

        printf("%s\n", myId);

        return 0;
    }

If the id comes from an extern source, you might want to verify it.

    int main(void) {
        /* ... */

        if(strid_valid(externId)) {
            printf("Valid ID");
        }

        /* ... */
    }

Two id can be compared with strcmp but, in case characters have been lowered, you might well use strid_equal.

    int main(void) {
    
        /* ... */

        if(strid_equal(Id1, Id2)) {
            printf("ID1 equals ID2");
        }

        /* ... */
        
    }

Please note that strid_equal doesn't validate the id at any point. So check first both ids with strid_valid and then strid_equal.

You can copy an id into another id with strid_copy.

    int main(void) {
        /* ... */

       strid_copy(idDest, idSource);

       /* ... */
    }

It's just a memcpy, see in strid.h :

    #define strid_copy(dest, src)           memcpy((dest), (src), STRID_MAX_SIZE)

License
-------

BSD 2-clauses, see LICENSE.txt

