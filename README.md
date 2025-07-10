# RU HH Vacancy Viewer

![License](https://img.shields.io/github/license/KOJIMEISTER/ru_hh_vacancy_viewer)

Welcome to the **RU HH Vacancy Viewer** – a powerful desktop application designed to streamline your job search by leveraging the HeadHunter (HH) API and advanced neural network classification.

## Table of Contents

- [🔍 Introduction](#-introduction)
- [✨ Features](#-features)
- [🛠️ Technology Stack](#️-technology-stack)
- [📸 Screenshots](#-screenshots)
- [💾 Installation](#-installation)
- [🚀 Usage](#-usage)
  - [Training Application](#training-application)
  - [Vacancy Viewer Application](#vacancy-viewer-application)
- [🤝 Contributing](#-contributing)
- [📄 License](#-license)
- [📫 Contact](#-contact)

## 🔍 Introduction

The RU HH Vacancy Viewer is a comprehensive desktop application tailored for IT professionals seeking job opportunities in Russia. By interfacing with the HeadHunter (HH) API, the application not only fetches real-time vacancy listings but also employs a custom-trained neural network model to classify vacancies by development areas. This enables users to quickly understand the relevant domain of each vacancy without the need to open and read through each listing manually.

## ✨ Features

- **Real-Time Vacancy Fetching:** Seamlessly retrieve up-to-date job listings from the HH API.
- **Neural Network Classification:** Automatically categorize vacancies into specific development areas using a custom-trained model.
- **Efficient Data Handling:** Prevents duplicate vacancy views and maintains a history of viewed positions.
- **User-Friendly Interface:** Intuitive desktop application built with Qt for a smooth user experience.
- **Dataset Management:** Tools for training the neural network and managing the dataset with ease.
- **Customizable Search Parameters:** Refine job searches based on roles, skills, cities, and more.
- **Performance Optimization:** Dynamic loading and unloading of vacancy information to conserve system and network resources.

## 🛠️ Technology Stack

- **Programming Languages:** C++, Python
- **Frameworks & Libraries:** Qt Quick/QML, Boost.Python, Transformers (Hugging Face), NLTK
- **Database:** PostgreSQL, SQLite
- **Others:** Boost, PyTorch

## 📸 Screenshots

<div align="center">
  <img src="https://github.com/KOJIMEISTER/ru_hh_vacancy_viewer/raw/main/screenshots/main_view.png" alt="Main View" width="600"/>
  <img src="https://github.com/KOJIMEISTER/ru_hh_vacancy_viewer/raw/main/screenshots/training_app.png" alt="Training Application" width="600"/>
</div>

## 💾 Installation

### Prerequisites

- **Operating System:** Windows 10/11, macOS, or Linux
- **Qt:** Version 5.15 or later
- **C++ Compiler:** Compatible with C++17
- **Python:** Version 3.8 or later
- **PostgreSQL:** For backend database

### Steps

1. **Clone the Repository:**

   ```bash
   git clone https://github.com/KOJIMEISTER/ru_hh_vacancy_viewer.git
   cd ru_hh_vacancy_viewer
   ```

2. **Setup the Backend Database:**

   - Install PostgreSQL and ensure it's running.
   - Create a database named `itddb`.
   - Update the `config.json` file with your PostgreSQL credentials.

3. **Install Python Dependencies:**

   ```bash
   pip install -r requirements.txt
   ```

4. **Build the Applications:**

   - **Training Application:**
     - Navigate to the `trainer` directory.
     - Open the project in Qt Creator and build.
   
   - **Vacancy Viewer Application:**
     - Navigate to the `viewer_app` directory.
     - Open the project in Qt Creator and build.

## 🚀 Usage

The project comprises two main applications:

### Training Application

This application is responsible for training the neural network model and managing the dataset.

1. **Launch the Training App:**
   - Execute the built training application.

2. **Configure Training Parameters:**
   - Select the development areas (roles) you wish to include.
   - Set the number of batches and epochs for training.
   - Specify paths for saving the trained model.

3. **Start Training:**
   - Click on the "Train" button to commence the training process.
   - Monitor training progress and completion status.

4. **Manage Dataset:**
   - Add or remove key skills and cities to refine the dataset.
   - View and edit existing dataset entries.

### Vacancy Viewer Application

This is the main application for viewing and interacting with job vacancies.

1. **Launch the Vacancy Viewer:**
   - Execute the built vacancy viewer application.

2. **Set Search Preferences:**
   - Choose roles, skills, and cities to filter job listings.
   - Adjust search parameters as needed.

3. **View Classified Vacancies:**
   - Browse through the list of vacancies, each classified by development area.
   - Click on a vacancy to view detailed information without leaving the main interface.

4. **Manage Viewed Vacancies:**
   - Track which vacancies have been viewed to avoid repetition.
   - Access history and revert to previous searches if necessary.

## 🤝 Contributing

Contributions are welcome! Whether it's reporting bugs, suggesting features, or submitting pull requests, your input is valuable.

1. **Fork the Repository**
2. **Create a Feature Branch:**

   ```bash
   git checkout -b feature/NewFeature
   ```

3. **Commit Your Changes:**

   ```bash
   git commit -m "Add new feature"
   ```

4. **Push to the Branch:**

   ```bash
   git push origin feature/NewFeature
   ```

5. **Open a Pull Request**

Please ensure your contributions adhere to the project's coding standards and include appropriate tests.

## 📄 License

This project is licensed under the [MIT License](LICENSE).

## 📫 Contact

- **Author:** Nikita Ryabov
- **GitHub:** [KOJIMEISTER](https://github.com/KOJIMEISTER)

Feel free to reach out for any queries or support regarding the project!

---

*Happy Job Hunting! 🚀*
