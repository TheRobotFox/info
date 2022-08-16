#include "info.h"

void im_here_now()
{
        HOLD
        INFO("But thats ")
        COLOR(255,0,0)
        PRINT("not ")
        COLOR(255,255,255)
        PRINT("all...")
        RELEASE
}

List primes(size_t n)
{
        List primes = List_create(sizeof(size_t));
        size_t i=2;
        INFO("Found prime number! %d", i)
        List_append(primes, &i);
        for(i++; i<n; i+=2)
        {
                for(size_t *start=List_start(primes),
                           *end=List_end(primes); start!=end; start++)
                {
                        if(i%*start==0)
                               goto non_prime;
                }
                INFO("Found prime number! %d", i)
                List_append(primes, &i);
        non_prime:
        }
        return primes;
}

int main()
{
        INFO("FIRST TEST!!!")
        COLOR(255,0,0);
        ERROR("Uhh oh this was a mistake")
        im_here_now();
        SUCCESS("Or was it?")
        HOLD
        INFO("I ")
        COLOR(0,255,255)
        PRINT("auto ")
        COLOR(255,255,255)
        PRINT("release")
        INFO("see")

        #define PRIMES_TO 25
        INFO("Calculating Primes to %d", PRIMES_TO)
        SEG_BEGIN("Calculating Primes")
        List p = primes(PRIMES_TO);
        SEG_END
        PRINT("Found %d primes", List_size(p))

        RESET
        HOLD
        PRINT("Wanna know something cool?\nLast prime was ")
        COLOR(255,200,120)
        PRINT("%d", *(size_t*)List_get(p, -1))
        RELEASE
        INFO("DONE!")

        List_free(p);

        COLOR(120,120,120)
        PRINT("Now check out the FATAL error auto exit!!")
        FATAL("noooo")
        INFO("DONT PRINT THIS\ni think i haven added the handler yet...")
}
