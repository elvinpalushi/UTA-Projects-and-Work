#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

int main()
{
  // Uncomment both set calls to see that ps Color is RED and Tickets are 1
  // setTickets(12);
  // setColor(GREEN);

  struct pstat *p = malloc(sizeof(struct pstat));

  getpinfo(p);

  printf("NAME\tPID\tSTATUS\t\tCOLOR\tTICKETS\n");

  for (int i = 0; i < NPROC; i++)
  {
    char *status_tostring = "NULL";
    char *color_tostring = "NULL";

    if (p->inuse[i])
    {
      switch (p->state[i])
      {
        case UNUSED:
          status_tostring = "UNUSED";
          break;
        case USED:
          status_tostring = "USED";
          break;
        case SLEEPING:
          status_tostring = "SLEEPING";
          break;
        case RUNNABLE:
          status_tostring = "RUNNABLE";
          break;
        case RUNNING:
          status_tostring = "RUNNING";
          break;
        case ZOMBIE:
          status_tostring = "ZOMBIE";
          break;
      }

      switch (p->color[i])
      {
        case RED:
          color_tostring = "RED";
          break;
        case ORANGE:
          color_tostring = "ORANGE";
          break;
        case YELLOW:
          color_tostring = "YELLOW";
          break;
        case GREEN:
          color_tostring = "GREEN";
          break;
        case BLUE:
          color_tostring = "BLUE";
          break;
        case INDIGO:
          color_tostring = "INDIGO";
          break;
        case VIOLET:
          color_tostring = "VIOLET";
          break;
      }

      if ( p->state[i] == SLEEPING || p->state[i] == RUNNABLE )
      {
        printf("%s\t%d\t%s\t%s\t%d\n", p->name[i], p->pid[i], status_tostring, color_tostring, p->tickets[i]);
      }
      
      else
      {
        printf("%s\t%d\t%s\t\t%s\t%d\n", p->name[i], p->pid[i], status_tostring, color_tostring, p->tickets[i]);
      } 
    }
  }

  free(p);

  return 0;
}