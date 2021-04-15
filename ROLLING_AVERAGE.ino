//https://gist.github.com/bmccormack/d12f4bf0c96423d03f82
//https://gist.github.com/mrfaptastic/3fd6394c5d6294c993d8b42b026578da

#define ROLLING_AVERAGE_WINDOW 6 

// CT Sensor (analogue read) - Moving average
float     maverage_values[ROLLING_AVERAGE_WINDOW] = {0}; // all are zero as a start
int       maverage_current_position  = 0;
float     maverage_current_sum = 0; //long
int       maverage_sample_length = sizeof(maverage_values) / sizeof(maverage_values[0]);

//...........................................................................................................................
//
//...........................................................................................................................
float RollingmAvg(float newValue)
{
         //Subtract the oldest number from the prev sum, add the new number
        maverage_current_sum = maverage_current_sum - maverage_values[maverage_current_position] + newValue;

        //Assign the newValue to the position in the array
        maverage_values[maverage_current_position] = newValue;

        maverage_current_position++;
        
        if (maverage_current_position >= maverage_sample_length) { // Don't go beyond the size of the array...
            maverage_current_position = 0;
        }
                
        //return the average
        return maverage_current_sum / maverage_sample_length;

}
