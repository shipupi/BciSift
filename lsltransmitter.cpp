#include <lsl_cpp.h>
#include <stdlib.h>

#include "eegimage.h"

using namespace lsl;

/**
 * This code simulates the sending of lsl EEG stream with markers.
 * *
 * /
 */


int sendeegandmarkers() {

    // make a new stream_info (128ch) and open an outlet with it
    stream_info info("openvibeSignal","EEG",11);
    stream_outlet outlet(info);

    // send data forever

    float forwardsample[11];

    while(true) {
        double ts = 1.12;

        forwardsample[0] = ts;

        // generate random data
        for (int c=0;c<11;c++)
            forwardsample[c] = (rand()%1500)/500.0-1.5;

        printf ("%10.8f:%10.8f\n",ts,forwardsample[0]);

        // send it
        outlet.push_sample(forwardsample);
    }

    return 0;
}

/**
 * This is a minimal example that demonstrates how a multi-channel stream (here 128ch)
 * of a particular name (here: SimpleStream) can be resolved into an inlet, and how the
 * raw sample data & time stamps are pulled from the inlet. This example does not
 * display the obtained data.
 * export LD_LIBRARY_PATH=/Users/rramele/work/labstreaminglayer/build/install/lsl_Release/LSL/lib/
 */

int receiving() {
    using namespace lsl;

    // resolve the stream of interest & make an inlet to get data from the first result
    std::vector<stream_info> results = resolve_stream("name","openvibeSignal");
    stream_inlet inlet(results[0]);

    // receive data & time stamps forever (not displaying them here)
    float sample[11];
    float marker;
    while (true)
    {
        double ts = inlet.pull_sample(&sample[0],11);
        printf ("%10.8f:%10.8f\n",ts,sample[0]);

        eegimage(0,sample[0]*10);
    }

    return 0;
}
