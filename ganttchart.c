#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
#define MAX 100

typedef struct
{
    int pid;
    int b_time;
    int w_time;
    int t_time;
} Student;

void print_table(Student p[], int n);
void print_gantt_chart(Student p[], int n);

int main()
{
    srand(time(NULL));
    Student p[MAX];
    int i, j, n=5;
    int sum_w_time = 0, sum_t_time;
    for(i=0; i<n; i++) {
        p[i].pid = i+1;
        p[i].b_time = rand()%16+5;
        p[i].w_time = p[i].t_time = 0;
    }


    p[0].t_time = p[0].b_time;

    for(i=1; i<n; i++) {
        p[i].w_time = p[i-1].w_time + p[i-1].b_time;
        p[i].t_time = p[i].w_time + p[i].b_time;
    }

    for(i=0; i<n; i++) {
            sum_w_time += p[i].w_time;
            sum_t_time += p[i].t_time;
    }

    // print table
    puts(""); // Empty line
    print_table(p, n);
    puts(""); // Empty Line
    printf("Total Waiting Time      : %-2d\n", sum_w_time);
    printf("Average Waiting Time    : %-2.2lf\n", (double)sum_w_time / (double) n);
    printf("Total Turnaround Time   : %-2d\n", sum_t_time);
    printf("Average Turnaround Time : %-2.2lf\n", (double)sum_t_time / (double) n);

    // print Gantt chart
    puts(""); // Empty line
    puts("          GANTT CHART          ");
    puts("          ***********          ");
    print_gantt_chart(p, n);
    return 0;
}


void print_table(Student p[], int n)
{
    int j;

    puts("+*****+*************+*************+*****************+");
    puts("| PID | Burst Time | Waiting Time | Turnaround Time |");
    puts("+*****+************+**************+*****************+");

    for(j=0; j<n; j++) {
        printf("| %2d  |     %2d     |      %2d      |        %2d       |\n"
               , p[j].pid, p[j].b_time, p[j].w_time, p[j].t_time );
        puts("+-----+------------+--------------+-----------------+");
    }

}

void print_gantt_chart(Student p[], int n)
{
    int i, j;
    printf(" ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].b_time; j++) printf("--");
        printf(" ");
    }
    printf("\n|");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].b_time - 1; j++) printf(" ");
        printf("S%d", p[i].pid);
        for(j=0; j<p[i].b_time - 1; j++) printf(" ");
        printf("|");
    }
    printf("\n ");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].b_time; j++) printf("--");
        printf(" ");
    }
    printf("\n");

    printf("0");
    for(i=0; i<n; i++) {
        for(j=0; j<p[i].b_time; j++) printf("  ");
        if(p[i].t_time > 9) printf("\b");
        printf("%d", p[i].t_time);

    }
    printf("\n");
}
