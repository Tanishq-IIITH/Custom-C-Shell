#include "iman.h"

int min(int x, int y)
{
    if (x < y)
        return x;
    else
        return y;
}
void remove_html_tags(char *str)
{
    int in_tag = 0;
    char *dst = str;

    for (char *src = str; *src != '\0'; src++)
    {
        if (*src == '<')
        {
            in_tag = 1;
            continue;
        }
        if (*src == '>')
        {
            in_tag = 0;
            continue;
        }
        if (!in_tag)
        {
            *dst++ = *src;
        }
    }
    *dst = '\0';
}

void fetchManPage(char *url)
{
    url = trimspace(url);
    char *hostname = "man.he.net\0";
    char *path = (char *)malloc(sizeof(char) * 1024);
    snprintf(path, 1024, "?topic=%s&section=all", url);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        perror("socket");
        exit(1);
    }
    struct hostent *host = gethostbyname(hostname);
    if (host == NULL)
    {
        perror("gethostbyname");
        close(sockfd);
        exit(1);
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // HTTP port
    memcpy(&server_addr.sin_addr.s_addr, host->h_addr, host->h_length);
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("connect");
        close(sockfd);
        exit(1);
    }
    char request[1000000];
    snprintf(request, sizeof(request), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, hostname);
    free(path);
    if (write(sockfd, request, strlen(request)) == -1)
    {
        perror("write");
        close(sockfd);
        exit(1);
    }
    char response[1000000];
    int bytes_received;
    int f = 0;
    int ct = 0;
    bytes_received = read(sockfd, response, sizeof(response));
    int br = 0;
    while (bytes_received > 0)
    {
        ct++;
        remove_html_tags(response);
        {
            for (int i = 0; i < bytes_received; i++)
            {
                if (f == 0)
                {
                    char *x = "NAME\n";
                    char *y1 = "AUTHO";
                    char *y = (char *)malloc(sizeof(char) * 10);
                    int ct = 0;
                    for (int j = i; j < min(i + 5, bytes_received); j++)
                    {
                        y[ct++] = response[j];
                    }
                    y[ct] = '\0';
                    if (strcmp(x, y) == 0)
                    {
                        i--;
                        f = 1;
                    }
                    else if (strcmp(y1, y) == 0)
                    {
                        br = 1;
                        free(y);
                        break;
                    }
                    free(y);
                }
                else if (f == 1)
                {
                    char *y1 = "AUTHO";
                    char *y = (char *)malloc(sizeof(char) * 10);
                    int ct = 0;
                    for (int j = i; j < min(i + 5, bytes_received); j++)
                    {
                        y[ct++] = response[j];
                    }
                    y[ct] = '\0';

                    if (strcmp(y1, y) == 0)
                    {
                        br = 1;
                        free(y);
                        break;
                    }
                    printf("%c", response[i]);
                    free(y);
                }
            }
        }
        if (br)
            break;
        printf("%s", response);

        bytes_received = read(sockfd, response, sizeof(response));
    }
    printf("\n");
    close(sockfd);

    return;
}
