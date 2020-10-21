#include "stack.h"

int main(){

    Stack steck;

    StackCreate(&steck);

    switch(prog){

    case 1:	
    		StackPush(&steck, 94.2);
    		StackPush(&steck, 62.3);
    		StackPush(&steck, 3.2);
    		StackPush(&steck, 24.4);
    		StackPush(&steck, 0.5);
	    	break;

	case 2:
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		StackPush(&steck, 5);
    		break;

    case 3:
    		StackPush(&steck, 'd');
    		StackPush(&steck, 'e');
    		StackPush(&steck, 'd');
    		StackPush(&steck, ' ');
	    	StackPush(&steck, 'e');
	    	StackPush(&steck, 'd');
	    	StackPush(&steck, 'a');
    		break;
}
    
    StackDump(&steck, 0);

    StackDiscard(&steck);
    
    return 0;
}