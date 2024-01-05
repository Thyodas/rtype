# Code Formatting and Linting Guide

## Overview

In our project, maintaining a consistent code style and adhering to best practices is crucial. To ensure this, we use `clang-tidy` as our linter and code analysis tool. This document provides guidelines on how to use `clang-tidy` for code formatting and linting in our project.

## Clang-Tidy Setup

### Installing Clang-Tidy

To install `clang-tidy`:

- **Ubuntu:**
  ```bash
  sudo apt-get install clang-tidy
  ```
- **Windows:**
  Use Chocolatey package manager:
  ```powershell
  choco install llvm
  ```

### Configuration

The configuration for `clang-tidy` is managed through a `.clang-tidy` file located at the root of the repository. This file defines the set of rules and checks that `clang-tidy` will apply to the code.

## Running Clang-Tidy

### Locally

To run `clang-tidy` locally, navigate to the root of the project and run:

```bash
clang-tidy [options] <source-files>
```

Replace `<source-files>` with the path to the source files you want to analyze.

### In CI/CD Pipeline

`clang-tidy` is integrated into our CI/CD pipeline, which automatically checks each push and pull request. If `clang-tidy` finds any issues, the CI/CD pipeline will fail, and you will need to address these issues.

## Code Style Guidelines

We follow a specific set of coding standards to maintain consistency across our codebase. Here are some key guidelines:

- **Indentation:** Use 4 spaces for indentation, not tabs.
- **Braces:** Use K&R style for braces.
- **Naming Conventions:**
    - **Classes/Structures:** Use CamelCase.
    - **Functions/Methods:** Use camelCase.
    - **Variables:** Use snake_case.
- **Comments:** Use doxygen-style comments for documenting classes, methods, and complex logic.

## Handling Linting Errors

When `clang-tidy` reports an issue, it's important to address it promptly:

- Review the error message and understand what needs to be fixed.
- Make the necessary changes in your code.
- Re-run `clang-tidy` to ensure the issue is resolved.

## Additional Resources

For more details on `clang-tidy` and the specific checks it provides, visit the [LLVM Clang-Tidy documentation](https://clang.llvm.org/extra/clang-tidy/).
