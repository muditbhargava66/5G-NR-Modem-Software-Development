import numpy as np
from models.ras_model import RASModel # noqa: F401
import concurrent.futures
import sys
import os

class RASSimulation:
    def __init__(self, num_subcarriers, num_symbols, num_antennas):
        self.model = RASModel(num_subcarriers, num_symbols, num_antennas)
        self.num_subcarriers = num_subcarriers
        self.num_symbols = num_symbols
        self.num_antennas = num_antennas

    def generate_random_signal(self):
        """Generate a random input signal."""
        real_part = np.random.randint(0, 2, size=(self.num_subcarriers, self.num_symbols, self.num_antennas))
        imag_part = np.random.randint(0, 2, size=(self.num_subcarriers, self.num_symbols, self.num_antennas))
        signal = real_part + 1j * imag_part
        return signal

    def simulate_transmission(self, signal):
        """Simulate the transmission of a signal through the channel."""
        channel_output = self.model.apply_channel(signal)
        noisy_output = self.model.add_noise(channel_output)
        equalized_output = self.model.equalize_signal(noisy_output)
        return equalized_output

    def calculate_ber(self, input_signal, output_signal):
        """Calculate the Bit Error Rate (BER) between input and output signals."""
        num_bits = input_signal.size
        num_errors = np.sum(np.abs(input_signal - output_signal) > 0.5)
        ber = num_errors / num_bits
        return ber

    def run_simulation(self, num_iterations, num_workers):
        """Run the RAS simulation for a specified number of iterations using multiple workers."""
        with concurrent.futures.ThreadPoolExecutor(max_workers=num_workers) as executor:
            futures = [executor.submit(self._run_single_simulation) for _ in range(num_iterations)]
            results = [future.result() for future in concurrent.futures.as_completed(futures)]
        average_ber = sum(results) / num_iterations
        return average_ber

    def _run_single_simulation(self):
        """Run a single iteration of the simulation."""
        input_signal = self.generate_random_signal()
        output_signal = self.simulate_transmission(input_signal)
        ber = self.calculate_ber(input_signal, output_signal)
        return ber

# Example usage
if __name__ == '__main__':
    # Add the project root directory to the Python module search path
    sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    num_subcarriers = 64
    num_symbols = 100
    num_antennas = 4
    num_iterations = 1000
    num_workers = 4  # Adjust the number of workers according to your system's capabilities

    simulation = RASSimulation(num_subcarriers, num_symbols, num_antennas)
    average_ber = simulation.run_simulation(num_iterations, num_workers)
    print(f"Average BER: {average_ber:.6f}")