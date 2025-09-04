#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/pstat.h"
#include "user/user.h"

int pid1 = 0, pid2 = 0, pid3 = 0;

void ps()
{
  struct pstat *p = malloc(sizeof(struct pstat));
  getpinfo(p);

  printf("NAME\tPID\tSTATUS\t\tCOLOR\tTICKETS\tTICKS\n");

  for (int i = 0; i < NPROC; i++)
  {
    char *status_tostring = "NULL";
    char *color_tostring = "NULL";

    if ( p->pid[i] == pid1 || p->pid[i] == pid2 || p->pid[i] == pid3 )
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
        printf("%s\t%d\t%s\t%s\t%d\t%d\n", p->name[i], p->pid[i], status_tostring, color_tostring, p->tickets[i], p->ticks[i]);
      }
      
      else
      {
        printf("%s\t%d\t%s\t\t%s\t%d\t%d\n", p->name[i], p->pid[i], status_tostring, color_tostring, p->tickets[i], p->ticks[i]);
      } 
    }
  }
  free(p);
}

int main()
{
  pid1 = fork();
  
  if ( pid1 == 0 )
  {
    setTickets(30);
    for(;;) {
    }

    exit(0);
  }

  pid2 = fork();

  if ( pid2 == 0 )
  {
    setTickets(20);
    for(;;);

    exit(0);
  }

  pid3 = fork();

  if ( pid3 == 0 )
  {
    setTickets(10);
    for(;;);

    exit(0);
  }

  printf("child pids: %d, %d, %d\n", pid1, pid2, pid3);
  sleep(100);

  // ps
  ps();
  sleep(100);
  ps();
  sleep(100);
  ps();
  

  kill(pid1);
  kill(pid2);
  kill(pid3);

  return 0;
}