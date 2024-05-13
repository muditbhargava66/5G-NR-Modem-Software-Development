import numpy as np

class RASModel:
    def __init__(self, num_subcarriers, num_symbols, num_antennas, channel_type, modulation_type):
        self.num_subcarriers = num_subcarriers
        self.num_symbols = num_symbols
        self.num_antennas = num_antennas
        self.channel_matrix = None
        self.noise_variance = 0.1
        self.channel_type = channel_type
        self.modulation_type = modulation_type

    def generate_random_signal(self):
        """Generate a random input signal based on the modulation type."""
        if self.modulation_type == 'qpsk':
            # Generate QPSK symbols
            signal = np.random.choice([-1, 1], size=(self.num_subcarriers, self.num_symbols, self.num_antennas))
        elif self.modulation_type == '16qam':
            # Generate 16-QAM symbols
            signal = np.random.choice([-3, -1, 1, 3], size=(self.num_subcarriers, self.num_symbols, self.num_antennas))
        elif self.modulation_type == '64qam':
            # Generate 64-QAM symbols
            signal = np.random.choice([-7, -5, -3, -1, 1, 3, 5, 7], size=(self.num_subcarriers, self.num_symbols, self.num_antennas))
        else:
            raise ValueError(f"Invalid modulation type: {self.modulation_type}")

        return signal

    def generate_channel_matrix(self):
        """Generate a random channel matrix based on the channel type."""
        np.random.seed(42)  # Set a fixed random seed for reproducibility

        if self.channel_type == 'rayleigh':
            # Generate Rayleigh fading channel coefficients
            self.channel_matrix = np.random.rayleigh(scale=1.0, size=(self.num_subcarriers, self.num_antennas, self.num_antennas))
        elif self.channel_type == 'rician':
            # Generate Rician fading channel coefficients
            k_factor = 5  # Rician K-factor
            mean = np.sqrt(k_factor / (k_factor + 1))
            std_dev = np.sqrt(1 / (2 * (k_factor + 1)))
            self.channel_matrix = np.random.normal(loc=mean, scale=std_dev, size=(self.num_subcarriers, self.num_antennas, self.num_antennas))
        else:
            raise ValueError(f"Invalid channel type: {self.channel_type}")

    def apply_channel(self, signal):
        """Apply the channel matrix to the input signal."""
        if self.channel_matrix is None:
            self.generate_channel_matrix()

        output_signal = np.zeros((self.num_subcarriers, self.num_symbols, self.num_antennas), dtype=complex)
        for i in range(self.num_subcarriers):
            for j in range(self.num_symbols):
                output_signal[i, j] = np.dot(self.channel_matrix[i], signal[i, j])

        return output_signal

    def add_noise(self, signal):
        """Add Gaussian noise to the signal."""
        noise = np.sqrt(self.noise_variance / 2) * (np.random.randn(*signal.shape) + 1j * np.random.randn(*signal.shape))
        return signal + noise

    def equalize_signal(self, signal):
        """Perform signal equalization."""
        equalized_signal = np.zeros_like(signal)
        for i in range(self.num_subcarriers):
            for j in range(self.num_symbols):
                channel_inv = np.linalg.inv(self.channel_matrix[i])
                equalized_signal[i, j] = np.dot(channel_inv, signal[i, j])

        return equalized_signal