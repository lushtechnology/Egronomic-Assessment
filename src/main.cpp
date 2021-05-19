#include "aggregator.h"
#include "ble.h"
#include "ei_run_classifier.h"
#include <drivers/sensor.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/printk.h>
#include <zephyr.h>

#include "ble.h"

static int64_t sampling_freq = EI_CLASSIFIER_FREQUENCY; // in Hz.
static int64_t time_between_samples_us = (1000000 / (sampling_freq - 1));

// to classify 1 frame of data you need EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE values
static float features[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];

int main() {
  // output immediately without buffering
  setvbuf(stdout, NULL, _IONBF, 0);

  printk("BLE Sensor Gateway sample started\n");

  // C Function call
  ble_init();

  struct sensor_data out_data;

        while (1) {

          size_t ix = 0;

          while (1) {

                  if (aggregator_get(&out_data) == 0) {
                   float orientation[3];

                    for (int i = 0; i < 12; i = i + 4) {

                            char byte_array[4] = {out_data.bytes[i], out_data.bytes[i + 1], out_data.bytes[i + 2], out_data.bytes[i + 3]};

                            memcpy(&orientation[i], byte_array, sizeof orientation[i]);

                            //printf("value %f\n", orientation[i]);

                           
                    }

                    // fill the features array
                    features[ix + 0] = orientation[0];
                    features[ix + 1] = orientation[1];
                    features[ix + 2] = orientation[2];

                     //printk("Filling feature array.\n");

                    ix += EI_CLASSIFIER_RAW_SAMPLES_PER_FRAME;

                    if (ix == EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE){ break;}

                  }else {

                    //printk("Was not able to get Fruit orientation data from aggregator.\n");
                    
                  }

                  k_sleep(K_MSEC(10));
                   
          } 

          // frame full? then classify
          ei_impulse_result_t result = {0};

          // create signal from features frame
          signal_t signal;
          numpy::signal_from_buffer(features, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);

          // run classifier
          EI_IMPULSE_ERROR res = run_classifier(&signal, &result, false);
          printf("run_classifier returned: %d\n", res);
          if (res != 0)
            return 1;

          // print predictions
          printf("Predictions (DSP: %d ms., Classification: %d ms., Anomaly: %d ms.): \n",
              result.timing.dsp, result.timing.classification, result.timing.anomaly);

          // print the predictions
          for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
            printf("%s:\t%.5f\n", result.classification[ix].label, result.classification[ix].value);
          }
  
          if (result.classification[0].value > 0.5) {
          
            int x = 1;
            orientation_notify(x);
          }

          else{

            int x = 0;
            orientation_notify(x);
          }

      #if EI_CLASSIFIER_HAS_ANOMALY == 1
          printf("anomaly:\t%.3f\n", result.anomaly);
      #endif

         
        }

       
}