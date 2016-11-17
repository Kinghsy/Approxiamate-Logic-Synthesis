#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "bnet.h"

using namespace std;  

   
struct NODE  
{  
    NODE *next;  
    char *data;  
};  
  
class queue
{  
	private:  
    	NODE* front; 
    	NODE* rear;
    	
	public:  
		queue();
		void push(char *e);
		char * pop();
		char * front_element();
		char * back_element();
		int size();
		bool empty();
		void traverse();
		~queue();
		
};  

#endif
