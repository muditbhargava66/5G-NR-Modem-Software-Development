import numpy as np
from ras_model import RASModel
from ofdm_model import OFDMModel
from mimo_model import MIMOModel
import concurrent.futures
import sys
import os
import matplotlib.pyplot as plt

class RASSimulation:
    def __init__(self, num_subcarriers, num_symbols, num_antennas, model_type, channel_type, modulation_type):
        if model_type == 'ras':
            self.model = RASModel(num_subcarriers, num_symbols, num_antennas, channel_type, modulation_type)
        elif model_type == 'ofdm':
            self.model = OFDMModel(num_subcarriers, num_symbols, num_antennas, cyclic_prefix_length=16, channel_type=channel_type, modulation_type=modulation_type)
        elif model_type == 'mimo':
            self.model = MIMOModel(num_tx_antennas=num_antennas, num_rx_antennas=num_antennas, channel_type=channel_type, modulation_type=modulation_type)
        else:
            raise ValueError(f"Invalid model type: {model_type}")

        self.num_subcarriers = num_subcarriers
        self.num_symbols = num_symbols
        self.num_antennas = num_antennas
        self.model_type = model_type
        self.channel_type = channel_type
        self.modulation_type = modulation_type

    def generate_random_signal(self):
        """Generate a random input signal."""
        signal = self.model.generate_random_signal()
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
    # Set a fixed random seed for reproducibility
    np.random.seed(42)

    # Add the project root directory to the Python module search path
    sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

    num_iterations = 1000
    num_workers = 4

    # Define the parameters for different scenarios
    scenarios = [
        {'model_type': 'ras', 'num_subcarriers': 64, 'num_symbols': 100, 'num_antennas': 4, 'channel_type': 'rayleigh', 'modulation_type': 'qpsk'},
        {'model_type': 'ofdm', 'num_subcarriers': 128, 'num_symbols': 200, 'num_antennas': 8, 'channel_type': 'rician', 'modulation_type': '16qam'},
        {'model_type': 'mimo', 'num_subcarriers': 256, 'num_symbols': 150, 'num_antennas': 16, 'channel_type': 'rayleigh', 'modulation_type': '64qam'}
    ]

    # Run simulations for each scenario and store the results
    results = []
    for scenario in scenarios:
        simulation = RASSimulation(**scenario)
        average_ber = simulation.run_simulation(num_iterations, num_workers)
        results.append({'scenario': scenario, 'average_ber': average_ber})
        print(f"Model: {scenario['model_type']}, Channel: {scenario['channel_type']}, Modulation: {scenario['modulation_type']}, Average BER: {average_ber:.6f}")

    # Plot the BER results for different scenarios
    fig, ax = plt.subplots()
    x = np.arange(len(results))
    ber_values = [result['average_ber'] for result in results]
    ax.bar(x, ber_values)
    ax.set_xticks(x)
    ax.set_xticklabels([f"{result['scenario']['model_type']}\n{result['scenario']['channel_type']}\n{result['scenario']['modulation_type']}" for result in results])
    ax.set_xlabel('Scenario')
    ax.set_ylabel('Average BER')
    ax.set_title('BER Performance for Different Scenarios')
    plt.tight_layout()
    plt.show()