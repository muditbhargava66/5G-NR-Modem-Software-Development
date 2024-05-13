# 5G NR Modem Software Development

This project aims to design, implement, and test a 5G NR modem software component, focusing on the Radio Access Stratum (RAS) layer. The project demonstrates expertise in modem software development, RF and WWAN L1/L2/L3 specifications, and collaboration with cross-functional teams.

## Table of Contents
- [Directory Structure](#directory-structure)
- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
  - [Linux](#linux)
  - [Windows](#windows)
- [Running the Tests](#running-the-tests)
  - [Linux](#linux-1)
  - [Windows](#windows-1)
- [Simulation and Modeling](#simulation-and-modeling)
- [Simulation Results](#simulation-results)
- [Mathematical Analysis](#mathematical-analysis)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [License](#license)

## Directory Structure

The project follows a well-organized directory structure to ensure code modularity, maintainability, and ease of navigation. Here's an overview of the important directories and files:

```
5G-NR-Modem-Software-Development/
├── code/
│   ├── driver/
│   │   ├── uart.h
│   │   └── uart.c
│   ├── ras/
│   │   ├── include/
│   │   │   └── ras.h
│   │   └── src/
│   │       └── ras.c
│   └── test/
│       └── ras_test.c
├── docs/
│   ├── ras_design_documentation.md
│   ├── ras_test_plan.md
│   ├── ras_test_report.md
│   └── images/
├── simulations/
│   ├── __init__.py
│   ├── mimo_model.py
│   ├── ofdm_model.py
│   ├── ras_model.py
│   └── ras_simulation.py
├── tools/
│   ├── ras_build_linux.sh
│   ├── ras_build_script.sh
│   ├── ras_build.bat
│   ├── ras_test_linux.sh
│   ├── ras_test_script.sh
│   └── ras_test.bat
├── .gitignore
├── manage
├── README.md
├── requirements.txt
└── LICENSE
```

## Getting Started

To get started with the project, follow these steps:

1. Clone the repository:
   ```
   git clone https://github.com/muditbhargava66/5G-NR-Modem-Software-Development.git
   ```

2. Navigate to the project directory:
   ```
   cd 5G-NR-Modem-Software-Development
   ```

3. Install the required dependencies:
   ```
   pip install -r requirements.txt
   ```

4. Review the project documentation in the `docs/` directory to understand the design, test plan, and test report.

## Prerequisites

Before building and running the project, ensure that you have the following prerequisites installed:

- GCC Compiler
- Python (version 3.10 or higher)
- Matplotlib (version 3.8.4 or higher)

## Building the Project

### Linux

To build the RAS layer, test framework, and UART driver on Linux, run the following command:
```
./tools/ras_build_linux.sh
```

### Windows

To build the RAS layer, test framework, and UART driver on Windows, run the following command:
```
tools\ras_build.bat
```

## Running the Tests

### Linux

To run the test framework and generate the test reports on Linux, use the following command:
```
./tools/ras_test_linux.sh
```

### Windows

To run the test framework and generate the test reports on Windows, use the following command:
```
tools\ras_test.bat
```

## Simulation and Modeling

The project includes simulation models and scripts to evaluate the performance and behavior of the 5G NR modem. The simulations cover different channel models (RAS, OFDM, MIMO) and modulation schemes (QPSK, 16-QAM, 64-QAM).

To run the simulations, use the following command:
```
python simulations/ras_simulation.py
```

## Simulation Results

The simulation results provide insights into the performance of the 5G NR modem under different channel conditions and modulation schemes. The average BER is calculated for each scenario, and the results are displayed in the terminal.

Example simulation results:
```
Running RAS simulation with rayleigh channel and QPSK modulation...
Average BER for RAS with rayleigh channel and QPSK modulation: 0.510215

Running OFDM simulation with rician channel and 16QAM modulation...
Average BER for OFDM with rician channel and 16QAM modulation: 0.841766

Running MIMO simulation with rayleigh channel and 64QAM modulation...
Average BER for MIMO with rayleigh channel and 64QAM modulation: 0.904750

Simulation Results:
Model: ras, Channel: rayleigh, Modulation: qpsk, Average BER: 0.510215
Model: ofdm, Channel: rician, Modulation: 16qam, Average BER: 0.841766
Model: mimo, Channel: rayleigh, Modulation: 64qam, Average BER: 0.904750
```

## Mathematical Analysis

The simulation results can be analyzed mathematically to understand the performance differences between the different models and modulation schemes.

- RAS (Random Access Stratum) model:
  - The RAS model simulates the basic random access procedure in the 5G NR network.
  - The BER performance of the RAS model depends on factors such as the number of subcarriers, number of symbols, and the modulation scheme used.
  - The RAS model with QPSK modulation achieves an average BER of 0.510215, indicating that approximately 51% of the bits are in error.

- OFDM (Orthogonal Frequency Division Multiplexing) model:
  - The OFDM model simulates the multi-carrier modulation technique used in 5G NR.
  - OFDM divides the available bandwidth into multiple orthogonal subcarriers, allowing for efficient spectrum utilization and robustness against multipath fading.
  - The BER performance of the OFDM model is influenced by the number of subcarriers, cyclic prefix length, and the modulation scheme.
  - The OFDM model with 16-QAM modulation and Rician channel achieves an average BER of 0.841766, indicating a higher error rate compared to the RAS model.

- MIMO (Multiple Input Multiple Output) model:
  - The MIMO model simulates the use of multiple antennas at both the transmitter and receiver to improve system capacity and reliability.
  - MIMO exploits spatial diversity and multiplexing to enhance the signal quality and data throughput.
  - The BER performance of the MIMO model depends on factors such as the number of transmit and receive antennas, channel conditions, and the modulation scheme.
  - The MIMO model with 64-QAM modulation and Rayleigh channel achieves an average BER of 0.904750, the highest among the simulated scenarios.

The mathematical analysis suggests that the RAS model with QPSK modulation performs better in terms of BER compared to the OFDM and MIMO models with higher-order modulation schemes. However, it's important to note that the performance may vary depending on the specific channel conditions and system parameters.

Further mathematical analysis can be conducted to derive theoretical BER expressions for each model and modulation scheme, taking into account factors such as signal-to-noise ratio (SNR), channel fading characteristics, and diversity techniques employed.

## Documentation

Detailed documentation for the project can be found in the [docs/](docs/) directory. The following documents are available:

- [ras_design_documentation.md](docs/ras_design_documentation.md): Provides a comprehensive explanation of the RAS layer design, including architecture, interfaces, and algorithms.
- [ras_test_plan.md](docs/ras_test_plan.md): Outlines the test plan for the RAS layer, specifying the test cases, expected results, and test coverage.
- [ras_test_report.md](docs/ras_test_report.md): Presents the test report with the actual results, analysis, and any identified issues or improvements.

Please refer to these documents for a deeper understanding of the project's design, testing strategy, and outcomes.

## Contributing

Contributions to the project are welcome! If you find any issues or have suggestions for improvements, please follow these steps:

1. Fork the repository.
2. Create a new branch for your feature or bug fix.
3. Make your changes and commit them with descriptive messages.
4. Push your changes to your forked repository.
5. Submit a pull request, explaining your changes and their benefits.

Please ensure that your contributions adhere to the project's coding standards and guidelines.

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code as per the terms of the license.

---

For any further questions or inquiries, please contact the project maintainer at [muditbhargava66](https://github.com/muditbhargava66).

Happy coding!

---