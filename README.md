# Bounty of Two

## Cloning the Project

To clone the repository including all submodules, run:

```bash
git clone --recurse-submodules https://github.com/majvax/bounty_of_two.git
cd bount_of_two
```

If you already cloned without submodules, initialize them with:

```bash
git submodule update --init --recursive
```

## Building the Project

The project uses Premake as the build system. Depending on your platform, use one of the following scripts to generate and build the project:

- On **Windows**:

  - To build with Make:

    ```bash
    ./tools/windows/Make.bat
    ```

  - To generate and open the Visual Studio 2022 solution:

    ```bash
    ./tools/windows/VS2022.bat
    ```

- On **Linux** (It uses gmake by default):

  ```bash
  ./tools/linux/Linux.sh
  ```

## Cleaning the Project

To clean the build files and reset the project state, use the appropriate clean script:

- On **Windows**:

  ```bash
  ./tools/windows/clean.bat
  ```

- On **Linux**:

  ```bash
  ./tools/linux/clean.sh
  ```
