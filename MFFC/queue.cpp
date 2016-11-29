#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include "queue.h"
#include "bnet.h"

using namespace std; 
	
//Constructor
queue::queue()                          
{  
   front = rear = new NODE; 
    if(!front) 
       cout << "Can't malloc new space"; 
    front->next = NULL; 
    rear->next = NULL;
}  

queue::~queue()                          
{  
	if(front != rear)
   	{
   		delete front;
   		delete rear;
   	}
   	else
   		delete front;
}  
	  
//Push into the queue
void queue::push(char *e)  
{  
	
	NODE* p = new NODE;  
	if (NULL == p)  
	{  
	    cout << "Failed to malloc the node." << endl;
		return;
	}  
	p->data = e;  		
	p->next = NULL;  
	rear->next = p;  
	rear = p; 
	
/*	BnetNode *nd = (*net)->nodes;    
    while (nd != NULL) 
    {
    	printf("Current node is %s, output: ", nd->name);
    	for(int i = 0; i < nd->nfo; i++)
    	{
    		char *outnode = nd->fanouts[i];
            cout << outnode << " ";
    	}
    	cout << endl;
    	nd = nd->next;
    }
*/
}  
	  
//Pop from the queue 
char * queue::pop()  
{  
	char * e;  
	if (front == rear)  
	{  
		cout << "The queue is empty." << endl;  
	    return NULL;  
	}  
	else  
	{  
	    NODE* p = front->next;  
	    front->next = p->next; 
//	    strcpy(e, p->data); 
	    e = p->data; 
		 
	    if (rear == p)  
	    {  
	        rear = front;  
	    }  
	    delete p;  
	    return e;  
	}  
}  
	  
//Get the front element
char * queue::front_element()  
{  
	if (front == rear)  
	{  
	    cout << "The queue is empty." << endl;  
	    return NULL;  
	}  
	else  
	{  
	    return front->next->data;  
	}  
}  
	  
//Get the back element
char * queue::back_element()  
{  
	if (front == rear)  
	{  
	    cout << "The queue is empty." << endl;  
	    return NULL;  
	}  
	else  
	{  
	    return rear->data;  
	}  
}  
	      
//Get the size of the queue  
int queue::size()  
{  
	int count(0);  
	  
	NODE* p = front;  
	  
	while (p != rear)  
	{  
	    p = p->next;  
	    count++;  
	}  
	return count;  
}  
	      
//Judge if the queue is empty  
bool queue::empty()  
{  
	if (front == rear)  
	{  
	    return true;  
	}  
	else  
	{  
	    return false;  
	}  
}  

void queue::traverse()
{
/*    cout << endl << "Traverse the queue: " << endl;
	NODE *p = front->next;
	while(p != NULL)
	{
		cout << p->data << " ";
		p = p->next;
	}
	cout << endl;
*/
}
	
	
