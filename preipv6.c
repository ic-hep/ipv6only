#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>


/* If this is set, then we don't allow IPv4 connections. */
static int gIPV6_ONLY = 0;

int (*real_connect)(int, __const struct sockaddr *, socklen_t) = NULL;

/* This function is run as the library is loaded. */
static void init_lib(void) __attribute__((constructor));

void init_lib(void)
{
  /* Find the handle for the real connect function. */
  real_connect = dlsym(RTLD_NEXT, "connect");
  if (!real_connect)
  {
    fprintf(stderr, "libipv6only: Failed to find connect function.\n");
    /* Exit now to avoid a crash later. */
    _exit(-1);
  }
  /* Process our control flags. */
  if (getenv("IPV6_ONLY"))
    gIPV6_ONLY = 1;
}

int connect (int __fd, __const struct sockaddr *__addr, socklen_t __len)
{
  /* Check this connect configuration is allowed. */
  if ((gIPV6_ONLY) && (__addr->sa_family == AF_INET))
  {
    errno = ENETUNREACH;
    return -1;
  }
  /* Just pass the request on. */
  return real_connect(__fd, __addr, __len);
}

