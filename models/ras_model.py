import numpy as np

class RASModel:
    def __init__(self, num_subcarriers, num_symbols, num_antennas):
        self.num_subcarriers = num_subcarriers
        self.num_symbols = num_symbols
        self.num_antennas = num_antennas
        self.channel_matrix = None
        self.noise_variance = 0.1

    def generate_channel_matrix(self):
        """Generate a random channel matrix."""
        self.channel_matrix = np.random.randn(self.num_subcarriers, self.num_antennas, self.num_antennas) + \
                              1j * np.random.randn(self.num_subcarriers, self.num_antennas, self.num_antennas)

    def apply_channel(self, signal):
        """Apply the channel matrix to the input signal."""
        if self.channel_matrix is None:
            self.generate_channel_matrix()

        output_signal = np.zeros_like(signal)
        for i in range(self.num_subcarriers):
            for j in range(self.num_symbols):
                output_signal[i, j] = np.dot(self.channel_matrix[i], signal[i, j].reshape(-1, 1)).reshape(-1)

        return output_signal

    def add_noise(self, signal):
        """Add Gaussian noise to the signal."""
        noise = np.sqrt(self.noise_variance / 2) * (np.random.randn(*signal.shape) + 1j * np.random.randn(*signal.shape))
        return signal + noise

    def equalize_signal(self, signal):
        """Perform signal equalization."""
        equalized_signal = np.zeros_like(signal)
        for i in range(self.num_subcarriers):
            channel_inv = np.linalg.inv(self.channel_matrix[i])
            for j in range(self.num_symbols):
                equalized_signal[i, j] = np.dot(channel_inv, signal[i, j].reshape(-1, 1)).reshape(-1)

        return equalized_signal