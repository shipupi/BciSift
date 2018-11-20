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

    const double markertypes[] = {32773.0, 32774.0, 33025.0, 33026.0, 33027.0, 33028.0,33029.0,33030.0,33031.0,
                                 33032.0,33033.0,33034.0,33035.0,33036.0,33037.0};

    // make a new stream_info (128ch) and open an outlet with it
    stream_info info("openvibeSignal2","EEG",8);
    stream_outlet outleteeg(info);

    // make a new stream_info
    lsl::stream_info infomk("openvibeMarkers2","Markers",1,lsl::IRREGULAR_RATE,lsl::cf_double64,"myuniquesourceid23443");

    // make a new outlet
    lsl::stream_outlet outletmk(infomk);


    // send data forever

    float forwardsample[8];

    while(true) {
        double ts = 1.12;

        forwardsample[0] = ts;

        // generate random data
        for (int c=0;c<8;c++)
            forwardsample[c] = (rand()%1500)/500.0-1.5;

        printf ("%10.8f\n",forwardsample[0]);

        // send it
        outleteeg.push_sample(forwardsample);
    }

    return 0;
}

int sendeeg() {

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
        double signal[256];
        for(int i=0;i<256;i++)
        {
            double ts = inlet.pull_sample(&sample[0],11);
            printf ("%10.8f:%10.8f\n",ts,sample[0]);
            signal[i]=sample[0];
        }

        eegimage(signal,256,1,1);
    }


    while (true)
    {
        double ts = inlet.pull_sample(&sample[0],11);
        printf ("%10.8f:%10.8f\n",ts,sample[0]);

        eegimage(0,sample[0]*10);
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

int receivingsignal() {
    using namespace lsl;

    // resolve the stream of interest & make an inlet to get data from the first result
    std::vector<stream_info> markers = resolve_stream("name","openvibeMarkers2");
    stream_inlet markersInlet(markers[0]);

    std::vector<stream_info> results = resolve_stream("name","openvibeSignal2");
    stream_inlet inlet(results[0]);

    // receive data & time stamps forever (not displaying them here)
    float sample[8];
    float marker;
    while (true)
    {
        double signal[256];
        for(int i=0;i<256;i++)
        {
            double ts = inlet.pull_sample(&sample[0],8);
            //printf ("%10.8f:%10.8f\n",ts,sample[0]);
            signal[i]=sample[0];

            double mts = markersInlet.pull_sample(&marker,1,0.0f);
            if (mts>0)
                printf ("%10.8f:%10.8f:Marker %10.8f\n",ts,mts,marker);
        }

        eegimage(signal,256, 10,1);

    }

    return 0;
}


void processtrial(double gammat, double Fs, stream_inlet &inlet, stream_inlet &markersInlet)
{
    // receive data & time stamps forever (not displaying them here)
    float sample[8];
    float marker;

    double Segments[15][12][256];
    int counters[12];
    double signal[256*400];
    int stims[130];
    int stimmarkers[130];
    int stimcounter=0;

    double tsoffset=0;

    memset(counters,0,12*sizeof(int));
    memset(signal,0,256*400*sizeof(double));
    memset(Segments,0,15*12*256*sizeof(double));

    while(true)
    {
        double mts = markersInlet.pull_sample(&marker,1,0.0f);
        if (mts>0)
        {
            // Trial start
            if (((int)marker)==32773)
            {
                printf("\nTrial start triggered.");
                tsoffset = mts;
                break;
            }
        }
    }
    for(int i=0;i<256*400;i++)
    {
        double ts = inlet.pull_sample(&sample[0],8);
        //printf ("%10.8f:%10.8f\n",ts,sample[0]);
        signal[i]=sample[0];

        double mts = markersInlet.pull_sample(&marker,1,0.0f);
        if (mts>0)
        {
            printf ("\n%10.8f:%10.8f:Marker %10.8f",ts,mts,marker);
            if (marker >= 33025 && marker <= 33037)
            {
                int stim = (int)marker - 33025;
                printf ("- Stim: %d.",ts,mts,marker,stim);
                stims[stimcounter]=stim;
                stimmarkers[stimcounter] = (int)((mts-tsoffset)*Fs*gammat);
                stimcounter++;
            }
            // Trial stop
            if (((int)marker)==32774)
            {
                printf("\nEnd of Trial triggered.");
                break;
            }
        }

        if (stimcounter>=160) break;
    }


    for(int i=0;i<stimcounter;i++)
    {
        int stim = stims[i];
        printf("\nCopying segment %d into position %d on repetition %d.",stim, stimmarkers[i],counters[stim]);
        //signal[stimmarkers[i] + 200] = 50;signal[stimmarkers[i] + 220] = -5;
        //memcpy(Segments[counters[stim]++][stim],&signal[stimmarkers[i]],256*sizeof(double));

        for(int j=0;j<256;j++)
        {
            Segments[counters[stim]][stim][j] = signal[stimmarkers[i]+j];
        }
        counters[stim]++;

    }


    for(int i=0;i<12;i++)
    {
        printf("\nCounter for stim %d:%d",i,counters[i] );

        double sign[256];

        for(int j=0;j<256;j++)
        {
            double avg = 0;
            for(int rep=0;rep<counters[i];rep++)
            {
                avg += Segments[rep][i][j];
            }
            sign[j] = avg/(counters[i]*1.0F);
        }

        eegimage(sign,256, 10,i);

    }
}

int receivingtrial() {
    using namespace lsl;

    // resolve the stream of interest & make an inlet to get data from the first result
    std::vector<stream_info> markers = resolve_stream("name","openvibeMarkers2");
    stream_inlet markersInlet(markers[0]);

    std::vector<stream_info> results = resolve_stream("name","openvibeSignal2");
    stream_inlet inlet(results[0]);

    double Fs = 256;
    double gammat  =1;

    while (true)
    {
        processtrial(gammat, Fs, inlet,markersInlet);

        //return 0;


    }

    return 0;
}

