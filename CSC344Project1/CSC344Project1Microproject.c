//Program by Jack Wilcox

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Item Item;

struct Item
{
    char name[255];
    float price;
    bool isEmpty;
};

int main()
{
    const char INPUT_QUIT = 'n';
    const int PROG_QUIT   = -1;

    int numShelves  = 0;
    int numSlots    = 0;

    char userItemInput = ' ';
    char itemName[255] = {};
    float itemPrice    = 0;
    int itemShelfLoc   = 0;
    int itemSlotLoc    = 0;

    int userShelfSearch = 0;
    int userSlotSearch  = 0;
    Item targetItem;

    /**************************************************************************************************************************************************/

    printf("How many shelves are there: ");
    scanf("%d", &numShelves);

    printf("How many slots on each shelf are there: ");
    scanf("%d", &numSlots);

    Item* shelf = malloc((numShelves * numSlots) * sizeof(Item));

    for(int i = 0; i < numShelves * numSlots; i++)
    {
        shelf[i].isEmpty = true;
    }

    fflush(stdin);

    printf("\nWould you like to add an item on the shelf? (Type 'y' for yes, 'n' for no): ");
    scanf("%c", &userItemInput);

    while(userItemInput != INPUT_QUIT)
    {
        if(userItemInput == 'y')
        {
            printf("\nGive Item Name: ");
            scanf("%s", &itemName);

            printf("Give Item Price: ");
            scanf("%f", &itemPrice);

            printf("Give Item Shelf Location: ");
            scanf("%d", &itemShelfLoc);

            printf("Give Item Slot Location: ");
            scanf("%d", &itemSlotLoc);

            while((itemShelfLoc < 1 || itemShelfLoc > numShelves) || (itemSlotLoc < 0 || itemSlotLoc > numSlots))
            {
                printf("\nThat spot on the shelf doesn't exist. Choose another\n\n");

                printf("Give Item Shelf Location: ");
                scanf("%d", &itemShelfLoc);

                printf("Give Item Slot Location: ");
                scanf("%d", &itemSlotLoc);
            }

            for (int i = 0; i < numShelves; i++)
            {
                for (int j = 0; j < numSlots; j++)
                {
                    if(i == itemShelfLoc - 1 && j == itemSlotLoc - 1)
                    {
                        strcpy(shelf[i * numSlots + j].name, itemName);
                        shelf[i * numSlots + j].price = itemPrice;
                        shelf[i * numSlots + j].isEmpty = false;

                        break;
                    }
                }
            }
        }

        else
        {
            printf("\n***NOT A VALID INPUT!***\n");
        }

        fflush(stdin);

        printf("\nWould you like to add an item on the shelf? (Type 'y' for yes, 'n' for no): ");
        scanf("%c", &userItemInput);
    }

    printf("\nYou can now browse the items that are on the shelf\n");

    printf("Enter the shelf number of an item (Or type '-1' to quit the program): ");
    scanf("%d", &userShelfSearch);

    while(userShelfSearch != PROG_QUIT)
    {
        printf("Enter the slot number of an item: ");
        scanf("%d", &userSlotSearch);

        if((userShelfSearch < 1 || userShelfSearch > numShelves) || (userSlotSearch < 0 || userSlotSearch > numSlots))
        {
            printf("\nThat spot on the shelf doesn't exist\n");
        }

        else
        {
            for (int i = 0; i < numShelves; i++)
            {
                for (int j = 0; j < numSlots; j++)
                {
                    if(i == userShelfSearch - 1 && j == userSlotSearch - 1)
                    {
                        targetItem = shelf[i * numSlots + j];

                        break;
                    }
                }
            }

            if(targetItem.isEmpty == true)
            {
                printf("\nThat spot on the shelf is empty\n");
            }

            else
            {
                printf("\nThat item's name is %s, and its price is %f\n", targetItem.name, targetItem.price);
            }
        }

        printf("\nEnter the shelf number of an item (Or type '-1' to quit the program): ");
        scanf("%d", &userShelfSearch);
    }

    free(shelf);

    return 0;
}
