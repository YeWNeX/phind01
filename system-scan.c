#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/utsname.h>

void write_command_output(FILE *fp, const char *header, const char *cmd) {
    fprintf(fp, "\n=== %s ===\n", header);
    FILE *cmd_fp = popen(cmd, "r");
    if (cmd_fp == NULL) {
        fprintf(fp, "[ERROR] Couldn't run command: %s\n", cmd);
        return;
    }
    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), cmd_fp) != NULL) {
        fputs(buffer, fp);
    }
    pclose(cmd_fp);
}

int main() {
    FILE *fp = fopen("system_report.txt", "w");
    if (fp == NULL) {
        perror("Cannot open report file");
        return 1;
    }

    // Print system information using uname
    struct utsname sysinfo;
    if (uname(&sysinfo) == 0) {
        fprintf(fp, "=== SYSTEM INFORMATION ===\n");
        fprintf(fp, "System Name: %s\n", sysinfo.sysname);
        fprintf(fp, "Node Name:   %s\n", sysinfo.nodename);
        fprintf(fp, "Release:     %s\n", sysinfo.release);
        fprintf(fp, "Version:     %s\n", sysinfo.version);
        fprintf(fp, "Machine:     %s\n", sysinfo.machine);
    } else {
        fprintf(fp, "Error retrieving system information.\n");
    }

    // CPU info
    write_command_output(fp, "CPU INFO", "lscpu");

    // RAM info
    write_command_output(fp, "MEMORY INFO", "free -h");

    // Disk usage
    write_command_output(fp, "DISK INFO", "df -h");

    // Installed dynamic libraries (system-wide)
    write_command_output(fp, "DYNAMIC LIBRARIES (ldconfig)", "ldconfig -p");

    // Environment variables
    write_command_output(fp, "ENVIRONMENT VARIABLES", "printenv");

    // USB devices
    write_command_output(fp, "USB DEVICES", "lsusb");

    // PCI devices
    write_command_output(fp, "PCI DEVICES", "lspci");

    // List current kernel modules
    write_command_output(fp, "LOADED KERNEL MODULES", "lsmod");

    fclose(fp);
    printf("System report written to system_report.txt\n");

    return 0;
}
