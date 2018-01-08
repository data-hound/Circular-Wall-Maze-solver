/*
  done - UTILITY FUNCTIONS ARE TO BE IMPLEMENTED
  THE CASE FOR BLOCKS LYING ALONG SAME DIRECTION ARE TO BE INCLUDED
  THE CONDITION OF RETURNING BACK TO THE CENTRE IS TO BE INCLUDED
  THE CONDITION FOR INDICATION OF PUSHING A BLOCK OUT IS TO BE INCLUDED
  TEST THE CONDITION WHEN WE HAVE TO PUSH THE BLOCK A DISTANCE INTO THE OUTER WHITE
*/

#include<stdio.h>
#include<math.h>
#define len 10
#define width 4


//Check if the definitions are consistent with usecases**
//bool is_dist_present(int dist_sample, int dist_arr[len][width], int arr_len);
void fill_dist_arr(int dist_arr[len][width], int dist_sample, int flag, int angle_sample, int to_be_sampled);//int len_diff_error);

//This function updates the true_angle and true_dist for the new peripheral position of the bot
void update_dist_arr(int dist_arr[len][width], int idx, );

//This function returns the angle to be rotated at a block to allign the bot in a radial direction at the block
int getRectification(int cent_angle, int cent_angle_pred, int true_angle);

//This function returns the angle to be rotated to allign along the next block when bot is at periphery
int getRotAngle(int true_angle, int cent_angle, int cent_angle_next);

//This function sorts the dist_arr in decreasing order of distance
void sort(dist_arr[len][width]);

