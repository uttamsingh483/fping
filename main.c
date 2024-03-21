#include <stdio.h>
#include <windows.h>
#include <process.h>

#define MAX_THREADS 254
#define MAX_IP_LENGTH 16

struct PingResult {
    char ip[MAX_IP_LENGTH];
    BOOL success;
};

struct PingResult pingResults[MAX_THREADS];

// BOOL check_success() {
//     char command[50];
//     sprintf(command, "ping -n 1 %s", result->ip);
//     FILE *fp = _popen(command, "r");
//     Sleep(5000);
//     if (fp == NULL) {
//         strcpy(result->status, "Error");
//     } else {
//         char line[100];
//         while (fgets(line, sizeof(line), fp) != NULL) {
//             printf("%s\n", line);
//             if (strstr(line, "Reply from") != NULL) {
//                 strcpy(result->status, "Success");
//                 break;
//             } else if (strstr(line, "Request timed out") != NULL || strstr(line, "Destination host unreachable") != NULL) {
//                 strcpy(result->status, "Failed");
//                 break;
//             }
//         }
//         if (feof(fp)) {
//             strcpy(result->status, "Error");
//         }
//         _pclose(fp);
//     }
// }

void pingThread(void *param) {
    struct PingResult *result = (struct PingResult *)param;
    char command[50];
    sprintf(command, "ping -n 1 %s > tmp/%s", result->ip, result->ip);
    system(command); // Check if ping was successful
    Sleep(5000);
    // result->success =  
    _endthread();
}

int main() {
    HANDLE threads[MAX_THREADS];
    struct PingResult pingResults[MAX_THREADS];
    int pingedCount = 0;

    // Start ping commands in separate threads
    for (int i = 1; i <= 254; i++) {
        sprintf(pingResults[i - 1].ip, "192.168.1.%d", i);
        threads[i - 1] = (HANDLE)_beginthread(pingThread, 0, (void *)&pingResults[i - 1]);
        if (threads[i - 1] == NULL) {
            fprintf(stderr, "Thread creation failed.\n");
            return 1;
        }
    }

    // Wait for all threads to finish
    Sleep(10000);
    WaitForMultipleObjects(MAX_THREADS, threads, TRUE, INFINITE);
    system("cat tmp/192.168.1.* | grep Reply | grep bytes | awk '{print substr($3, 1, length($3)-1)}'");
    system("arp -a | grep \"  192\" | awk '{print $1,$2}' | while read ip mac; do echo -n \"$ip $mac \"; curl -s https://api.macvendors.com/$mac; sleep 1; echo \"\"; done");
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
