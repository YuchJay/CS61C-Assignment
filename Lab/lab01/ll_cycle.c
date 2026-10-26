#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head)
{
    node *tortoise = head;
    node *hare = head;
    while (hare != NULL && hare->next != NULL)
    {
        for (int i = 0; i < 2; i++)
        {
            if (hare->next == NULL)
            {
                return 0;
            }
            hare = hare->next;
        }
        tortoise = tortoise->next;

        if(tortoise == hare){
            return 1;
        }
    }
    return 0;
}