int main(){

    int dist_limit = 75;
    int dist_arr[len][width];
    dist_arr={0};
    //dist_arr[10][4] is the size of dist_arr
    //dist_arr[][0]-stores the radial distance of the block
    //dist_arr[][1]-stores the central angle of the block
    //dist_arr[][2]-stores the angle from current location to the block
    //dist_arr[][3]-stores the dist from current location to the block
    int count =10;
    int sample_delay = 10;
    int move_delay = 100;
    int MOTOR_PORT=0xFF;
    int MOVE_LEFT_CONFIG=0xFF;
    int MOVE_RIGHT_CONFIG=0xFF;
    int SENSOR_PORT = 0xFF;
    int PUSH_END_CONFIG = 0xFF;
    int RETURN_END_CONFIG = 0xFF;
    int state = 0;
    //state = 0 => Sampling State
    //state = 1 => Sort the array
    //state = 2 => Motion State
    int to_be_sampled = len;
    int to_be_pushed = len;
    int sample_angle_count = 0; //to keep a record of the angle while sampling
    int degree_per_sample = 5;
    int angle_error = 5;
    //while rotating the bot to a certain angle, we first move an angle less than the actual angle
    //The difference between true_angle and the angle rotated is the angle_error
    //The angle_error should be large enough that the bot must not move beyond the block's true_angle in any cicumstance
    int dist_error = 5;
    //The tolerance limit of distance measures

    while(1){

        //======================================================
        //FIRST WE SAMPLE ALL THE BLOCKS USING 2 SENSORS
        //======================================================
        if(state==0){

          int flag;
          int dist_sample_fnt;//front US
          int dist_sample_bck;//back US

            //First we move around and sample with both the US sensors.
            //We sample both distance and angle from the reference
            //dist_arr[i][0]-> value of distance of the i-th block
            //dist_arr[i][1]-> value of angle of the i-th block from the reference

          MOTOR_PORT = MOVE_LEFT_CONFIG;
    	    delay_ms(sample_delay);
    	    sample_angle_count+= degree_per_sample;

          dist_sample_fnt = Read_Ultrasonic_f();//front US
    	    dist_sample_bck = Read_Ultrasonic_b();//back US

    	    if(dist_sample_fnt < dist_limit && to_be_sampled > 0){
    	        flag = 0;
    	        fill_dist_arr(dist_arr, dist_sample_fnt, flag, sample_angle_count, to_be_sampled);
    	        to_be_sampled--;
    	    }

    	    if(dist_sample_bck < dist_limit && to_be_sampled > 0){
    	        flag = 1;
    	        fill_dist_arr(dist_arr, dist_sample_bck, flag, sample_angle_count, to_be_sampled);
    	        to_be_sampled--;
    	    }

    	    if(to_be_sampled == 0){
    	        state = 1;

    	    }

        }//END OF SAMPLING

        //======================================================
        //NEXT WE PROCESS ALL THE BLOCKS AND ANGLES
        //======================================================
        if(state == 1){
          sort(dist_arr);
          state = 2;
        }


        //We assume the bot rotates only in ACW direction for angle measurements
        //=> THE BOT ALWAYS ROTATES LEFT IN ORDER TO FIND THE BLOCKS
        if(state==2){
          int rot_angle_count = 0;

          int rad_dist = dist_arr[len - to_be_pushed][0];
          int cent_angle = dist_arr[len - to_be_pushed][1];

          int true_dist = dist_arr[len - to_be_pushed][3];
          int true_angle = dist_arr[len - to_be_pushed][2];

          int cent_angle_pred = 0;

          int cent_angle_next = (to_be_pushed > 1) ? dist_arr[len - to_be_pushed+1][1] : 0;

          if(len - to_be_pushed > 0){
            cent_angle_pred = dist_arr[len - to_be_pushed-1][1];
          }

          //If the bot is going for its first push, then, it needs to rotate only true_angle = cent_angle
          //But if bot is going for pushing a block from periphery, it needs to rotate (180 - true_angle)
          //This is bcz, after a push, the bot would be facing radially outwards
          //getRotAngle returns (180 - true_angle) if next block is on left side of bot
          //else returns (180 + true_angle) if next block on right side of bot
          int rot_angle_delay;
          if(len - to_be_pushed){
             rot_angle_delay= getRotAngle(true_angle, cent_angle, cent_angle_next) * sample_delay - angle_error;
          }

          else{
             rot_angle_delay= true_angle * sample_delay - angle_error;
          }



          MOTOR_PORT = MOVE_LEFT_CONFIG;
    	    delay_ms(rot_angle_delay);

          int dist_sample_fnt = Read_Ultrasonic_f();//front US
          int dist_sample_bck = Read_Ultrasonic_b();//back US

          //Locate the exact location of  block within tolerance limit of distance
          while((dist_sample_fnt <= (true_dist + dist_error) && dist_sample_fnt >= (true_dist - dist_error)))// || (dist_sample_bck <= (true_dist + dist_error) && dist_sample_bck >= (true_dist - dist_error)))
          {
            MOTOR_PORT = MOVE_LEFT_CONFIG;
      	    delay_ms(sample_delay);
      	    rot_angle_count += degree_per_sample;
            dist_sample_fnt = Read_Ultrasonic_f();
            //dist_sample_bck = Read_Ultrasonic_b();
          }

          //If the block is in the range of front sensor, move front till you reach the block
          if((dist_sample_fnt <= (true_dist + dist_error) && dist_sample_fnt >= (true_dist - dist_error))){
            prev_sample = dist_sample_fnt;
            curr_sample = Read_Ultrasonic_f();

  	    		while(curr_sample < prev_sample){
  	    			MOTOR_PORT = FORWARD_CONFIG;
  	    			delay_ms(move_delay);
  	    			prev_sample = curr_sample;
  	    			curr_sample = Read_Ultrasonic_f();
  	    		}

  	    		int step_count = 50;
  	    		int i;

  	    		for(i=0 ; i<step_count; i++){
  	    			MOTOR_PORT = MOVE_LEFT_CONFIG;
  	    			delay_ms(sample_delay);
  	    			prev_sample = curr_sample;
  	    			curr_sample = Read_Ultrasonic_f();
  	    			if(curr_sample < prev_sample){
  	    				break;
  	    			}
  	    		}

  	    		for(i=0; i<2*step_count;i++){
  	    			MOTOR_PORT = MOVE_RIGHT_CONFIG;
  	    			delay_ms(sample_delay);
  	    			prev_sample = curr_sample;
  	    			curr_sample = Read_Ultrasonic_f();
  	    			if(curr_sample < prev_sample){
  	    				break;
  	    			}

  	    		}
          }

          //If the block is in the range of back sensor, move back till you reach the block
          /*if((dist_sample_bck <= (true_dist + dist_error) && dist_sample_bck >= (true_dist - dist_error))){
            prev_sample = dist_sample_bck;
            curr_sample = Read_Ultrasonic_b();

            while(curr_sample < prev_sample){
              MOTOR_PORT = FORWARD_CONFIG;
              delay_ms(move_delay);
              prev_sample = curr_sample;
              curr_sample = Read_Ultrasonic_b();
            }

            int step_count = 50;
            int i;

            for(i=0 ; i<step_count; i++){
              MOTOR_PORT = MOVE_LEFT_CONFIG;
              delay_ms(sample_delay);
              prev_sample = curr_sample;
              curr_sample = Read_Ultrasonic_b();
              if(curr_sample < prev_sample){
                break;
              }
            }

            for(i=0; i<2*step_count;i++){
              MOTOR_PORT = MOVE_RIGHT_CONFIG;
              delay_ms(sample_delay);
              prev_sample = curr_sample;
              curr_sample = Read_Ultrasonic_b();
              if(curr_sample < prev_sample){
                break;
              }

            }
          }

          */

          //If the bot has approached the block in a radial direction, push along that direction till the end of circle
          if(true_angle == cent_angle){
            prev_sample = dist_sample_fnt;
            curr_sample = Read_Ultrasonic_f();

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
      	    			curr_sample = Read_Ultrasonic_f();
      	    		}

      	    		//This variable is used for determining the angle the bot would rotate for reconfiguration
      	    		//If it deviates from straight path => The angular range the bot should check to reallign itself

      	    		int step_count = 50;
      	    		int i;
      	    		for(i=0 ; i<step_count; i++){
      	    			MOTOR_PORT = MOVE_LEFT_CONFIG;
      	    			delay_ms(sample_delay);
      	    			prev_sample = curr_sample;
      	    			curr_sample = Read_Ultrasonic_f();
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
      	    			curr_sample = Read_Ultrasonic_f();
      	    			if(curr_sample < = prev_sample){
      	    				break;
      	    			}

      	    		}
      	    		//This for loop is used to scan to the right of the point
      	    		//=====USED WHEN BOT DEVIATES TO THE LEFT OF PATH=======

      			    //SENSOR_PORT = Read_IR();

      		    }//END OF FORWARD WHILE
          }

          else{
            int rectify_angle = getRectification(cent_angle, cent_angle_pred, true_angle);
            //After reaching the block, the bot needs to turn this angle in order to get in the radial direction

            rectify_angle_delay = rectify_angle * sample_delay;

            MOTOR_PORT = MOVE_LEFT_CONFIG;
      	    delay_ms(rectify_angle_delay);

            prev_sample = dist_sample_fnt;
            curr_sample = Read_Ultrasonic_f();

            //while(SENSOR_PORT != PUSH_END_CONFIG) we move the bot forward
      			//PUSH_END_CONFIG occurs when both the front IR sensors cross into the white circle AND
      			//both the back IR sensors are still in the black circle region
            while((read_ad(2)!=1 || read_ad(3)!=1) && (read_ad(4)==0 && read_ad(5)==0)){
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

          }//end of IF-ELSE

          }

          //Now the bot has reached the periphery of the black annulus
          //We may need to push the block out into the white circle for some distance now, depending on the
          //resolution of the sensors

          //==============================================================================================
          //====CODE TO PUSH THE BLOCK A LENGTH EQUAL TO THE LENGTH OF THE BOT OUT INTO WHITE CIRCLLE=====
          //==============================================================================================

          while((read_ad(4)!=0 && read_ad(5)!=0)){
            curr_sample = Read_Ultrasonic_fwd(); //We take a distance sample from the block before
            //Beginning to push it.

           //This while runs till the bot moves straight in the direction of block
           while(curr_sample < = prev_sample){
             MOTOR_PORT = FORWARD_CONFIG;
             delay_ms(move_delay);
             prev_sample = curr_sample;
             curr_sample = Read_Ultrasonic_fwd();
           }

         }//END OF PUSH-OUT WHILE



         //====================================================================
         //==============ADD CODE TO GLOW LED/ SOUND BUZZER====================
         //====================================================================

         //state = 3;-> STATE 3 WAS SUPPOSED TO CONTAIN CODE TO

         update_dist_arr(dist_arr,(len - to_be_pushed));

         to_be_pushed--; //One block has been pushed out into the white circle.

         //====================================================================
         //=======IF LAST BLOCK THEN REVERT BACK TO CENTRE AND THEN STOP=======
         //============FOR AN ARBITRARY AMOUNT OF TIME=========================
         //================ADD CODE FOR THIS==================================
         //====================================================================



        }




    }//END OF INFINITE WHILE

    return 0;

}// END OF MAIN


