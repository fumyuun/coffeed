#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdio.h>
#include <stdlib.h>

//===============================
// Start of configuration options
//===============================

// How many seconds is our timer?
#define TIMER_S 5
// How many seconds to wait polling?
#define POLLRATE_S 1

// How many users do we have?
#define NO_USERS 6

// Path to the joystick device to use
char *js_device = "/dev/input/js0";

// Names of the users
char *users[NO_USERS] = 
{
    "user1",
    "user2",
    "user3",
    "user4",
    "user5",
    "user6"
};

//===============================
// End of configuration options
//===============================

// Will store offsets of each username, used when printing
int user_offsets[NO_USERS] = {0};

void print_buttons(int buttons);

int main() 
{
    int i;
    int js = open(js_device, O_RDONLY | O_NONBLOCK);
    srand(time(NULL));

    // calculate name offsets
    for(i = 1; i < NO_USERS; ++i)
    {
        user_offsets[i] = user_offsets[i - 1] + strlen(users[i - 1] + 1);
    }

    while(1)
    {
        struct js_event e;
        // First read always starts a pool
        if(read(js, &e, sizeof(struct js_event)) > 0)
        {
            if(e.type == JS_EVENT_BUTTON && e.value && e.number < NO_USERS)
            {
                int userpool[NO_USERS];
                int usercount = 0;

                printf("%s has started a coffee timer!\n", users[e.number]);
                userpool[usercount++] = e.number;
                
                // Print an overview of all users
                for(i = 0; i < NO_USERS; ++i)
                {
                    printf("%s ", users[i]);
                }
                putchar('\n');

                printf("\033[%dG^", user_offsets[e.number]+1);
                fflush(stdout);

                // Start counter!
                int time_left;
                for(time_left = TIMER_S; time_left >= 0; --time_left)
                {
                    printf("\033[%dG(%d)", user_offsets[NO_USERS-1] + 4, time_left);
                    fflush(stdout);

                    // Read all events until queue is empty
                    while(read(js, &e, sizeof(struct js_event)) > 0)
                    {
                        if(e.type == JS_EVENT_BUTTON && e.value && e.number < NO_USERS)
                        {
                            // Quickly search if this user is in the queue already
                            char user_found = 0;
                            for(i = 0; i < usercount; ++i)
                            {
                                if(userpool[i] == e.number)
                                {
                                    user_found = 1;
                                    break;
                                }
                            }
                            if(user_found)
                            {
                                continue;
                            }

                            // Add user to pool
                            printf("\033[%dG^", user_offsets[e.number]+1);
                            fflush(stdout);
                            userpool[usercount++] = e.number;
                        }
                    }
                    sleep(POLLRATE_S);
                }

                // Timer ended, print pool
                printf("\nCoffee timer is over, who wants coffee?\n");
                for(i = 0; i < usercount; ++i)
                {
                    puts(users[userpool[i]]);
                }
                // Get a random lucky user!
                printf("Who will get coffee: %s!\n", users[userpool[rand() % usercount]]);
            }
        }
        else
        {
            // Nothing to read, just sleep
            sleep(POLLRATE_S);
        }
    }
}
