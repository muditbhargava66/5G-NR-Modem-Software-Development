# 5G NR Modem Software Development

This project aims to design, implement, and test a 5G NR modem software component, focusing on the Radio Access Stratum (RAS) layer. The project demonstrates expertise in modem software development, RF and WWAN L1/L2/L3 specifications, and collaboration with cross-functional teams.

## Table of Contents
- [Directory Structure](#directory-structure)
- [Getting Started](#getting-started)
- [Prerequisites](#prerequisites)
- [Building the Project](#building-the-project)
- [Running the Tests](#running-the-tests)
- [Simulation and Modeling](#simulation-and-modeling)
- [Simulation Results](#simulation-results)
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
│   └── ras_test_report.md
├── simulations/
│   ├── __init__.py
│   ├── mimo_model.py
│   ├── ofdm_model.py
│   ├── ras_model.py
│   └── ras_simulation.py
├── tools/
│   ├── ras_build_script.sh
│   └── ras_test_script.sh
├── .gitignore
├── README.md
└── LICENSE
```

- `code/`: Contains the source code for the RAS layer implementation, test framework, and UART driver.
  - `driver/`: Contains the UART driver implementation.
    - `uart.h`: Header file for the UART driver.
    - `uart.c`: Source file implementing the UART driver functionality.
  - `ras/`: Implements the RAS layer functionality.
    - `include/`: Contains the header file `ras.h` declaring the RAS layer API.
    - `src/`: Contains the source file `ras.c` implementing the RAS layer functionality.
  - `test/`: Contains the test framework for the RAS layer.
    - `ras_test.c`: Implements the test cases for the RAS layer.
- `docs/`: Contains the project documentation.
  - `ras_design_documentation.md`: Provides the design documentation for the RAS layer.
  - `ras_test_plan.md`: Outlines the test plan for the RAS layer.
  - `ras_test_report.md`: Presents the test report with results and analysis.
- `simulations/`: Contains the simulation models and scripts.
  - `__init__.py`: Package initialization file for the `simulations` package.
  - `mimo_model.py`: Implements the MIMO channel model.
  - `ofdm_model.py`: Implements the OFDM channel model.
  - `ras_model.py`: Implements the RAS channel model.
  - `ras_simulation.py`: Simulates the behavior of the 5G NR modem with different channel models and modulation schemes.
- `tools/`: Contains the build and test scripts.
  - `ras_build_script.sh`: Builds the RAS layer, test framework, and UART driver.
  - `ras_test_script.sh`: Runs the test framework and generates test reports.
- `.gitignore`: Specifies files and directories to be ignored by version control.
- `README.md`: Provides an overview and instructions for the project.
- `LICENSE`: Contains the license information for the project.

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

3. Review the project documentation in the `docs/` directory to understand the design, test plan, and test report.

## Prerequisites

Before building and running the project, ensure that you have the following prerequisites installed:

- GCC Compiler
- Python (version 3.10 or higher)

## Building the Project

To build the RAS layer, test framework, and UART driver, run the following command:
```
./tools/ras_build_script.sh
```

This script will compile the source code and generate the necessary object files and libraries.

## Running the Tests

To run the test framework and generate the test reports, use the following command:
```
./tools/ras_test_script.sh
```

The test script will execute the test cases defined in `code/test/ras_test.c` and provide the test results and analysis.

## Simulation and Modeling

The project includes simulation models and scripts to evaluate the performance and behavior of the 5G NR modem. The simulations cover different channel models (RAS, OFDM, MIMO) and modulation schemes (QPSK, 16-QAM, 64-QAM).

To run the simulations, use the following command:
```
python simulations/ras_simulation.py
```

The simulation script will execute the modeling and simulation algorithms for various scenarios and provide the average Bit Error Rate (BER) for each scenario.

## Simulation Results

The simulation results provide insights into the performance of the 5G NR modem under different channel conditions and modulation schemes. The average BER is calculated for each scenario, and the results are plotted using Matplotlib for visual comparison.

For a detailed analysis of the simulation results, refer to the [Simulation Results Report](docs/simulation_results_report.md) in the `docs/` directory.

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