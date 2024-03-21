#include <stdio.h>
#include <windows.h>
#include <process.h>

#define MAX_THREADS 256
#define MAX_IP_LENGTH 16

struct PingResult {
    char ip[MAX_IP_LENGTH];
    BOOL success;
};

struct PingResult pingResults[MAX_THREADS];

void pingThread(void *param) {
    struct PingResult *result = (struct PingResult *)param;
    char command[50];
    sprintf(command, "ping -n 1 %s > tmp/%s", result->ip, result->ip);
    system(command); // Check if ping was successful
    Sleep(5000);
    // result->success =  
    _endthread();
}

HANDLE threads[MAX_THREADS];
int processArgsAndCreateThreads(const char *range) {
    struct PingResult pingResults[MAX_THREADS];
    int pingedCount = 0;

    char start[20];
    char end[20];
    int i, j;

    // Find the position of the '-' character
    int dashIndex = -1;
    for (i = 0; range[i] != '\0'; i++) {
        if (range[i] == '-') {
            dashIndex = i;
            break;
        }
    }

    // Extract start and end parts of the range
    if (dashIndex == -1) {
        printf("Invalid range format.\n");
        return -1;
    }
    strncpy(start, range, dashIndex);
    start[dashIndex] = '\0';
    strcpy(end, range + dashIndex + 1);

    // Convert the end part to an integer
    int endValue = atoi(end);

    // Validate the end part of the range
    if (endValue < 0 || endValue > 255) {
        printf("Invalid range format.\n");
        return -1;
    }

    // Process each IP address in the range
    for (i = 0; i <= endValue; i++) {
        // char ip[20];
        sprintf(pingResults[i - 1].ip, "%s%d", start, i);

        // Construct and execute the command for each IP address
        char command[256];
        
        // sprintf(pingResults[i - 1].ip, "192.168.1.%d", i);
        threads[i - 1] = (HANDLE)_beginthread(pingThread, 0, (void *)&pingResults[i - 1]);
        if (threads[i - 1] == NULL) {
            fprintf(stderr, "Thread creation failed.\n");
            return -1;
        }
    }
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s <range>\n", argv[0]);
        return 1;
    }
    system("rm -rf tmp");
    system("mkdir tmp");
    if(processArgsAndCreateThreads(argv[1]) == -1)
        return 1;

    // Wait for all threads to finish
    Sleep(10000);
    WaitForMultipleObjects(MAX_THREADS, threads, TRUE, INFINITE);
    system("cat tmp/192.168.* | grep Reply | grep bytes | awk '{print substr($3, 1, length($3)-1)}'");
    system("arp -a");
    system("rm -rf tmp");

    // system("arp -a | grep \"  192\" | awk '{print $1,$2}' | while read ip mac; do echo -n \"$ip $mac \"; curl -s https://api.macvendors.com/$mac; sleep 1; echo \"\"; done");
    // // Print pinged IP addresses in table format
    // printf("+-----------------+----------+\n");
    // printf("| %-15s | %-8s |\n", "IP Address", "Status");
    // printf("+-----------------+----------+\n");
    // for (int i = 0; i < MAX_THREADS; i++) {
    //     if (pingResults[i].success) {
    //         printf("| %-15s | %-8s |\n", pingResults[i].ip, "Success");
    //         pingedCount++;
    //     }
    // }
    // printf("+-----------------+----------+\n");
    // printf("Total IP addresses pinged: %d\n", pingedCount);

    return 0;
}
