import pyaudio
import numpy as np
import matplotlib.pyplot as plt

from scipy.fftpack import fft
import matplotlib.animation as animation


CHUNK = 1024  # signal is split into CHUNK number of frames
FORMAT = pyaudio.paInt16
CHANNELS = 2
RATE = 44100  # (sampling rate) number of frames per second
AMPLITUDE = 2 ** 16 / 2

p = pyaudio.PyAudio()

stream = p.open(format=FORMAT,
                channels=CHANNELS,
                rate=RATE,
                input=True,
                frames_per_buffer=CHUNK)


print("Recording...")

fig, (axis1, axis2) = plt.subplots(2)
axis1.set_ylim(-AMPLITUDE, AMPLITUDE)
axis2.set_ylim(0, 1)

x = np.arange(0, CHUNK * CHANNELS)
x_fft = np.linspace(0, RATE, CHUNK)

line, = axis1.plot(x, np.random.randn(CHUNK * CHANNELS))
line_fft, = axis2.semilogx(x_fft, np.random.randn(CHUNK))

axis2.set_xlim(1, RATE/2)


def update(frame):
    data = np.frombuffer(stream.read(CHUNK, exception_on_overflow=False), dtype=np.int16)
    line.set_data(x, data)

    y_fft = fft(data)

    # 1 / AMPLITUDE * CHUNK == 2 / (AMPLITUDE * 2) * CHUNK
    line_fft.set_ydata(np.abs(y_fft[0:(CHUNK)]) * 1/(AMPLITUDE * CHUNK))


animation = animation.FuncAnimation(fig, update, interval=60)
plt.show()