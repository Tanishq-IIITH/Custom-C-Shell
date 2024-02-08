#include "headers.h"
#include <netdb.h>
#include <arpa/inet.h>
#include <regex.h>
#include "math.h"
#define MAN_HE_NET_IP "216.239.32.21" // IP address for man.he.net
#define HTTP_PORT 80
void fetchManPage(char *url);
int min(int x, int y);
void remove_html_tags(char *str);
void fetchManPage(char *url);
