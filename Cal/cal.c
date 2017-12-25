/***********************************************************************
* Code listing from "Advanced Linux Programming," by CodeSourcery LLC  *
* Copyright (C) 2001 by New Riders Publishing                          *
* See COPYRIGHT for license information.                               *
***********************************************************************/

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "server.h"

/* HTML source for the start of the page we generate.  */

static char* page_start =
  "<html >\n"
  "<meta charset='UTF-8'>\n"
  " <body>\n"
  "  <pre>\n";

/* HTML source for the end of the page we generate.  */

static char* page_end =
  "  </pre>\n"
  " </body>\n"
  "</html>\n";

void module_generate (int fd)
{
  pid_t child_pid;
  int rval;

  /* Write the start of the page.  */
  write (fd, page_start, strlen (page_start));
  /* Fork a child process.  */
  child_pid = fork ();
  if (child_pid == 0) {
    /* This is the child process.  */
    /* Set up an argumnet list for the invocation of df.  */
    char *mes = NULL;
    char *ano = NULL;
    char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
    char *p4 = NULL;
    /*
    p1 = strtok(parametros, " ?=&");
    p2 = strtok(NULL, " ?&=");
    p3 = strtok(NULL, " ?&=");
    p4 = strtok(NULL, " ?&=");


    if(strcmp(p1,"ano") == 0 || strcmp(p3,"mes") == 0){
      ano = p2;
      mes = p4;
    }else{
      ano = p4;
      mes = p2;
    }*/

    if(strncmp(parametros, "ano", 3) == 0){
      ano = strtok(parametros, "?ano=&mes");
      mes = strtok(NULL, "&mes=ano");
    }else{
      mes = strtok(parametros, "?mes=&ano");
      ano = strtok(NULL, "&ano=mes");
    }
    if(mes == NULL){
      mes = ano;
      ano = NULL;
    }


    char* argv[] = { "/usr/bin/cal", "-h", mes,  ano, NULL };

    /* Duplicate stdout and stderr to send data to the client socket.  */
    rval = dup2 (fd, STDOUT_FILENO);
    if (rval == -1)
      system_error ("dup2");
    rval = dup2 (fd, STDERR_FILENO);
    if (rval == -1)
      system_error ("dup2");
    /* Run df to show the free space on mounted file systems.  */
    execv (argv[0], argv);
    /* A call to execv does not return unless an error occurred.  */
    system_error ("execv");
  }
  else if (child_pid > 0) {
    /* This is the parent process.  Wait for the child process to
       finish.  */
    rval = waitpid (child_pid, NULL, 0);
    if (rval == -1)
      system_error ("waitpid");
  }
  else
    /* The call to fork failed.  */
    system_error ("fork");
  /* Write the end of the page.  */
  write (fd, page_end, strlen (page_end));
}
