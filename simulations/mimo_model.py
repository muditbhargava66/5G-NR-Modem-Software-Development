import numpy as np

class MIMOModel:
    def __init__(self, num_tx_antennas, num_rx_antennas, channel_type, modulation_type):
        self.num_tx_antennas = num_tx_antennas
        self.num_rx_antennas = num_rx_antennas
        self.channel_matrix = None
        self.noise_variance = 0.1
        self.channel_type = channel_type
        self.modulation_type = modulation_type

    def generate_random_signal(self):
        """Generate a random input signal based on the modulation type."""
        if self.modulation_type == 'qpsk':
            # Generate QPSK symbols
            signal = np.random.choice([-1, 1], size=(self.num_tx_antennas, 1))
        elif self.modulation_type == '16qam':
            # Generate 16-QAM symbols
            signal = np.random.choice([-3, -1, 1, 3], size=(self.num_tx_antennas, 1))
        elif self.modulation_type == '64qam':
            # Generate 64-QAM symbols
            signal = np.random.choice([-7, -5, -3, -1, 1, 3, 5, 7], size=(self.num_tx_antennas, 1))
        else:
            raise ValueError(f"Invalid modulation type: {self.modulation_type}")

        return signal

    def generate_channel_matrix(self):
        """Generate a random channel matrix based on the channel type."""
        np.random.seed(42)  # Set a fixed random seed for reproducibility

        if self.channel_type == 'rayleigh':
            # Generate Rayleigh fading channel coefficients
            self.channel_matrix = np.random.rayleigh(scale=1.0, size=(self.num_rx_antennas, self.num_tx_antennas))
        elif self.channel_type == 'rician':
            # Generate Rician fading channel coefficients
            k_factor = 5  # Rician K-factor
            mean = np.sqrt(k_factor / (k_factor + 1))
            std_dev = np.sqrt(1 / (2 * (k_factor + 1)))
            self.channel_matrix = np.random.normal(loc=mean, scale=std_dev, size=(self.num_rx_antennas, self.num_tx_antennas))
        else:
            raise ValueError(f"Invalid channel type: {self.channel_type}")

    def apply_channel(self, signal):
        """Apply the channel matrix to the input signal."""
        if self.channel_matrix is None:
            self.generate_channel_matrix()

        output_signal = np.dot(self.channel_matrix, signal)
        return output_signal

    def add_noise(self, signal):
        """Add Gaussian noise to the signal."""
        noise = np.sqrt(self.noise_variance / 2) * (np.random.randn(*signal.shape) + 1j * np.random.randn(*signal.shape))
        return signal + noise

    def equalize_signal(self, signal):
        """Perform signal equalization."""
        channel_pinv = np.linalg.pinv(self.channel_matrix)
        equalized_signal = np.dot(channel_pinv, signal)
        return equalized_signal