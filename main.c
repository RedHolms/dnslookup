#include <stdio.h>

#include <conio.h>
#include <WinSock2.h>

void
halt()
{
  (void)fputs("Press any key to continue...", stdout);
  (void)_getch();
  (void)fputs("\n", stdout);
}

int
main(int argc, char** argv)
{
  WSADATA wsaData;
  int code;
  char* domain;
  struct hostent* h;
  char* addr;
  char addr_str[3 + 1 + 3 + 1 + 3 + 1 + 3 + 1];

  if (argc < 2) {
    (void)fputs("No domain name\n", stderr);
    halt();
    return 1;
  }
  
  code = WSAStartup(MAKEWORD(1, 1), &wsaData);

  if (code != 0) {
    (void)fputs("Failed to startup WSA\n", stderr);
    halt();
    return 1;
  }

  for (char** arg_ptr = &argv[1]; *arg_ptr; ++arg_ptr) {
    domain = *arg_ptr;

    (void)fputs(domain, stdout);
    (void)fputs("\n", stdout);

    h = gethostbyname(domain);

    for (char** addr_ptr = &h->h_addr_list[0]; *addr_ptr; ++addr_ptr) {
      addr = *addr_ptr;

      sprintf(addr_str, "%hhu.%hhu.%hhu.%hhu", addr[0], addr[1], addr[2], addr[3]);

      (void)fputs("\t", stdout);
      (void)fputs(addr_str, stdout);
      (void)fputs("\n", stdout);
    }
  }

  halt();

  (void)WSACleanup();
  return 0;
}