//dist_arr[10][4] is the size of dist_arr
//dist_arr[][0]-stores the radial distance of the block
//dist_arr[][1]-stores the central angle of the block
//dist_arr[][2]-stores the angle from current location to the block
//dist_arr[][3]-stores the dist from current location to the block

//the variable len_diff_error is used to account for difference in distance of fwd sensor and bwd sensor from centre of rotation
void fill_dist_arr(int dist_arr[len][width], int dist_sample, int flag, int angle_sample, int to_be_sampled){ //int len_diff_error){

  int pos;

  //For FWD Sensor
  if(flag == 0){
    pos = len - to_be_sampled;
    dist_arr[pos][0] = dist_sample;
    dist_arr[pos][1] = angle_sample;
    dist_arr[pos][2] = angle_sample;
    dist_arr[pos][3] = dist_sample;
  }

  //For BWD Sensor
  else{
    pos = to_be_sampled;
    dist_arr[pos][0] = dist_sample;
    dist_arr[pos][1] = angle_sample + 180;
    dist_arr[pos][2] = angle_sample + 180;
    dist_arr[pos][3] = dist_sample;
  }

}


void sort(int dist_arr[len][width]) {
  /* code */
  int i,j;

  for(i=0; i < len - 1; i++){
    for(j=i +1 ; j < len; j++){

      if(dist_arr[i][0] < dist_arr[j][0]){
        int temp0 = dist_arr[j][0];
        int temp1 = dist_arr[j][1];
        int temp2 = dist_arr[j][2];
        int temp3 = dist_arr[j][3];

        dist_arr[j][0] = dist_arr[i][0];
        dist_arr[j][1] = dist_arr[i][1];
        dist_arr[j][2] = dist_arr[i][2];
        dist_arr[j][3] = dist_arr[i][3];

        dist_arr[i][0] = temp0;
        dist_arr[i][1] = temp1;
        dist_arr[i][2] = temp2;
        dist_arr[i][3] = temp3;

      }
    }
  }

}

