# Simple Thread Manager

Please Note this is under development, documentiation needs to be updated!!!

This project implements a simple thread manager in C++. It provides functionality to manage and execute processes concurrently across multiple threads.

## Features

- **Process Control**: Add, manage, and execute processes with dependencies and priority levels.
- **Thread Management**: Control the number of threads used for process execution.
- **Data Handling**: Shared data management with thread safety.

## Usage

### Adding Processes

Processes can be added using either a custom class derived from `IProcess` or a lambda function.

#### Example using a custom process class:

```cpp
std::shared_ptr<IProcess> process = std::make_shared<ProcessTest>("A",  5000, "test 1");
control.AddProcess("A",  process);
```

#### Example using a lambda function:

```cpp
control.AddProcess("E",
                   [](std::shared_ptr<Data> data){
                       if(data) data->SetData("TEST", "TEST aaa" );
                   },
                   {"A"});
```

### Executing Processes

Processes can be executed either with or without providing shared data.

```cpp
// With shared data
std::shared_ptr<Data> data = std::make_shared<Data>();
control.Run(data);

// Without shared data
control.Run();
```

### Dependencies and Priority Levels

Processes can have dependencies and priority levels assigned to control the execution order.

```cpp
// Adding a process with dependencies and priority level
control.AddProcess("D1", process4, {"B", "A"}, 2);
```

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/your-username/simple-thread-manager.git
   ```
2. Build the project using a C++ compiler:
   ```bash
   cd simple-thread-manager
   g++ -std=c++11 -o main main.cpp
   ```
3. Run the executable:
   ```bash
   ./main
   ```

## Contributing

Contributions are welcome! Please create a pull request or open an issue for any enhancements or bug fixes.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
