#include<stdio.h>
#define len 6
void update_dist_arr(int *dist_arr)

int is_dist_present(int dist_sample, int dist_arr[len])
void update_dist_arr(int dist_sample, int dist_arr[len])

int main(){

int dist_limit = 75;
int dist_arr[len];
dist_arr={0};
int count = len;
int to_be_pushed = len;
int sample_delay = 10;
int move_delay = 100;
int MOTOR_PORT=0xFF;
int MOVE_LEFT_CONFIG=0xFF;
int MOVE_RIGHT_CONFIG=0xFF;
int SENSOR_PORT = 0xFF;
int PUSH_END_CONFIG = 0xFF;
int RETURN_END_CONFIG = 0xFF;
int STOP_CONFIG = 0xFF;

while(to_be_pushed){
	MOTOR_PORT = MOVE_LEFT_CONFIG;
	delay_ms(sample_delay);
	float dist_sample = Read_Ultrasonic();
	float prev_sample = dist_sample;
	float curr_sample;
	if(dist_sample < dist_limit){
		if(is_dist_present(dist_sample, dist_arr){
		    //adc[0]=Ultrasonic Forward
		    //adc[1]=Ultrasonic Backward
		    //adc[2]=Left Forward IR
		    //adc[3]=Right Forward IR
		    //adc[4]=Left Backward IR
		    //adc[5]=Right Backwar IR
		    while((read_ad(2)!=1 || read_ad(3)!=1) && (read_ad(3)==0 && read_ad(4)==0)){
			//while(SENSOR_PORT != PUSH_END_CONFIG){
		      curr_sample = Read_Ultrasonic();
					int step_count = 50;
	    		int i;

	    		while(curr_sample < = prev_sample){
	    			MOTOR_PORT = FORWARD_CONFIG;
	    			delay_ms(move_delay);
						MOTOR_PORT = STOP_CONFIG;
	    			prev_sample = curr_sample;
	    			curr_sample = Read_Ultrasonic();
	    		}


	    		for(i=0 ; i<step_count; i++){
	    			MOTOR_PORT = MOVE_LEFT_CONFIG;
	    			delay_ms(sample_delay);
						MOTOR_PORT = STOP_CONFIG;
	    			prev_sample = curr_sample;
	    			curr_sample = Read_Ultrasonic();
	    			if(curr_sample < = prev_sample){
							break;
	    			}
	    		}

	    		for(i=0; i<2*step_count;i++){
	    			MOTOR_PORT = MOVE_RIGHT_CONFIG;
	    			delay_ms(sample_delay);
						MOTOR_PORT = STOP_CONFIG;
	    			prev_sample = curr_sample;
	    			curr_sample = Read_Ultrasonic();
	    			if(curr_sample < = prev_sample){
	    				break;
	    			}

	    		}
			    //SENSOR_PORT = Read_IR();
		    }//END OF FORWARD WHILE

				to_be_pushed -- ;


			while((read_ad(2)==0 && read_ad(3)==0) && (read_ad(3)!=1 && read_ad(4)!=1)){
			//while(SENSOR_PORT != RETURN_END_CONFIG){
			    curr_sample = Read_Ultrasonic();
					int step_count = 50;
			    int i;

			    while(curr_sample > = prev_sample){
				    MOTOR_PORT = BACKWARD_CONFIG;
				    delay_ms(move_delay);
						MOTOR_PORT = STOP_CONFIG;
				    prev_sample = curr_sample;
				    curr_sample = Read_Ultrasonic();
			    }

			    for(i=0 ; i<step_count; i++){
				    MOTOR_PORT = MOVE_LEFT_CONFIG;
				    delay_ms(sample_delay);
						MOTOR_PORT = STOP_CONFIG;
				    prev_sample = curr_sample;
				    curr_sample = Read_Ultrasonic();
				    if(curr_sample > prev_sample){
				    	break;
				    }
			    }

			    for(i=0; i<2*step_count;i++){
				    MOTOR_PORT = MOVE_RIGHT_CONFIG;
				    delay_ms(sample_delay);
						MOTOR_PORT = STOP_CONFIG;
				    prev_sample = curr_sample;
				    curr_sample = Read_Ultrasonic();
				    if(curr_sample > prev_sample){
				    	break;
				    }

			    }
			    SENSOR_PORT = Read_IR();
			}//END OF BACKWARD WHILE

			//While loop to bring the bot to the inside of the inner circle

			while((read_ad(2)!=1 && read_ad(3)!=1) && (read_ad(3)==1 && read_ad(4)==1)){
			    MOTOR_PORT = BACKWARD_CONFIG;
			    delay_ms(move_delay);
					MOTOR_PORT = STOP_CONFIG;
			}




			}

		else{
			update_dist_arr(dist_sample, dist_arr );
		}
	}
	}
return 0;
}

int is_dist_present(int dist_sample, int dist_arr[len]){
    bool truth = 0;

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
            truth = 1;
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
