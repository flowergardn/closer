# closer

An application written to automatically close any application upon keybind. 

## But, why?

This is my first actual C++ project, I made it purely to solve a single problem I had; That being me unable to close programs without opening task manager, or opening the program itself. Basically it's just a stealth way to close programs.

## Download

You can download the latest version [here](https://github.com/astridlol/closer/releases/latest)

### Configuration

The only configuration able to be done is the processName, and whether to run in the background or not.

Example:

```json
{
  "processName": "firefox.exe",
  "hidden": true
}
```