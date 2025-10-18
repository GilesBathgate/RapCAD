# RapCAD Coding Style Guide

This document outlines the coding style and conventions used in the RapCAD project. Adhering to these guidelines will help maintain a consistent and readable codebase.

## Naming Conventions

-   **Classes and Structs:** Use PascalCase (e.g., `BreadboardDesigner`, `MainWindow`).
-   **Methods and Functions:** Use camelCase (e.g., `buildHoles`, `paintEvent`).
-   **Variables:** Use camelCase (e.g., `clickedHole`, `movingConnectionId`).
-   **Private Member Variables:** Use camelCase (e.g., `model`, `controller`).
-   **UI Elements:** Prefix with `ui->` (e.g., `ui->tabWidget`, `ui->actionNew`).
-   **Constants and Enums:** Use PascalCase for enum names and enum values (e.g., `CreationMode`, `NotCreating`).

## Class Design

-   **Pimpl Idiom:** The Pimpl (pointer to implementation) idiom is not used. Private members are declared directly in the header file.
-   **Inheritance:** The codebase makes extensive use of inheritance, particularly from Qt's `QObject` and its subclasses.
-   **RAII:** The RAII (Resource Acquisition Is Initialization) pattern is used for managing resources, with object ownership typically handled by Qt's parent-child mechanism.

## C++ and Qt Features

-   **C++11:** The codebase uses C++11 features, including range-based for loops, lambdas, and the `auto` keyword.
-   **Qt:** The project is heavily reliant on the Qt framework. Use Qt's containers (e.g., `QVector`, `QString`, `QList`) and other utilities where appropriate.
-   **Signals and Slots:** Use Qt's signals and slots mechanism for communication between objects.
-   **Smart Pointers:** The codebase does not appear to use smart pointers (e.g., `std::unique_ptr`, `std::shared_ptr`). Object lifetime is managed manually, often with the help of Qt's parent-child system.
