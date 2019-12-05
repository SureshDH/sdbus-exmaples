#include <stdio.h>
#include <string.h>

struct Student{
	int id;
	char name[1024];
};

struct Student s2 = {515, "Nirosh Nani"};

int main(){
	struct Student s1 = {514, "Dharavath Suresh"};
	// s1.id = 514;
	// strncpy(s1.name, "Dharavath Suresh", 16);
	printf("Student id: %d and name: %s\n", s1.id, s1.name);
	printf("Student id: %d and name: %s\n", s2.id, s2.name);
	return 0;
}
