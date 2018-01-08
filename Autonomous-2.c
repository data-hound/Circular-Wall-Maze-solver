#include<stdio.h>
#define len 6

bool is_dist_present(int dist_sample, int dist_arr[len]);
void update_dist_arr(int dist_sample, int dist_arr[len]);

int main(){

    int dist_limit = 75;
    int dist_arr[len];
    dist_arr={0};
    int count =len;
    int sample_delay = 10;
    int move_delay = 100;
    int MOTOR_PORT=0xFF;
    int MOVE_LEFT_CONFIG=0xFF;
    int MOVE_RIGHT_CONFIG=0xFF;
    int SENSOR_PORT = 0xFF;
    int PUSH_END_CONFIG = 0xFF;
    int RETURN_END_CONFIG = 0xFF;

    while(1){
    	MOTOR_PORT = MOVE_LEFT_CONFIG;
    	delay_ms(sample_delay);
    	int dist_sample_1 = Read_Ultrasonic_fwd();
    	int dist_sample_2 = Read_Ultrasonic_bwd();

    	if(dist_sample_1 < dist_limit || dist_sample_2 < dist_limit){
    		if(is_dist_present(dist_sample_1, dist_arr)){
    		    //adc[0]=Ultrasonic Forward
    		    //adc[1]=Ultrasonic Backward
    		    //adc[2]=Left Forward IR
    		    //adc[3]=Right Forward IR
    		    //adc[4]=Left Backward IR
    		    //adc[5]=Right Backwar IR

    		    while((read_ad(2)!=1 || read_ad(3)!=1) && (read_ad(4)==0 && read_ad(5)==0)){
    			//while(SENSOR_PORT != PUSH_END_CONFIG) we move the bot forward
    			//PUSH_END_CONFIG occurs when both the front IR sensors cross into the white circle AND
    			//both the back IR sensors are still in the black circle region

    		        curr_sample = Read_Ultrasonic_fwd(); //We take a distance sample from the block before
    		                                             //Beginning to push it.

    	    		//This while runs till the bot moves straight in the direction of block
    	    		while(curr_sample < = prev_sample){
    	    			MOTOR_PORT = FORWARD_CONFIG;
    	    			delay_ms(move_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_fwd();
    	    		}

    	    		//This variable is used for determining the angle the bot would rotate for reconfiguration
    	    		//If it deviates from straight path => The angular range the bot should check to reallign itself

    	    		int step_count = 50;
    	    		int i;
    	    		for(i=0 ; i<step_count; i++){
    	    			MOTOR_PORT = MOVE_LEFT_CONFIG;
    	    			delay_ms(sample_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_fwd();
    	    			if(curr_sample < = prev_sample){
    	    				break;
    	    			}
    	    		}
    	    		//This for loop is used to scan to the left of the point
    	    		//=======USED WHEN BOT DEVIATES TO RIGHT OF PATH========

    	    		for(i=0; i<2*step_count;i++){
    	    			MOTOR_PORT = MOVE_RIGHT_CONFIG;
    	    			delay_ms(sample_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_fwd();
    	    			if(curr_sample < = prev_sample){
    	    				break;
    	    			}

    	    		}
    	    		//This for loop is used to scan to the right of the point
    	    		//=====USED WHEN BOT DEVIATES TO THE LEFT OF PATH=======

    			    //SENSOR_PORT = Read_IR();

    		    }//END OF FORWARD WHILE

    			//================================================================
    			//We need to add a code to push the block a bit into the white circle
    			//This is needed so that the bot doesnt confuse blocks outside the circle
    			//And bots close to the margin of the circle
    			//=================================================================

    			while((read_ad(2)==0 && read_ad(3)==0) && (read_ad(4)!=1 && read_ad(5)!=1)){
    			//while(SENSOR_PORT != RETURN_END_CONFIG) we move the bot backward from there
    			//RETURN_END_CONFIG is when both the back IR sensors enter the inner circle AND
                //both the front IR sensors are still in the black area

    			    curr_sample = Read_Ultrasonic_fwd(); //We take a distance sample from the block before
    		                                             //Beginning to push it.


    			    //This while runs till the bot moves straight in the direction of block
    			    while(curr_sample > = prev_sample){
    				    MOTOR_PORT = BACKWARD_CONFIG;
    				    delay_ms(move_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_fwd();
    			    }

    			    //This variable is used for determining the angle the bot would rotate for reconfiguration
    	    		//If it deviates from straight path => The angular range the bot should check to reallign itself

    			    int step_count = 50;

    			    int i;
    			    for(i=0 ; i<step_count; i++){
    				    MOTOR_PORT = MOVE_LEFT_CONFIG;
    				    delay_ms(sample_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_fwd();
    				    if(curr_sample > prev_sample){
    				    	break;
    				    }
    			    }

    			    for(i=0; i<2*step_count;i++){
				        MOTOR_PORT = MOVE_RIGHT_CONFIG;
    				    delay_ms(sample_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_fwd();
    				    if(curr_sample > prev_sample){
    				    	break;
    				    }

    			    }
    			    SENSOR_PORT = Read_IR();
    			}//END OF BACKWARD WHILE



    			//While loop to bring the bot to the inside of the inner circle

    			while((read_ad(2)!=1 && read_ad(3)!=1) && (read_ad(4)==1 && read_ad(5)==1)){
    			    MOTOR_PORT = BACKWARD_CONFIG;
    			    delay_ms(move_delay);
    			}




    			}

    		else{

    			update_dist_arr(dist_sample_1, dist_arr);
    		}

    		//=======================================================
    		//FOR SECOND US SENSOR
    		//=======================================================
    		if(is_dist_present(dist_sample_2, dist_arr)){
    		    while((read_ad(4)!=1 || read_ad(5)!=1) && (read_ad(2)==0 && read_ad(3)==0)){
    			//while(SENSOR_PORT != PUSH_END_CONFIG){
    		        curr_sample = Read_Ultrasonic_bwd();
    	    		while(curr_sample < = prev_sample){
    	    			MOTOR_PORT = BACKWARD_CONFIG;
    	    			delay_ms(move_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_bwd();
    	    		}
    	    		int step_count = 50;
    	    		int i;
    	    		for(i=0 ; i<step_count; i++){
    	    			MOTOR_PORT = MOVE_LEFT_CONFIG;
    	    			delay_ms(sample_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_bwd();
    	    			if(curr_sample < = prev_sample){
    	    				break;
    	    			}
    	    		}

    	    		for(i=0; i<2*step_count;i++){
    	    			MOTOR_PORT = MOVE_RIGHT_CONFIG;
    	    			delay_ms(sample_delay);
    	    			prev_sample = curr_sample;
    	    			curr_sample = Read_Ultrasonic_bwd();
    	    			if(curr_sample < = prev_sample){
    	    				break;
    	    			}

    	    		}
    			    //SENSOR_PORT = Read_IR();
    		    }//END OF FORWARD WHILE FOR MOVEMENT USING BACK ULTRASONIC SENSOR


    			while((read_ad(4)==0 && read_ad(5)==0) && (read_ad(2)!=1 && read_ad(3)!=1)){
    			//while(SENSOR_PORT != RETURN_END_CONFIG){
    			    curr_sample = Read_Ultrasonic_bwd();
    			    while(curr_sample > = prev_sample){
    				    MOTOR_PORT = FORWARD_CONFIG;
    				    delay_ms(move_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_bwd();
    			    }
    			    int step_count = 50;
    			    int i;
    			    for(i=0 ; i<step_count; i++){
    				    MOTOR_PORT = MOVE_LEFT_CONFIG;
    				    delay_ms(sample_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_bwd();
    				    if(curr_sample > prev_sample){
    				    	break;
    				    }
    			    }

			    for(i=0; i<2*step_count;i++){
    				    MOTOR_PORT = MOVE_RIGHT_CONFIG;
    				    delay_ms(sample_delay);
    				    prev_sample = curr_sample;
    				    curr_sample = Read_Ultrasonic_bwd();
    				    if(curr_sample > prev_sample){
    				    	break;
    				    }

    			    }
			    SENSOR_PORT = Read_IR();
    			}//END OF BACKWARD WHILE FOR MOVEMENT USING BACK ULTRASONIC SENSOR



    			//While loop to bring the bot to the inside of the inner circle

    			while((read_ad(4)!=1 && read_ad(5)!=1) && (read_ad(2)==1 && read_ad(3)==1)){
    			    MOTOR_PORT = FORWARD_CONFIG;
    			    delay_ms(move_delay);
    			}


    		}

    		else{
    		    update_dist_arr(dist_sample_2, dist_arr);
    		}

    	}



    	}
    return 0;
}


//Utility Functions
bool is_dist_present(int dist_sample, int dist_arr[len]){
    bool truth = false;

    int start = 0;
    int end = arr_len-1;
    int mid = (start+end)/2;

    while(1){
        if(dist_sample < *(dist_arr+mid)){
            end = mid-1;
        }

        else if(dist_sample > *(dist_arr+mid)){
            start = mid+1;
        }

        mid = (start+end)/2;

        if(*(dist_arr + mid) == dist_sample){
            truth = true;
            break;
        }

        else{
            continue;
        }
    }

    return truth
}


void update_dist_arr(int dist_sample, int dist_arr[len]){
    int i;

    for(i=0; i<len; i++){
        if(*(dist_arr+i)==0){
            *(dist_arr + i) = dist_sample;
            break;
        }
    }
}
