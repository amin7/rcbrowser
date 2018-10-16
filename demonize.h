/*
 * demonize.h
 *
 *  https://github.com/jirihnidek/daemon/blob/master/src/daemon.c
 *
 */

#ifndef DEMONIZE_H_
#define DEMONIZE_H_
#include <unistd.h>
#include <syslog.h>
#include <signal.h>
#include <iostream>

static void daemonize()
{
  pid_t pid = 0;
  int fd;

  /* Fork off the parent process */
  pid = fork();

  /* An error occurred */
  if (pid < 0) {
    exit (EXIT_FAILURE);
  }

  /* Success: Let the parent terminate */
  if (pid > 0) {
    exit (EXIT_SUCCESS);
  }

  /* On success: The child process becomes session leader */
  if (setsid() < 0) {
    exit (EXIT_FAILURE);
  }

  /* Ignore signal sent from child to parent process */
  signal(SIGCHLD, SIG_IGN);

  /* Fork off for the second time*/
  pid = fork();

  /* An error occurred */
  if (pid < 0) {
    exit (EXIT_FAILURE);
  }

  /* Success: Let the parent terminate */
  if (pid > 0) {
    exit (EXIT_SUCCESS);
  }

  /* Set new file permissions */
  umask(0);

//  /* Change the working directory to the root directory */
//  /* or another appropriated directory */
//  chdir("/");
//
//  /* Close all open file descriptors */
//  for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
//    close(fd);
//  }
  std::cout << "demon started" << std::endl;
  /* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
//  stdin = fopen("/dev/null", "r");
//  stdout = fopen("/var/log/debug", "w+");
//  stderr = fopen("/var/log/debug ", "w+");
}




#endif /* DEMONIZE_H_ */
