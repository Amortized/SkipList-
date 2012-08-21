/*  SkipList - Implemented by Rahul Manghwani , NYU Courant*/

#include <iostream>
#include <math.h>
#include <time.h>
#include <cstdlib>

using namespace std;


class Skiplist 
{

private:
  struct Node 
  {
  double val;
  Node* below;  //Node below it
  Node* next;  //Next Node to this Node
  int level;  //Current Level of this node
  int height; //Total Height of the tower
  };


  Node* root;
  int currentHeight; //Current Height of the Tree


  int calLevels()
  {
	
	int randRes = 1;
	int level = 0;
	
	while(randRes)
	{
	   randRes = rand()%2;
	   if(randRes) //Head Go up
	   {
	     ++level;
	   }
	   
	   if(level > currentHeight)
	   {
	     break; //We only allow one level above the current level		
	   }	
	
         //If it was tail breaks here
	}

     return level;	
  }
 
  Node* createNode(double value, int level, int height) //This creates a top down link list of length level
  {
	if(level < 0)
	{
	  return NULL;
	}
	else
	{
	  //Create a stack of nodes recursively
	  Node* newNode = new Node();
	  (*newNode).val = value;
          (*newNode).level = level;
	  (*newNode).height = height;
          (*newNode).next = NULL;
          (*newNode).below = createNode(value, level-1, height);
	  return newNode; //Return a Node at the top of the stack
	}
	

  }

public:
  void add(double val);
  bool find(double val);
  bool remove(double val);

  Skiplist()
  {
    root = NULL;
    currentHeight = 0;

    srand(time(NULL)); //Needed by calLevels()	
  }


};


void Skiplist::add(double val)
{
    
  //If the Skiplist is empty
  if(root == NULL)
  {
     root = createNode(val, 0, 0);
     return;			
  }	

  //---Root Exists--

  //If new val is less than root; fix the roots 
  if((*root).val > val) 
  {
    double temp = (*root).val;	
    Node* cur = root;

    //Change Root to New val from top to bottom
    for(int i=currentHeight; i>=0; i--)
    {
	(*cur).val = val;	
	cur = (*cur).below;	
    }		

    //Now we should use the old root value to insert it
    val = temp;
  }
  
   //Get the no of Levels for this node
   int levels = calLevels();

   // Do we need to raise the tower; 
   if(levels > currentHeight)	  
   {
      currentHeight++;
      Node* newroot = new Node();
      (*newroot).val = (*root).val;
      (*newroot).next = NULL;
      (*newroot).level = currentHeight;
      (*newroot).below = root;
      root = newroot;
   } 


   /////At this Stage Root has been fixed
   
   //Create a Slab of Nodes
   Node* newNode = createNode(val,levels,levels);
   
   //Now add the slab 
   Node* current = root;
   Node* previous = NULL;

   for(int i = currentHeight; i>=0; i--)
   {

	//Go Ahead Until I hit a value greater than me or No more values exists
        while( (*current).next != NULL)
        {
	   if((*(*current).next).val > val)
	   {
	     //Oops to Ahead ; Search Ends for this Tower
	     break;								
	   } 
	   current = (*current).next;
        }
	   
	//Check if Newnode needs to be inserted at this level 
        if(i <= levels)
	{
	    (*newNode).next = (*current).next;
	    (*current).next = newNode;
	
	    //Go Below in the Slab
	    newNode = (*newNode).below;
	}

       //Root Goes Down
       current = (*current).below;

   }

}


bool Skiplist::find(double val)
{
      
  //Skiplist is empty
  if(root == NULL) 
  {
    return false;	
  }

  //If root is the value 
  if((*root).val == val)
  {
    return true;
  }

  Node* current = root;
  for(int i = currentHeight; i>=0 ; i--)
  {

    //Go Ahead Until u hit someone greater than you or no more no nodes are found 
    while( (*current).next !=NULL )
    {
	if( (*(*current).next).val > val) //If val is not found on this level
	{
	  break;
	}
	else if ( (*(*current).next).val == val)  //Found it
        {
	  return true;
        }		

	current = (*current).next;
    }	
	
    //Level Finished ; Go Down
    current = (*current).below;				 				
  }  

  //Not found 
  return false;   

}


//--------Remove -----------

bool Skiplist::remove(double val)
{

  if(root==NULL)							
  {						
   return false;												
  }							
  			
  Node* current = root;
  //Special Case:: Root is to be removed
  if((*root).val == val)
  {
     
    //Get the next root 
    for(int l = (*root).level; l>0; --l)
    {
	current = (*current).below;		
    } 	 		

    if(!(*current).next) //There was only one root node
    {
	delete root;
	root = NULL;
	currentHeight = 0;
	return true;
    }	
    

    // Change value of root to next node
    Node *n = root;
    Node *nextNode = (*current).next;

    for(int l = currentHeight; l >= 0; --l)
    {
       // change the root to the new value
       (*n).val = (*nextNode).val;

       // update next pointer if it exists
       if( (*n).next )
       {
          (*n).next = (*(*n).next).next;
       }
				
        // Move down to next level                              
        n = (*n).below;
     }

     return true;
	
  }


  bool found = false;
  /////  Remove after root; current still points to the root
  for(int l = currentHeight; l>=0; l--)
  {
    //Move forward while u hit a value greater than you
    while( (*current).next != NULL)
    { 	    
	
	if( (* (*current).next).val == val)
	{
	   found = true;
	
	   //Need to Remove the node; if next of next exist chnage pointers
	   if( (*(*current).next).next != NULL)
	 	(*current).next = (*(*current).next).next;  
	   else
	        (*current).next = NULL;	 

	   break; //This Level is Done
         
	} 
	else if( (* (*current).next).val > val)  //Need to go down; No at this level
	{
	  break;	
	
	}
	
	current = (*current).next;
    }  	

    //New level
    current = (*current).below;	
  }

  return found;
  
}


int main()
{
  Skiplist s1;
  for(int i=1;i<10000;i++)
   s1.add(i*2.5);
  
  for(int i=1;i<10000;i++)
   cout << s1.find(i*2.5) << endl;

  for(int i=10000;i>=1;i--)
   s1.remove(i*2.5);

  for(int i=1;i<10000;i++)
   cout << s1.find(i*2.5) << endl;		
 
  return 0;
}



   
