# C-Program-for-Microphone-Recording-and-Plotting

The C++ code utilizes the PortAudio library to record audio from the default microphone input, saves the recorded samples to a text file, and plots the recorded sound waveform using GNUPlot, providing a comprehensive solution for capturing and visualizing audio data.

● Audio Recording: The code utilizes the PortAudio library to capture audio from the
default microphone input on the computer. It sets parameters such as the sample rate,
number of channels (mono recording), and sample block size to configure the audio
stream. A callback function records incoming audio samples into a vector in real-time.

● Data Storage: Once the recording duration specified by `RECORDING_TIME_SEC`
elapses, the audio samples are saved to a text file named `recorded_samples.txt`. Each
sample is written to a new line in the file, enabling easy retrieval and processing of the
recorded data.

● Plotting with GNUPlot: After saving the recorded samples, the code invokes GNUPlot as
a subprocess to generate a plot of the recorded sound. It communicates with GNUPlot via
a pipe, passing commands to set plot settings and plot the data from the saved file. This
allows for visualizing the recorded sound waveform directly from the C++ program.

