#include <stdio.h>

#include <conio.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define MAX(a, b) ((a > b) ? a : b)

void
init_ws()
{
  static WSADATA wsaData;
  register int code;

  code = WSAStartup(MAKEWORD(1, 1), &wsaData);

  if (code != 0) {
    fputs("Failed to initialize WSA\n", stderr);
    exit(1);
  }
}

void
halt()
{
  fputs("Press any key to continue...", stdout);
  _getch();
  fputs("\n", stdout);
}

void
dnslookup(const char* domain)
{
  struct hostent* hostent;
  char* alias;
  char addr[MAX(INET6_ADDRSTRLEN, INET_ADDRSTRLEN)];

  hostent = gethostbyname(domain);

  fputs(domain, stdout);
  fputs("\n", stdout);

  if (hostent == NULL) {
    fputs("  [not found]\n", stdout);
    return;
  }

  fputs("  aliasses:\n", stdout);
  if (hostent->h_aliases == NULL || hostent->h_aliases[0] == NULL) {
    fputs("    [none]\n", stdout);
    goto skip_aliases;
  }

  for (char** p = hostent->h_aliases; *p; ++p) {
    alias = *p;

    fputs("    ", stdout);
    fputs(alias, stdout);
    fputs("\n", stdout);
  }

  skip_aliases:

  fputs("  addresses:\n", stdout);
  if (hostent->h_addr_list == NULL || hostent->h_addr_list[0] == NULL) {
    fputs("    [none]\n", stdout);
    return;
  }

  for (char** p = hostent->h_addr_list; *p; ++p) {
    addr[0] = 0;
    (void)inet_ntop(hostent->h_addrtype, *p, addr, sizeof(addr));

    if (addr[0] == 0) {
      fputs("    [failed]\n", stdout);
      continue;
    }

    fputs("    ", stdout);
    fputs(addr, stdout);
    fputs("\n", stdout);
  }
}

int
main(int argc, char** argv)
{
  if (argc < 2) {
    fputs("No domain name\n", stderr);

    halt();

    return 1;
  }
  
  init_ws();

  for (char** arg = &argv[1]; *arg; ++arg) {
    dnslookup(*arg);
  }

  halt();

  WSACleanup();

  return 0;
}
