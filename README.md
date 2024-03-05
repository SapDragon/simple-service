# simple-service
A Windows service application developed in C++ to fetch currency rates using data from the National Bank of Ukraine (NBU) API.

## Installation
1. Build the project using Visual Studio 2019.
2. Run compiled executable with the `install` argument to install the service. For example:
```cmd
simple-service.exe install
```

For uninstallation, use the `uninstall` argument:
```cmd
simple-service.exe uninstall
```

## Configuration
Config saved to 'Drive:\MyService\config.json'
Json fields:
- `Interval` - interval in ms to fetch currency rates
- `DataType` - json / xms / csv. Type of data to fetch
- `JsonUrl` - Link to NBU API for json data
- `XmlUrl` - Link to NBU API for xml data
- `Work Mode` - 0 - overwrite file, 1 - create new file with timestamp
- `LogDirectory` - Path to output directory ( Can be relative to the executable or absolute path)

## Libraries used
- [nlohmann/json] - JSON for Modern C++. For parsing and creating JSON data.

## Control codes
- 0xDEAD ( Reload Config ) - Reloads the configuration from the config file

## Troubleshooting
If you encounter any issues while using the Currency Rates Fetcher service, consider the following:
- Check the logs for any error messages or warnings.
- Verify that the configuration file or registry entries are properly set.
- Ensure that the service has the necessary permissions to access the output file path.
- Make sure that the NBU API is accessible and responding correctly.

### Docs about structure code
[Docs about structure code](docs/structure.md)