//We need to compute:
//T - the distance of bot on periphery to a block
//A - Acute angle to turn at the periphery
void update_dist_arr(int dist_arr[len][width], int idx){
  dist_arr[idx][3] = 200;
  //float A,T;
  float R =

  int i = idx;

  int prev_cent_angle = dist_arr[idx][1];
  float prev_cent_degress = prev_cent_angle;

  for(i+=1;i<len;i++){
    if(dist_arr[i][3] < 200){
      float current_degrees = dist_arr[i][1];
      float diff_degrees = current_degrees - prev_cent_degress;
      float theta;
      float A;
      float D2 = (float)dist_arr[i][0];
      float t;

      if(diff_degrees > 180){
        theta = 360 - diff_degrees;
      }

      else{
        theta = diff_degrees;
      }

      t = sqrt( (pow(D2,2)) + (pow(R,2)) + 2*R*D2*cos(theta));
      dist_arr[i][3] = (int)t;

      A = acos( ( pow(D2,2) - ( pow(R,2) + pow(t,2) ) ) / ( -2 * R * t) );
      dist_arr[i][2] = (int)A;

    }
  }



}


int getRectification(int cent_angle, int cent_angle_pred, int true_angle){
  int diff_degrees = cent_angle - cent_angle_pred;
  int theta;

  if(diff_degrees > 180){
    theta = 360 - diff_degrees;
  }

  else{
    theta = diff_degrees;
  }

  return ( 180 - (theta + true_angle));

}

int getRotAngle(int true_angle, int cent_angle, int cent_angle_next){

  if(cent_angle_next == 0){
    return 180;
  }

  else{
    int diff = cent_angle_next - cent_angle;

    if(diff < 180){
      return (180 - true_angle);
    }

    else{
      return (180 + true_angle);
    }

  }

}
