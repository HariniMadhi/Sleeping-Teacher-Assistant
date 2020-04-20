
#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<fcntl.h>
#include<string.h>

/* semaphore declaration*/

sem_t chair_sem[3];
sem_t stud;
sem_t ta_state;

/* threads */
pthread_t *Students;  //thread for N students
pthread_t TA; 	     //thread for teacher assistant
pthread_mutex_t ChairAccess; // mutex to apply locks on chairs

int chair_occupied = 0;
int next_index = 0;

int num_stud=-1;
int stop=-1;
char out_file[35]="";
int file_d=0;
int num_chairs=3;

/* this function checks that ta is sleeping, wakes him up when student comes, if no stud is threre,
we unlock chair mutex to allow for occupying seat , when chair is occupied chair count decrements which tells that only that number of
students can come. And Ta helps the stud */

void * TeacherAssistant()
{
	while(1)
	{
		sem_wait(&ta_state);		//TA is currently sleeping.
		printf("Student arrives\n");
		printf("TA has been awakened by a student.\n");
		while(1)
		{
			// when Ta is awakened a student occupies chair so we have to put lock on chairs

			pthread_mutex_lock(&ChairAccess);
			if(chair_occupied == 0) // we will check if chairs are empty then we unlock the chairs and let them occupy
			{
				//if chairs are empty, break the loop.
				pthread_mutex_unlock(&ChairAccess);
				break;
			}

			//TA gets next stud on chair.

			sem_post(&chair_sem[next_index]); // we signal the chair that has been occupied by a student, to come inside
			chair_occupied--;  // so chair occupied count decrements
			printf("Student left his/her chair in waiting room and goes to ta. Remaining Chairs %d\n",3 - chair_occupied);
			next_index = (next_index + 1) % 3;
			pthread_mutex_unlock(&ChairAccess);

			printf("\t TA is currently helping the student.\n");
			sleep(rand()%16+5); // TA helps the student for a random time between 5 to 20 seconds
			sem_post(&stud);   // Signal that doubt has been cleared
			usleep(1000);

		}

	}

}



void *Student(void *threadID)
{
	while(1)
	{

		printf("Student %ld is coding.\n", (long)threadID);
		sleep(rand()%20+1);
		printf("Student %ld wants to clear doubt from the TA\n", (long)threadID);
		// Student arrives so put lock on the chair
		pthread_mutex_lock(&ChairAccess);
		int count = chair_occupied;
		if(count < 3)		//if chair count < 3 student will occupy and  wait.
		{
			if(count == 0)		//If no one is there before student , wake up the TA.
				sem_post(&ta_state); // wake up ta semaphore
			else
				printf("Student %ld waiting for the TA to finish. \n", (long)threadID);

			int index = (next_index + chair_occupied) % 3;
			chair_occupied++;
			printf("Student sat on chair.Chairs Remaining: %d\n", 3 - chair_occupied);
			pthread_mutex_unlock(&ChairAccess);

			sem_wait(&chair_sem[index]);	//Student leaves his chair.
			printf("\t Student %ld is getting help from the TA. \n", (long)threadID);
			sem_wait(&stud);		//Student waits to go next.
			printf("Student %ld left TA room.\n",(long)threadID);

		}

		else
		{
			printf("Student %ld will return at another time. \n", (long)threadID);
			pthread_mutex_unlock(&ChairAccess);
		}
		sleep(5);

	}
}
int main(int argc,char* argv[])
{
	int id,i;
	for(i = 1; i < argc; i = i + 2)
        {
                int j = i + 1;
                if(strcmp(argv[i],"-n") == 0)   //number of students
                {
                        num_stud = atoi(argv[j]);
                }
                else if(strcmp(argv[i],"-s") == 0)   //stop time i.e working time
                {
                        stop = atoi(argv[j]);
                }
                else if(strcmp(argv[i],"--help") == 0)    //whether user asks for help
                {
			printf("\nUsage: ./sta [OPTION]... [FILE]...\nSimulation of sleeping teacher assistant problem\n\nCommand line arguments that can be supplied are\n    -n\tNumber of students in the class (default value is 5 if not specified)\n    -s\tWorking hrs(Time for program to run) in seconds\n    -f\tTo specify the file to display the output\n\t--help\tdisplay this help and exit program\n");
			exit(0);
                }
                else if(strcmp(argv[i],"-f") == 0)    //file to put the output
                {
                        strcpy(out_file,argv[j]);
			file_d = open(out_file, O_WRONLY | O_CREAT, 0644);
    			if (file_d == -1)
			{
        			perror("File open failed");
        			exit(1);
    			}
    			if (dup2(file_d, 1) == -1)
			{
        			perror("dup2 failed");
        			exit(1);
    			}
                }
                else                                    //an invalid argument
                {
                        printf("Invalid argument %s\nUse --help argument for information\n",argv[i]);
                        return 1;
		}
	}
	if (num_stud<0)
		num_stud=5;
	if (stop<0)
		stop=100000;
	srand(time(NULL));
	//Initializing Mutex Lock and Semaphores.
	time_t end,start=time(NULL);
	time_t inter=stop;
	end=start+inter;
	sem_init(&ta_state, 0, 0);

	sem_init(&stud, 0, 0);

	for(id = 0; id < 3; ++id)	//Chairs array of 3 semaphores.
		sem_init(&chair_sem[id], 0, 0);
	pthread_mutex_init(&ChairAccess, NULL);

	Students = (pthread_t*) malloc(sizeof(pthread_t)*num_stud);

	pthread_create(&TA, NULL, TeacherAssistant, (void*) (long)id);

	for(id = 0; id < num_stud; id++)
		pthread_create(&Students[id], NULL, Student,(void*) (long)id);
	while(1)
	{
		if (start<end)
		{
			sleep(1);
			start=time(NULL);
		}
		else
		{
        		pthread_cancel(TA);
			for(id = 0; id < num_stud; id++)
				pthread_cancel(Students[id]);
			printf("\nWorking hours are over, so TA goes home\n");
			break;
		}
	}
	free(Students);
	close(file_d);
	return 0;
}
