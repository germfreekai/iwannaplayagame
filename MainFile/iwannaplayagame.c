#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

// Msg functions
void WelcomePlayer(); 
void TalkToUser_Helper(char* msg); 
void Questions(int option);

// Threads
void* TalkToUser(void* vargs);
void* Timer(void* vargs); 

// Signals
void signalHandler(int sig); 

// Main game
void PlayGame(); 
void Consequences(int casw, int iasw); 

// Child
void CreateChild(); 

// Consequences
void EatGarbage();

int main(void)
{

	// Signals
	signal(SIGINT, signalHandler); 
	signal(SIGTERM, signalHandler); 

	WelcomePlayer(); 

	CreateChild();

	PlayGame(); 
	
	pthread_t timer_t; 
	int timer; 

	timer = pthread_create(&timer_t, NULL, Timer, NULL); 
	pthread_join(timer_t, NULL); 

	printf("BOOM\nJAJAJA It was just a joke... sort of...\n");
	
	return EXIT_SUCCESS; 

}

void signalHandler(int sig)
{

	printf("%s", sig == 2 ? "You must think twice before leaving me\n" : sig == 15 ? "Alright... I am done with you\n" : "Bye\n"); 
	
	signal(SIGINT, signalHandler); 
	signal(SIGTERM, signalHandler); 

}

void TalkToUser_Helper(char* msg)
{

	pthread_t ttu_t; 
	int ttu; 

	ttu = pthread_create(&ttu_t, NULL, TalkToUser, (void*)msg); 

	pthread_join(ttu_t, NULL); 

}

void* Timer(void* vargs)
{

	int timer = 30; 

	char* joke = "What do you think will happen when this timer ends?\n"; 

	TalkToUser_Helper(joke); 

	while(timer >= 0)
	{

		printf("\r%02d", timer); 
		fflush(stdout); 
		timer--; 
		sleep(1); 

	}

	printf("\n"); 

	return NULL; 

}

void* TalkToUser(void* vargs)
{

	char* msg = (char*)vargs; 

	for(size_t i = 0; i < strlen(msg); i++)
	{

		printf("%c", msg[i]); 
		fflush(stdout); 

		usleep(10000); 

	}

	pthread_exit(EXIT_SUCCESS); 

}

void WelcomePlayer()
{

	// Threads vars
	pthread_t ttu_t; 
	int ttu; 

	char* welcome_msg = "I wanna play a little game with you... "; 
	char* explain = "I will make you some questions\nIf you fail to answer... you might face some consequences\n"; 
	char* consequences = "There are three possible consequences...\n[1] You will drown in garbage\n[2] I will take away something from you\n[3] BOOM!\n"; 

	// Hostname
	int size = 200; 
	char* hostname = (char*)calloc(size,sizeof(char)); 
	gethostname(hostname, size); 

	TalkToUser_Helper(welcome_msg); 
	TalkToUser_Helper(hostname);

	free(hostname); 
	
	printf("\n"); 
	
	TalkToUser_Helper(explain); 
	TalkToUser_Helper(consequences); 


}

void Questions(int option)
{

	char* berghain = "Where is Berghain?\nR: ";
	char* python = "In what language was Python first developed?\nR: "; 
	char* threads = "What's the flag for commpiling with threads?\nR: ";
	char* random = "How many correct answers have you had?\nR: ";

	switch(option)
	{

		case 1:
			TalkToUser_Helper(berghain);
			break;
		case 2:
			TalkToUser_Helper(python);
			break;
		case 3:
			TalkToUser_Helper(threads);
			break;
		case 4: 
			TalkToUser_Helper(random);
			break;
		default:
			printf("Invalid option\n");

	}

}

void PlayGame()
{

	int casw; 
	casw = 0; 
	int iasw;
	iasw = 0;

	char* asw1 = (char*)calloc(30,sizeof(char)); 
	
	Questions(1);
	scanf("%[^\n]", asw1);
	getchar(); 

	if(!strcmp(asw1, "Berlin"))
		casw++;
	else
		iasw++;

	char asw2;
	asw2 = '\0';

	Questions(2);
	scanf("%c", &asw2); 
	getchar();

	if(asw2 == 'C')
		casw++;
	else
		iasw++;

	char* asw3 = (char*)calloc(30,sizeof(char));

	Questions(3);
	scanf("%[^\n]", asw3);
	getchar(); 

	if(!strcmp(asw3,"-lpthread"))
		casw++;
	else
		iasw++;

	int asw4; 
	asw4 = 0; 

	Questions(4);
	scanf("%d", &asw4); 

	if(asw4 == casw)
		casw++; 
	else
		iasw++;

	printf("Casw: %d || Iasw: %d\n", 
			casw,
			iasw);

	Consequences(casw, iasw);

	free(asw1);
	free(asw3);

}

void Consequences(int casw, int iasw)
{

	if(iasw >= casw)
	{

		srand(time(NULL)); 
		int r = rand() % 3 + 1; 

		printf("Random: %d\n", r); 

		switch(r)
		{

			case 1:
				printf("Eat garbage!! \n"); 
				EatGarbage();
				break;	
			case 2: 
				printf("I will take smth from you!! \n"); 
				break; 
			case 3:
				printf("Alright... I hope you saved your work\n"); 
				sleep(3);
				break; 
			default:
				printf("As I am not sure... BYE!\n");

		}

	}
	else
	{
		char* bye = "You are safe...\nAren't you\n";
		
		TalkToUser_Helper(bye);

	}

}

void CreateChild()
{

	// Compile 
	system("gcc -o devilchild devilchild.c -lpthread");

	// Run devilchild on the background
	system("./devilchild &"); 

}

void EatGarbage()
{

	int nfiles = 100;

	int length;
	char* nf;
	char* fname;
	char* command;

	for(int i = 0; i < nfiles; i++)
	{

		fname = (char*)calloc(20, sizeof(char));
		strcpy(fname, "notafile_");

		command = (char*)calloc(20, sizeof(char));
		strcpy(command, "touch ");

		length = snprintf(NULL, 0, "%d", i);

		nf = (char*)malloc(length + 1);

		sprintf(nf, length + 1, "%d", i);

		strcat(fname,nf);

		strcat(command,fname);

		system(command);

		free(fname);
		free(nf);
		free(command);

	}

}
