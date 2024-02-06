#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>

// Define parameters
#define SAMPLE_RATE         (48000)
#define NUM_CHANNELS        (1)     // Mono recording
#define SAMPLE_BLOCK_SIZE   (1024)  // Size of each audio buffer
#define RECORDING_TIME_SEC  (4.5)   // Recording time in seconds

// Function to plot the recorded samples using GNUPlot
void plotRecordedSamples() {
    // Open a pipe to GNUPlot
    FILE *gnuplotPipe = popen("gnuplot -persist", "w");
    if (!gnuplotPipe) {
        std::cerr << "Error opening GNUPlot pipe." << std::endl;
        return;
    }

    // Set GNUPlot settings and commands
    fprintf(gnuplotPipe, "set title 'Sampled Recorded Audio'\n");
    fprintf(gnuplotPipe, "set xlabel 'Sample Index'\n");
    fprintf(gnuplotPipe, "set ylabel 'Amplitude'\n");
    fprintf(gnuplotPipe, "plot 'recorded_samples.txt' with lines\n");

    // Close the pipe to GNUPlot
    pclose(gnuplotPipe);
}

// Callback function for recording
static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags,
                          void *userData)
{
    std::vector<float> *samples = static_cast<std::vector<float>*>(userData);
    const float *in = static_cast<const float*>(inputBuffer);

    for (unsigned int i = 0; i < framesPerBuffer; i++) {
        samples->push_back(*in++);
    }

    return paContinue;
}

int main()
{
    // Initialize PortAudio
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Open an audio stream for recording
    PaStream *stream;
    std::vector<float> samples;
    err = Pa_OpenDefaultStream(&stream, NUM_CHANNELS, 0, paFloat32, SAMPLE_RATE,
                               SAMPLE_BLOCK_SIZE, recordCallback, &samples);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Start the audio stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    // Wait for recording to finish
    Pa_Sleep(RECORDING_TIME_SEC * 1000);

    // Stop and close the audio stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }
    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
    }

    // Terminate PortAudio
    Pa_Terminate();

    // Save recorded samples to a file
    std::ofstream outFile("recorded_samples.txt");
    if (outFile.is_open()) {
        for (float sample : samples) {
            outFile << sample << std::endl;
        }
        outFile.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
        return 1;
    }

    std::cout << "Recording saved to recorded_samples.txt" << std::endl;

    // Plot the recorded samples using GNUPlot
    plotRecordedSamples();

    return 0;
}
